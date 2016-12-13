#include "PhaseShiftProcess.h"
#define PI 3.1415926

PhaseShiftProcess::PhaseShiftProcess(Mat _imgPhase1, Mat _imgPhase2, Mat _imgPhase3):
    imgPhase1(_imgPhase1), imgPhase2(_imgPhase2), imgPhase3(_imgPhase3)
{
    width = imgPhase1.cols;
    height = imgPhase1.rows;

    if (imgPhase2.cols != width || imgPhase2.rows != height
            || imgPhase3.cols != width || imgPhase3.rows != height)
    {
        throw "invalid arguments: input images must have same dimension!";
    }

    imgColor = Mat::zeros(height, width, CV_8UC3);
    imgWrappedPhase = Mat::zeros(height, width, CV_32FC1);
    imgUnwrappedPhase = Mat::zeros(height, width, CV_32FC1);

    //    mask = Mat::zeros(height, width, CV_8UC1);
    process = Mat::zeros(height, width, CV_8UC1);
    range = Mat::zeros(height, width, CV_32FC1);
    quality = Mat::zeros(height, width, CV_32FC1);

    noiseThreshold = 0.2;
    zscale = 130;
    zskew = 24;

    // init step width for color and single channel images
    step  = width;

    cout << "Image width = " << width << endl
         << "Image height = " << height << endl;
}

PhaseShiftProcess::~PhaseShiftProcess()
{}

float PhaseShiftProcess::getIntensity(Vec3b phi)
{

    uchar blue = phi.val[0];
    uchar green = phi.val[1];
    uchar red = phi.val[2];
    return (blue + green + red) / (3.f * 255);
}

void PhaseShiftProcess::phaseDecode()
{
    float phi1, phi2, phi3;
    float phiSum = 0;
    float phiRange, minPhi, maxPhi;
    float noise;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            phi1 = getIntensity(imgPhase1.at<Vec3b>(Point(j, i)));
            phi2 = getIntensity(imgPhase2.at<Vec3b>(Point(j, i)));
            phi3 = getIntensity(imgPhase3.at<Vec3b>(Point(j, i)));
            phiSum = phi1 + phi2 + phi3;

            // Find min max phi value
            auto minMax = minmax({phi1, phi2, phi3});
            minPhi = minMax.first;
            maxPhi = minMax.second;

            phiRange = abs(maxPhi - minPhi);
            range.at<float>(Point(j, i)) = phiRange;
            noise = phiRange / phiSum;

            // Create the mask to remove noise background
            //            mask.at<uchar>(Point(j, i)) = (noise < noiseThreshold);
            process.at<uchar>(Point(j, i)) = (noise >= noiseThreshold);

            if (noise >= noiseThreshold)
            {
                imgWrappedPhase.at<float>(Point(j, i)) = (float)atan2(sqrt(3)*(phi1 - phi3),(2*phi2 - phi1 - phi3));
//                cout << imgWrappedPhase.at<float>(Point(j, i)) << endl;
            }
            else
            {
                imgWrappedPhase.at<float>(Point(j, i)) = 0.f;
            }
        }
    }
    Mat output;
    cv::normalize(imgWrappedPhase, output,255, 0, NORM_MINMAX, CV_8UC1);
    imshow("Wrapped phase", output);

    //    Mat mask_;
    //    cv::normalize(process, mask_,255, 0, NORM_MINMAX, CV_8UC1);
    //    imshow("Mask", mask_);
}

void PhaseShiftProcess::computeQualityMap()
{
    //    float temp = 0.0f;
    float phi_center = 0.0f;
    float phi_left = 0.0f;
    float phi_right = 0.0f;
    float phi_above = 0.0f;
    float phi_behind = 0.0f;

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            phi_center = imgWrappedPhase.at<float>(Point(j, i))/(2*PI);
            phi_left = imgWrappedPhase.at<float>(Point(j - 1, i))/(2*PI);
            phi_right = imgWrappedPhase.at<float>(Point(j + 1, i))/(2*PI);
            phi_above = imgWrappedPhase.at<float>(Point(j, i - 1))/(2*PI);
            phi_behind = imgWrappedPhase.at<float>(Point(j, i + 1))/(2*PI);

            //            float delta = 0.25*(abs(phi_center - phi_left) + abs(phi_center - phi_right)
            //                                           + abs(phi_center - phi_above) + abs(phi_center - phi_behind));
            //            quality.at<float>(Point(j, i)) = 1 - delta/(2*PI);
            quality.at<float>(Point(j, i)) = sqdist(phi_center, phi_above)+
                    sqdist(phi_center,phi_left)+
                    sqdist(phi_center, phi_right)+
                    sqdist(phi_center, phi_behind);

            quality.at<float>(Point(j, i)) /= range.at<float>(Point(j, i));
        }
    }

    //    Mat output;
    //    cv::normalize(quality, output,255, 0, NORM_MINMAX, CV_8UC1);
    imshow("Quality map", quality);
}

void PhaseShiftProcess::computeQuality_Zhang()
{
    //    float temp = 0.0f;
    float phi_center = 0.0f;
    float phi_left = 0.0f;
    float phi_right = 0.0f;
    float phi_above = 0.0f;
    float phi_behind = 0.0f;

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            phi_center = imgWrappedPhase.at<float>(Point(j, i));
            phi_left = imgWrappedPhase.at<float>(Point(j - 1, i));
            phi_right = imgWrappedPhase.at<float>(Point(j + 1, i));
            phi_above = imgWrappedPhase.at<float>(Point(j, i - 1));
            phi_behind = imgWrappedPhase.at<float>(Point(j, i + 1));

            float delta = 0.25*(abs(phi_center - phi_left) + abs(phi_center - phi_right)
                                + abs(phi_center - phi_above) + abs(phi_center - phi_behind));
            quality.at<float>(Point(j, i)) = 1 - delta;
        }
    }

    Mat output;
    cv::normalize(quality, output,255, 0, NORM_MINMAX, CV_8UC1);
    imshow("Quality map", output);
}

void PhaseShiftProcess::unwrapPhase()
{
    for (int i = 1; i < height-1; i++)
    {
        for (int j = 1; j < width-1; j++)
        {
            if (imgWrappedPhase.at<float>(Point(j, i)) - imgWrappedPhase.at<float>(Point(j ,i - 1)) > PI)
            {
                imgUnwrappedPhase.at<float>(Point(j, i)) = imgWrappedPhase.at<float>(Point(j, i)) - 2*PI;
            }

            else if (imgWrappedPhase.at<float>(Point(j, i)) - imgWrappedPhase.at<float>(Point(j, i - 1)) < -PI)
            {
                imgUnwrappedPhase.at<float>(Point(j, i)) = imgWrappedPhase.at<float>(Point(j, i)) + 2*PI;
            }

            else
            {
                imgUnwrappedPhase.at<float>(Point(j, i)) = imgWrappedPhase.at<float>(Point(j, i));
            }
        }
    }

    Mat output;
    cv::normalize(imgUnwrappedPhase, output,255, 0, NORM_MINMAX, CV_8UC1);
    imshow("Unwrapped phase", output);

}
