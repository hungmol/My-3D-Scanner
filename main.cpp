#include <iostream>
#include "PhaseShiftSrocess.h"
int main()
{
    Mat img1 = imread("../pattern/fringe1.png", CV_LOAD_IMAGE_COLOR);
    Mat img2 = imread("../pattern/fringe2.png", CV_LOAD_IMAGE_COLOR);
    Mat img3 = imread("../pattern/fringe3.png", CV_LOAD_IMAGE_COLOR);

    if (!img1.data || !img2.data || !img3.data)
    {
        cout << "loi roi" << endl;
        throw "Error! Can't load image data";
    }

    return 0;
}
