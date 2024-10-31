#include <iostream>
#include <thread>
#include <vector>
#include "game.h"
#include "utils.h"

void Warships::setup() {
    playerBoard.resize(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '-'));
    computerBoard.resize(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '-'));

    std::cout << "Placing ships for the player..." << std::endl;
    placeShips(playerBoard);
    std::cout << "Placing ships for the computer..." << std::endl;
    placeShips(computerBoard);
}

void Warships::play() {
    bool gameRunning = true;

    while (gameRunning) {
        std::cout << "Player board:" << std::endl;
        printBoard(playerBoard, true);

        std::cout << "Computer board:" << std::endl;
        printBoard(computerBoard, false);

        while (true) {
            bool playerTurnContinue = playerTurn();
            if (allShipsSunk(computerBoard)) {
                std::cout << "You won!" << std::endl;
                return;
            }
            if (!playerTurnContinue) {
                break;
            }
        }

        while (true) {
            bool computerTurnContinue = computerTurn();
            if (allShipsSunk(playerBoard)) {
                std::cout << "loss" << std::endl;
                return;
            }
            if (!computerTurnContinue) {
                break;
            }
        }
    }
}

void Warships::initializeBoard(std::vector<std::vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = '-';
        }
    }
}

void Warships::printBoard(const std::vector<std::vector<char>>& board, bool reveal) {
    std::cout << "  ";
    for (int j = 0; j < BOARD_SIZE; ++j) {
        std::cout << j << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        std::cout << i << " ";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (reveal || board[i][j] == '*' || board[i][j] == '#') {
                std::cout << board[i][j] << ' ';
            }
            else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
}

void Warships::placeShips(std::vector<std::vector<char>>& board) {
    int shipSizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            int x = getRandomNumber(0, BOARD_SIZE - 1);
            int y = getRandomNumber(0, BOARD_SIZE - 1);
            int direction = getRandomNumber(0, 1);

            if (canPlaceShip(board, x, y, size, direction)) {
                for (int i = 0; i < size; ++i) {
                    if (direction == 0) {
                        board[x][y + i] = 'W';
                    }
                    else {
                        board[x + i][y] = 'W';
                    }
                }
                placed = true;
            }
        }
    }
}

bool Warships::canPlaceShip(const std::vector<std::vector<char>>& board, int x, int y, int size, int direction) {
    if (direction == 0) {
        if (y + size > BOARD_SIZE) return false;

        for (int i = -1; i <= size; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int checkX = x + j;
                int checkY = y + i;

                if (checkX >= 0 && checkX < BOARD_SIZE && checkY >= 0 && checkY < BOARD_SIZE) {
                    if (i >= 0 && i < size && j == 0) {
                        continue;
                    }
                    if (board[checkX][checkY] != '-') return false;
                }
            }
        }
    }
    else {
        if (x + size > BOARD_SIZE) return false;

        for (int i = -1; i <= size; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int checkX = x + i;
                int checkY = y + j;

                if (checkX >= 0 && checkX < BOARD_SIZE && checkY >= 0 && checkY < BOARD_SIZE) {
                    if (i >= 0 && i < size && j == 0) {
                        continue;
                    }
                    if (board[checkX][checkY] != '-') return false;
                }
            }
        }
    }
    return true;
}

bool Warships::isHit(const std::vector<std::vector<char>>& board, int x, int y) {
    return board[x][y] == 'S';
}

bool Warships::playerTurn() {
    int x, y;
    std::cout << "Enter coordinates to attack (format: x y): ";

    while (!(std::cin >> x >> y) || x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        std::cout << "Error! Try again." << std::endl;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter coordinates to attack (format: x y): ";
    }

    std::cout << "Player attacks at (" << x << ", " << y << ")" << std::endl;

    if (isHit(computerBoard, x, y)) {
        std::cout << "Hit!" << std::endl;
        computerBoard[x][y] = '#';

    }
    else {
        std::cout << "Miss!" << std::endl;
        computerBoard[x][y] = '*';
        return false;
    }
}

bool Warships::computerTurn() {
    int x = getRandomNumber(0, BOARD_SIZE - 1);
    int y = getRandomNumber(0, BOARD_SIZE - 1);
    std::cout << "Comp attacks at (" << x << ", " << y << ")" << std::endl;

    if (isHit(playerBoard, x, y)) {
        std::cout << "Comp hit!" << std::endl;
        playerBoard[x][y] = '#';
    }
    else {
        std::cout << "Comp missed!" << std::endl;
        playerBoard[x][y] = '*';
        return false;
    }
}

bool Warships::allShipsSunk(const std::vector<std::vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == 'W') {
                return false;
            }
        }
    }
    return true;
}
