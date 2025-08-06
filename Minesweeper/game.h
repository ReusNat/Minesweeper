#pragma once
#include <vector>
#include <print>

namespace Minesweeper {
	class Board {
	private:
		int rows = 0;
		int columns = 0;
		std::vector<std::vector<char>> board;

	public:	
		Board(int _height, int _width) {
			this->rows = _height;
			this->columns = _width;
			this->board = std::vector(rows, std::vector<char>(columns, '0'));
		}

		void printBoard() {
			for (auto row : this->board) {
				std::print("|");
				for (auto value : row) {
					std::print("{}|", value);
				}
				std::println();
			}
		}

		std::vector<std::vector<char>> getBoard() {
			return this->board;
		}
	};

}
