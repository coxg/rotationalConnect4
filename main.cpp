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
    // left and right rotation follow the same pattern, just in different directions
    // x and y coordinates also follow the same pattern, but y has an offset of 1 - this is why it's length 5 despite
    // there only being 4 positions represented
    int coords[5] = { x, y, width - 1 - x, width - 1 - y, x };

    int startIndex = doRotateRight ? 3 : 0;
    int endIndex = doRotateRight ? 0 : 3;
    int increment = doRotateRight ? -1 : 1;

    Color tmp = board[coords[startIndex]][coords[startIndex + 1]];
    int fromCoord;
    for (int i = startIndex; i != endIndex; i += increment) {
        fromCoord = (i + increment) % 4;
        board[coords[i]][coords[i + 1]] = board[coords[fromCoord]][coords[fromCoord + 1]];
    }
    board[coords[fromCoord]][coords[fromCoord + 1]] = tmp;
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

void applyGravity(int x, int y) {
    Color color = board[x][y];
    if (color == none) {
        return;
    }

    board[x][y] = none;
    while (x + 1 < width) {
        if (board[x + 1][y] == none) {
            x++;
        } else {
            break;
        }
    }
    board[x][y] = color;
}

void dropPuck(Color color, int x, int y) {
    assert(board[x][y] == none);
    board[x][y] = color;
    applyGravity(x, y);
}

void rotateBoard(bool doRotateRight) {
    // one must be < and one must be <= to avoid overlap
    for (int x = 0; x < maxRotationIndex; x++) {
        for (int y = 0; y <= maxRotationIndex; y++) {
            rotatePuck(doRotateRight, x, y);
        }
    }
    // must apply gravity only after all pucks have rotated, and from bottom to top
    for (int x = width - 1; x >= 0; x--) {
        for (int y = 0; y < width; y++) {
            applyGravity(x, y);
        }
    }
}

void checkForWinner() {
    // TODO
    // check horizontal
    // check vertical
    // check up/right diagonal
    // check down/right diagonal
}

void doAction(Color color) {
    cout << "Type a number to drop a " + names.at(color) + " puck, type 'left' to rotate left, or type 'right' to rotate right\n";
    string action;
    cin >> action;
    if (action == "left") {
        rotateBoard(false);
    } else if (action == "right") {
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
        checkForWinner();
        curColor = curColor == red ? black : red;
    }
}
