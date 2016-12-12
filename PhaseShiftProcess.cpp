#include "PhaseShiftProcess.h"

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


}

PhaseShiftProcess::~PhaseShiftProcess()
{}
