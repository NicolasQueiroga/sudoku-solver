#pragma once

#include <vector>
#include <memory>

class SudokuBoard
{
public:
  SudokuBoard();
  void setBoard(const std::vector<std::vector<int>> &board);
  const std::vector<std::vector<int>> &getBoard() const;
  void printBoard() const;

private:
  std::vector<std::vector<int>> board_;
};
