#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <memory>

class ImageProcessor
{
public:
  ImageProcessor();
  cv::Mat preprocessImage(const cv::Mat &image);
  cv::Mat warpImage(const cv::Mat &image, const std::vector<cv::Point> &sudokuContour);
  std::vector<std::vector<int>> extractSudokuMatrix(const cv::Mat &image);

private:
  cv::Mat convertToGray(const cv::Mat &image);
  cv::Mat applyGaussianBlur(const cv::Mat &grayImage);
  cv::Mat applyThreshold(const cv::Mat &grayImage);
  std::vector<std::vector<cv::Point>> findContours(const cv::Mat &thresh);
  std::vector<cv::Point> findLargestContour(const std::vector<std::vector<cv::Point>> &contours);
};
