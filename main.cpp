#include <iostream>

using namespace std;

const int width = 7;

class Color {
public:
    string name;
    string repr;

    bool operator==(const Color& rhs) const {
        return name == rhs.name;
    }
};

Color* board[width][width] = {nullptr};

void printColNumbers() {
    for (int i = 0; i < width; i++)
    {
        cout << " " + to_string(i) + " ";
    }
    cout << "\n";
}

void printBoard() {
    for (auto & i : board)
    {
        for (auto & j : i)
        {
            if (j == nullptr) {
                cout << "   ";
            } else {
                cout << " " + j->repr + " ";
            }
        }
        cout << "\n";
    }
    printColNumbers();
}

void dropPuck(Color &color, int col) {
    int row = 0;
    assert(col >= 0);
    assert(col < width);
    assert(board[row][col] == nullptr);

    while (row + 1 < width) {
        if (board[row + 1][col] == nullptr) {
            row++;
        } else {
            break;
        }
    }

    cout << "dropping " + color.name + " puck at " << row << "," << col;
    board[row][col] = &color;
}

optional<Color> getWinner() {
    // TODO
    // check horizontal
    // check vertical
    // check up/right diagonal
    // check down/right diagonal

    return nullopt;
}

void doAction(Color &color) {
    cout << "Type a number to drop a " + color.repr + "puck, type 'left' to rotate left, or type 'right' to rotate right\n";
    string action;
    cin >> action;
    if (action == "left") {
        cout << "Rotating the board to the left";
        // TODO
    } else if (action == "right") {
        cout << "Rotating the board to the right";
        // TODO
    } else {
        int col = stoi(action);
        dropPuck(color, col);
    }
}

int main()
{
    Color red;
    red.name = "red";
    red.repr = "r";

    Color black;
    black.name = "black";
    black.repr = "b";

    bool isRed = true;
    while (true) {
        printBoard();
        doAction(isRed ? red : black);
        auto winner = getWinner();
        if (winner.has_value()) {
            cout << winner->name + " wins!";
            break;
        }
        isRed = !isRed;
    }
}
