#include <iostream>
#include <map>

using namespace std;

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

const int width = 7;
const int maxRotationIndex = (width - 1) / 2;
Color board[width][width] = {};

void rotatePuck(bool doRotateRight, int x, int y) {
    const int numCoords = 4;
    int xCoords[numCoords] = { x, y, width - 1 - x, width - 1 - y };
    int yCoords[numCoords] = { y, width - 1 - x, width - 1 - y, x };

    int startIndex = doRotateRight ? 3 : 0;
    int endIndex = doRotateRight ? 0 : 3;
    int increment = doRotateRight ? -1 : 1;

    Color savedPuck = board[xCoords[startIndex]][yCoords[startIndex]];
    int fromCoord;
    for (int i = startIndex; i != endIndex; i += increment) {
        fromCoord = (i + increment) % numCoords;
        board[xCoords[i]][yCoords[i]] = board[xCoords[fromCoord]][yCoords[fromCoord]];
    }
    board[xCoords[fromCoord]][yCoords[fromCoord]] = savedPuck;
}

void rotateBoard(bool doRotateRight) {
    // one must be < and one must be <= to avoid overlap
    for (int x = 0; x < maxRotationIndex; x++) {
        for (int y = 0; y <= maxRotationIndex; y++) {
            rotatePuck(doRotateRight, x, y);
        }
    }
}

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

bool inBounds(int x, int y) {
    return 0 <= x && x < width && 0 <= y && y < width;
}

void dropPuck(Color color, int x, int y) {
    assert(inBounds(x, y));

    while (inBounds(x, y)) {
        if (!inBounds(x + 1, y)) {
            break;
        }
        if (board[x + 1][y] == none) {
            x++;
        } else {
            break;
        }
    }

    cout << "dropping " + names.at(color) + " puck at " << x << "," << y << "\n";
    board[x][y] = color;
    cout << board[x][y] << " has been dropped!\n";
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
    cout << "Type a number to drop a " + names.at(color) + " puck, type 'left' to rotate left, or type 'right' to rotate right\n";
    string action;
    cin >> action;
    if (action == "left") {
        cout << "Rotating the board to the left\n";
        rotateBoard(false);
    } else if (action == "right") {
        cout << "Rotating the board to the right\n";
        rotateBoard(true);
    } else {
        int y = stoi(action);
        dropPuck(color, 0, y);
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
