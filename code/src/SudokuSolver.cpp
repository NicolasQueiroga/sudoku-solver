#include "SudokuSolver.hpp"

SudokuSolver::SudokuSolver()
    : imageProcessor_(std::make_unique<ImageProcessor>()), sudokuBoard_(std::make_unique<SudokuBoard>()) {}

void SudokuSolver::processImage(const cv::Mat &image)
{
  cv::Mat processedImage = imageProcessor_->preprocessImage(image);
  // std::vector<cv::Point> corners = imageProcessor_->detectBoard(processedImage);
  // cv::Mat warpedImage = imageProcessor_->warpImage(processedImage, corners);
  // std::vector<std::vector<int>> matrix = imageProcessor_->extractSudokuMatrix(warpedImage);
  // sudokuBoard_->setBoard(matrix);
}

void SudokuSolver::displayBoard() const
{
  sudokuBoard_->printBoard();
}
