#include "SudokuSolver.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_image>" << std::endl;
        return -1;
    }

    std::string imagePath = argv[1];
    cv::Mat image = cv::imread(imagePath);
    if (image.empty())
    {
        std::cerr << "Could not open or find the image" << std::endl;
        return -1;
    }

    SudokuSolver solver;
    solver.processImage(image);
    solver.displayBoard();

    return 0;
}
