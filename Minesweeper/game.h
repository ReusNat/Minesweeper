#pragma once
#include <map>
#include <vector>
#include <print>
#include <utility>
#include <format>
#include <algorithm>
#include <random>
#include <ctime>

class Game {
private:
	struct Space {
		bool is_bomb = false;
		bool is_revealed = false;
		bool is_flagged = false;
		bool counted = false;
		int num_bombs = 0;
	};

	std::map<std::string, char> spaces 
	{
		{"unchecked", '?'},
		{"clear", '#'},
		{"bomb", 'X'},
		{"flag", 'F'}
	};

	int rows = 10;
	int columns = 10;
	int num_bombs = 10;
	int num_flagged = 0;
	std::vector<std::vector<Space>> board;

public:	
	Game() {
		this->board = std::vector(rows, std::vector<Space>(columns, Space(false, false, false, false, 0)));
	}

	void run() {
		int row;
		int col;
		char action;
		std::vector<std::pair<int, int>> compass;
		setBombs();
		
		while (!finished()) {
			printBoard();
			row = getInt(true);
			col = getInt(false);
			std::print("Action: ");
			std::cin >> action;

			compass = {
				std::pair<int, int>(row - 1, col),
				std::pair<int, int>(row + 1, col),
				std::pair<int, int>(row, col + 1),
				std::pair<int, int>(row, col - 1),
				std::pair<int, int>(row + 1, col - 1),
				std::pair<int, int>(row + 1, col + 1),
				std::pair<int, int>(row - 1, col - 1),
				std::pair<int, int>(row - 1, col + 1),
			};

			switch (action) {
				case 'c':
					if (!board[row][col].is_revealed) {
						if (board[row][col].is_bomb) {
							std::println("GAME OVER");
							return;
						}
						
						board[row][col].is_revealed = true;
						int _num_bombs = countBombs(row, col);
						board[row][col].num_bombs = _num_bombs;
						if (_num_bombs == 0) {
							for (auto cords : compass) {
								if ((cords.first >= 0 && cords.first <= 9) && (cords.second >= 0 && cords.second <= 9)) {
									_num_bombs = countBombs(cords.first, cords.second);
									if (!board[cords.first][cords.second].is_bomb || !board[cords.first][cords.second].is_flagged) {
										board[cords.first][cords.second].is_revealed = true;
										board[cords.first][cords.second].num_bombs = _num_bombs;
									}
								}
							}
						}
					}
					break;
				case 'f':
					setSpace(row, col, "flag");
					break;
			}
		}
		printBoard();

	}

	const void printBoard() {
		int row_num = 0;
		std::println(" |0|1|2|3|4|5|6|7|8|9|");
		for (auto& row : this->board) {
			std::print("{}|", row_num++);
			for (Space _space : row) {
				if (_space.is_revealed) {
					if (_space.num_bombs == 0)
						std::print("{}|", spaces["clear"]);
					else
						std::print("{}|", _space.num_bombs);
				}
				else if (_space.is_flagged)
					std::print("{}|", spaces["flag"]);
				else
					std::print("{}|", spaces["unchecked"]);
			}
			std::println();
		}
	}


private:
	// gets input from the player
	// input must be an int to succeed
	int getInt(bool isRow) {
		int ret;

		while (true) {
			if (isRow)
				std::print("Enter row number: ");
			else
				std::print("Enter column number: ");

			if (std::cin >> ret) {
				return ret;
			} 
			else {
				std::println("Invalid input, try again.");
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
	}

	bool finished() {
		for (auto& row : board) {
			for (Space _space : row) {
				if (_space.is_flagged && !_space.is_bomb) {
					return false;
				}
			}
		}

		if (num_flagged == num_bombs)
			return true;

		return false;
	}

	void setBombs() {
		std::vector<int> positions(100);
		for (int i = 0; i < 100; ++i) positions[i] = i;

		// Shuffle positions
		std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
		std::shuffle(positions.begin(), positions.end(), rng);

		// Set bombs
		for (int i = 0; i < num_bombs; ++i) {
			int pos = positions[i];
			int row = pos / 10;
			int col = pos % 10;
			board[row][col].is_bomb = true;
		}
	}

	void setSpace(int row, int col, std::string type) {
		if (type == "flag") {
			if (board[row][col].is_flagged) {
				type = "unchecked";
				num_flagged--;
				board[row][col].is_flagged = false;
			}
			else {
				num_flagged++;
				board[row][col].is_flagged = true;
			}
		}
	}

	int countBombs(int row, int col) {
		if (!board[row][col].counted) {
			int count = 0;

			std::vector<std::pair<int, int>> compass{
				std::pair<int, int>(row - 1, col),
				std::pair<int, int>(row + 1, col),
				std::pair<int, int>(row, col + 1),
				std::pair<int, int>(row, col - 1),
				std::pair<int, int>(row + 1, col - 1),
				std::pair<int, int>(row + 1, col + 1),
				std::pair<int, int>(row - 1, col - 1),
				std::pair<int, int>(row - 1, col + 1),
			};

			for (std::pair<int, int> cords : compass) {
				if ((cords.first >= 0 && cords.first <= 9) && (cords.second >= 0 && cords.second <= 9))
					if (board[cords.first][cords.second].is_bomb) count++;
			}

			return count;
		}
		return board[row][col].num_bombs;
	}
};


