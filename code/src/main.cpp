#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void webcam()
{
    cv::VideoCapture cap(0);
    cv::Mat img;

    while (true)
    {
        cap.read(img);
        cv::imshow("Image", img);
        cv::waitKey(1);
    }
}

int main(int argc, char **argv)
{
    webcam();
    return 0;
}
