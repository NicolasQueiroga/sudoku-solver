#include "SudokuBoard.hpp"
#include <iostream>

SudokuBoard::SudokuBoard() : board_(9, std::vector<int>(9, -1)) {}

void SudokuBoard::setBoard(const std::vector<std::vector<int>> &board)
{
    board_ = board;
}

const std::vector<std::vector<int>> &SudokuBoard::getBoard() const
{
    return board_;
}

void SudokuBoard::printBoard() const
{
    for (const auto &row : board_)
    {
        for (int cell : row)
            std::cout << (cell == -1 ? "." : std::to_string(cell)) << " ";
        std::cout << std::endl;
    }
}
