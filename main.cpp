#include <iostream>
#include <map>

using namespace std;

const int width = 7;

enum Color { none, red, black };

map<Color, string> names = {
        { red, "red"},
        { black, "black"}
};

map<Color, string> reprs = {
        { none, " " },
        { red, "r"},
        { black, "b"}
};

Color board[width][width] = {};

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
            cout << " " + reprs.at(j) + " ";
        }
        cout << "\n";
    }
    printColNumbers();
}

void dropPuck(Color color, int col) {
    int row = 0;
    assert(col >= 0);
    assert(col < width);
    assert(board[row][col] == none);

    while (row + 1 < width) {
        if (board[row + 1][col] == none) {
            row++;
        } else {
            break;
        }
    }

    cout << "dropping " + names.at(color) + " puck at " << row << "," << col;
    board[row][col] = color;
}

optional<Color> getWinner() {
    // TODO
    // check horizontal
    // check vertical
    // check up/right diagonal
    // check down/right diagonal

    return nullopt;
}

void doAction(Color color) {
    cout << "Type a number to drop a " + names.at(color) + "puck, type 'left' to rotate left, or type 'right' to rotate right\n";
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
    Color curColor = red;
    while (true) {
        printBoard();
        doAction(curColor);
        auto winner = getWinner();
        if (winner.has_value()) {
            cout << names.at(winner.value()) + " wins!";
            break;
        }
        curColor = curColor == red ? black : red;
    }
}
