#pragma once

#include "ImageProcessor.hpp"
#include "SudokuBoard.hpp"
#include <memory>

class SudokuSolver
{
public:
  SudokuSolver();
  void processImage(const cv::Mat &image);
  void displayBoard() const;

private:
  std::unique_ptr<ImageProcessor> imageProcessor_;
  std::unique_ptr<SudokuBoard> sudokuBoard_;
};
