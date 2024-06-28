#include "ImageProcessor.hpp"
#include <iostream>

ImageProcessor::ImageProcessor() {}

cv::Mat ImageProcessor::preprocessImage(const cv::Mat &image)
{
  cv::Mat gray = convertToGray(image);
  cv::Mat blurred = applyGaussianBlur(gray);
  cv::Mat thresh = applyThreshold(gray);
  std::vector<std::vector<cv::Point>> contours = findContours(thresh);
  std::vector<cv::Point> sudokuContour = findLargestContour(contours);
  cv::Mat warped = warpImage(image, sudokuContour);

  // show each step
  cv::imshow("Original Image", image);
  cv::waitKey(0);
  cv::imshow("Gray Image", gray);
  cv::waitKey(0);
  cv::imshow("Blurred Image", blurred);
  cv::waitKey(0);
  cv::imshow("Threshold Image", thresh);
  cv::waitKey(0);

  // show sudoku contour on a copy of the original image
  cv::Mat imageCopy = image.clone();
  cv::drawContours(imageCopy, std::vector<std::vector<cv::Point>>{sudokuContour}, -1, cv::Scalar(0, 255, 0), 2);
  cv::imshow("Sudoku Contour", imageCopy);
  cv::waitKey(0);

  return image;
}

cv::Mat ImageProcessor::warpImage(const cv::Mat &image, const std::vector<cv::Point> &sudokuContour)
{
  cv::Rect boundingBox = cv::boundingRect(sudokuContour);

  std::vector<cv::Point> approx;
  cv::approxPolyDP(sudokuContour, approx, 0.02 * cv::arcLength(sudokuContour, true), true);

  if (approx.size() != 4)
    throw std::runtime_error("Sudoku contour does not have 4 points.");

  std::sort(approx.begin(), approx.end(), [](const cv::Point &a, const cv::Point &b)
            { return a.y < b.y; });

  if (approx[0].x > approx[1].x)
    std::swap(approx[0], approx[1]);
  if (approx[2].x < approx[3].x)
    std::swap(approx[2], approx[3]);

  cv::Point2f dstPts[] = {
      cv::Point2f(0, 0),
      cv::Point2f(boundingBox.width - 1, 0),
      cv::Point2f(boundingBox.width - 1, boundingBox.height - 1),
      cv::Point2f(0, boundingBox.height - 1)};

  std::vector<cv::Point2f> approx_float(approx.begin(), approx.end());
  cv::Mat transformMatrix = cv::getPerspectiveTransform(approx_float, dstPts);

  cv::Mat warped;
  cv::warpPerspective(image, warped, transformMatrix, boundingBox.size());

  return warped;
}

std::vector<std::vector<int>> ImageProcessor::extractSudokuMatrix(const cv::Mat &image)
{
  // Implement cell extraction logic
  std::vector<std::vector<int>> sudokuMatrix(9, std::vector<int>(9, -1));
  return sudokuMatrix;
}

//

cv::Mat ImageProcessor::convertToGray(const cv::Mat &image)
{
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  return gray;
}

cv::Mat ImageProcessor::applyGaussianBlur(const cv::Mat &grayImage)
{
  cv::Mat blurred;
  cv::GaussianBlur(grayImage, blurred, cv::Size(5, 5), 0);
  return blurred;
}

cv::Mat ImageProcessor::applyThreshold(const cv::Mat &grayImage)
{
  cv::Mat thresh;
  cv::adaptiveThreshold(grayImage, thresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);
  return thresh;
}

std::vector<std::vector<cv::Point>> ImageProcessor::findContours(const cv::Mat &thresh)
{
  std::vector<std::vector<cv::Point>> contours;
  cv::Mat hierarchy;
  cv::findContours(thresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
  return contours;
}

std::vector<cv::Point> ImageProcessor::findLargestContour(const std::vector<std::vector<cv::Point>> &contours)
{
  double maxArea = 0;
  std::vector<cv::Point> sudokuContour;
  for (const std::vector<cv::Point> &contour : contours)
  {
    if (contour.empty())
      continue;

    double area = cv::contourArea(contour);
    if (area > maxArea)
    {
      maxArea = area;
      sudokuContour = contour;
    }
  }

  return sudokuContour;
}
