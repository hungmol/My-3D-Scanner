#ifndef PHASESHIFTPROCESS_H
#define PHASESHIFTPROCESS_H

#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include "algorithm"

using namespace std;
using namespace cv;

class PhaseShiftProcess
{
public:
    PhaseShiftProcess(Mat _imgPhase1, Mat _imgPhase2, Mat _imgPhase3);
    ~PhaseShiftProcess();

    float getZSkew() const {return this->zskew;}

    void setZSkew(float zSkew)  {this->zskew = zSkew;}

    float getZScale() const {return this->zscale;}

    void setZScale(float zScale) {this->zscale = zScale;}

    void phaseDecode();
    void phaseUnwrapped();
    void convertDepth();

    void computeQualityMap();
    void computeQuality_Zhang();
    float getIntensity(Vec3b phi);

//    void phaseUnwrap();
//    void phaseUnwrap(int x, int y, float phi, float q);
    void unwrapPhase();

protected:

    float sqdist(float v1, float v2)
    {
        float d = v1 - v2;
        return 1.f - d*d;
    }

private:
    Mat imgPhase1;
    Mat imgPhase2;
    Mat imgPhase3;
    Mat imgColor;  // reconstructed color image
    Mat imgWrappedPhase;
    Mat imgUnwrappedPhase;

    // some helper matrices to track phase quality and
    // processing state (each from the same dimension as the input image)
    Mat  mask;
    Mat  process;
    Mat quality;
    Mat range;
    float depth;
    float noiseThreshold;
    float zscale;
    float zskew;

    int width;
    int height;
    int step;   // for single channel images

};

#endif // PHASESHIFTPROCESS_H
