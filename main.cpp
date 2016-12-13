#include <iostream>
#include "PhaseShiftProcess.h"
#define COLOR_YES_NO CV_LOAD_IMAGE_COLOR

int main()
{
    Mat img1 = imread("../image/phase1.jpg", COLOR_YES_NO );
    Mat img2 = imread("../image/phase2.jpg", COLOR_YES_NO );
    Mat img3 = imread("../image/phase3.jpg", COLOR_YES_NO );

    if (!img1.data || !img2.data || !img3.data)
    {
        cout << "loi roi" << endl;
        throw "Error! Can't load image data";
    }

    imshow("Image1", img1);
    PhaseShiftProcess ps(img1, img2, img3);
    ps.phaseDecode();
    ps.computeQualityMap();
    ps.unwrapPhase();
    cvWaitKey(0);

    return 0;
}
