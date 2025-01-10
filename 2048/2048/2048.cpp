#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>

using namespace std;

class Board {
private:
    vector<vector<int>> grid;
    int size;

    void addRandomTile() {
        vector<pair<int, int>> emptyCells;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i][j] == 0) {
                    emptyCells.push_back({ i, j });
                }
            }
        }
        if (!emptyCells.empty()) {
            int randomIndex = rand() % emptyCells.size();
            int i = emptyCells[randomIndex].first;
            int j = emptyCells[randomIndex].second;
            grid[i][j] = (rand() % 2 + 1) * 2; // Add 2 or 4 randomly
        }
    }

    bool canMerge() const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (j + 1 < size && grid[i][j] == grid[i][j + 1]) return true;
                if (i + 1 < size && grid[i][j] == grid[i + 1][j]) return true;
            }
        }
        return false;
    }

public:
    Board(int s) : size(s), grid(s, vector<int>(s, 0)) {
        addRandomTile();
        addRandomTile();
    }

    void display() const {
        cout << "---------------------" << endl;
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                cout << setw(5) << (cell ? to_string(cell) : ".");
            }
            cout << endl;
        }
        cout << "---------------------" << endl;
    }

    bool move(char direction) {
        vector<vector<int>> oldGrid = grid;

        auto slide = [](vector<int>& row) {
            vector<int> newRow(row.size(), 0);
            int index = 0;
            for (int num : row) {
                if (num != 0) {
                    newRow[index++] = num;
                }
            }
            return newRow;
            };

        auto merge = [](vector<int>& row) {
            for (int i = 0; i < row.size() - 1; i++) {
                if (row[i] == row[i + 1] && row[i] != 0) {
                    row[i] *= 2;
                    row[i + 1] = 0;
                }
            }
            };

        for (int i = 0; i < size; i++) {
            vector<int> row;
            switch (direction) {
            case 'w': // Up
                for (int j = 0; j < size; j++) row.push_back(grid[j][i]);
                row = slide(row);
                merge(row);
                row = slide(row);
                for (int j = 0; j < size; j++) grid[j][i] = row[j];
                break;

            case 's': // Down
                for (int j = size - 1; j >= 0; j--) row.push_back(grid[j][i]);
                row = slide(row);
                merge(row);
                row = slide(row);
                for (int j = size - 1, k = 0; j >= 0; j--, k++) grid[j][i] = row[k];
                break;

            case 'a': // Left
                row = grid[i];
                row = slide(row);
                merge(row);
                row = slide(row);
                grid[i] = row;
                break;

            case 'd': // Right
                row.assign(grid[i].rbegin(), grid[i].rend());
                row = slide(row);
                merge(row);
                row = slide(row);
                for (int j = 0; j < size; j++) grid[i][size - 1 - j] = row[j];
                break;

            default:
                break;
            }
        }

        if (oldGrid != grid) {
            addRandomTile();
            return true;
        }
        return false;
    }

    bool isGameOver() const {
        for (const auto& row : grid) {
            if (find(row.begin(), row.end(), 0) != row.end()) return false;
        }
        return !canMerge();
    }
};

class Game {
private:
    Board board;

public:
    Game(int size) : board(size) {}

    void start() {
        char move;
        cout << "Welcome to 2048! Use 'w', 'a', 's', 'd' to move, and 'q' to quit." << endl;
        while (true) {
            board.display();
            if (board.isGameOver()) {
                cout << "Game Over!" << endl;
                break;
            }
            cout << "Enter your move: ";
            cin >> move;
            if (move == 'q') {
                cout << "Thanks for playing!" << endl;
                break;
            }
            if (!board.move(move)) {
                cout << "Invalid move! Try again." << endl;
            }
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));
    Game game(4);
    game.start();
    return 0;
}
