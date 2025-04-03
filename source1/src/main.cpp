#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

void showImage(const std::string &windowName, const cv::Mat &image)
{
    cv::imshow(windowName, image);
    cv::waitKey(0);
}

cv::Mat *drawContours(const cv::Mat &image, const std::vector<std::vector<cv::Point>> &contours)
{
    cv::Mat *output = new cv::Mat(image.clone());
    for (const auto &contour : contours)
    {
        cv::drawContours(*output, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 255, 0), 2);
    }
    return output;
}

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

    double aspectRatio = static_cast<double>(image.cols) / image.rows;
    int newWidth = 600;
    int newHeight = static_cast<int>(newWidth / aspectRatio);
    cv::resize(image, image, cv::Size(newWidth, newHeight));

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.5);

    cv::Mat sobelX, sobelY, sobelEdges;
    cv::Sobel(blurred, sobelX, CV_64F, 1, 0, 3);
    cv::Sobel(blurred, sobelY, CV_64F, 0, 1, 3);
    cv::magnitude(sobelX, sobelY, sobelEdges);
    sobelEdges.convertTo(sobelEdges, CV_8U);
    showImage("Sobel Edges", sobelEdges);

    cv::Mat binaryEdges;
    cv::threshold(sobelEdges, binaryEdges, 50, 255, cv::THRESH_BINARY);
    showImage("Binary Edges", binaryEdges);

    cv::Mat erodedEdges;
    cv::erode(binaryEdges, erodedEdges, cv::Mat(), cv::Point(-1, -1), 2);
    showImage("Eroded Edges", erodedEdges);

    cv::Mat kernel;
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1));
    cv::morphologyEx(binaryEdges, binaryEdges, cv::MORPH_CLOSE, kernel);
    showImage("Closed Edges", binaryEdges);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryEdges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat *contourImage = drawContours(image, contours);

    showImage("All Contours", *contourImage);

    std::vector<std::vector<cv::Point>> filteredContours;
    for (const std::vector<cv::Point> &contour : contours)
    {
        double area = cv::contourArea(contour);
        if (area > 35000 && area < 305000)
            filteredContours.push_back(contour);
    }

    std::vector<cv::Point> largestContour = filteredContours[0];
    double perimeter = cv::arcLength(largestContour, true);
    std::vector<cv::Point> approx;
    cv::approxPolyDP(largestContour, approx, 0.031 * perimeter, true);
    cv::drawContours(image, std::vector<std::vector<cv::Point>>{approx}, -1, cv::Scalar(0, 255, 0), 2);

    cv::Point2f topLeft = approx[0];
    cv::Point2f topRight = approx[1];
    cv::Point2f bottomLeft = approx[2];
    cv::Point2f bottomRight = approx[3];
    cv::circle(image, topLeft, 5, cv::Scalar(255, 0, 0), -1);
    cv::circle(image, topRight, 5, cv::Scalar(255, 0, 0), -1);
    cv::circle(image, bottomLeft, 5, cv::Scalar(255, 0, 0), -1);
    cv::circle(image, bottomRight, 5, cv::Scalar(255, 0, 0), -1);
    showImage("Filtered Contours", image);

    //! warp the sobelEdges image so that the top-left point is at (0, 0), the top-right point is at (width, 0), the bottom-left point is at (0, height), and the bottom-right point is at (width, height)
    //! get the width and height of the new image
    //! subdivide the formed square into 9 equal parts horizontally and vertically
    //! show each square in a separate window to verify the correctness of the warp and if the squares are inline with the sudoku grid

    delete contourImage;
    cv::destroyAllWindows();
    return 0;
}
