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

const int WIDTH = 7;
const int CONTINUOUS_PUCKS_TO_WIN = 4;
const int MAX_ROTATION_INDEX = (WIDTH - 1) / 2;
Color board[WIDTH][WIDTH] = {};

void rotatePuck(bool doRotateRight, int x, int y) {
    // left and right rotation follow the same pattern, just in different directions
    // x and y coordinates also follow the same pattern, but y has an offset of 1 - this is why it's length 5 despite
    // there only being 4 positions represented
    int coords[5] = { x, y, WIDTH - 1 - x, WIDTH - 1 - y, x };

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
    for (int i = 0; i < WIDTH; i++)
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
    while (x + 1 < WIDTH) {
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
    for (int x = 0; x < MAX_ROTATION_INDEX; x++) {
        for (int y = 0; y <= MAX_ROTATION_INDEX; y++) {
            rotatePuck(doRotateRight, x, y);
        }
    }
    // must apply gravity only after all pucks have rotated, and from bottom to top
    for (int x = WIDTH - 1; x >= 0; x--) {
        for (int y = 0; y < WIDTH; y++) {
            applyGravity(x, y);
        }
    }
}

bool isInBounds(int x, int y) {
    return 0 <= x && x < WIDTH && 0 <= y && y < WIDTH;
}

int getWinnerOnLine(int x, int y, int xIncrement, int yIncrement) {
    Color trackingColor = none;
    int continuousColorCount = 0;
    while (isInBounds(x, y)) {
        Color color = board[x][y];
        if (color == trackingColor && color != none) {
            continuousColorCount++;
            if (continuousColorCount == CONTINUOUS_PUCKS_TO_WIN) {
                return color;
            }
        } else {
            trackingColor = color;
            continuousColorCount = 1;
        }
        x += xIncrement;
        y += yIncrement;
    }
    return none;
}

int getLinearWinners() {
    int winners = 0;
    for (int i = 0; i < WIDTH; i++) {
        // check vertical
        winners |= getWinnerOnLine(i, 0, 0, 1);
        // check horizontal
        winners |= getWinnerOnLine(0, i, 1, 0);
    }
    return winners;
}

int getDiagonalWinners() {
    int winners = 0;
    for (int i = 0; i < WIDTH; i++) {
        // check up/right along x-axis
        winners |= getWinnerOnLine(i, 0, 1, 1);
        // check up/left along x-axis
        winners |= getWinnerOnLine(i, 0, -1, 1);
        // check down/right along top
        winners |= getWinnerOnLine(i, WIDTH - 1, 1, -1);
        // check down/left along top
        winners |= getWinnerOnLine(i, WIDTH - 1, -1, -1);
    }
    return winners;
}

int getWinners() {
    int winners = getLinearWinners();
    winners |= getDiagonalWinners();
    return winners;
}

void declareWinners(int winners) {
    bool redWins = (winners & red) == winners;
    bool blackWins = (winners & black) == winners;
    assert(redWins || blackWins);
    if (redWins && blackWins) {
        cout << "Both teams win!\n";
    } else if (redWins) {
        cout << "Red wins!\n";
    } else {
        cout << "Black wins!\n";
    }
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
    int winners = 0;
    printBoard();
    while (!winners) {
        doAction(curColor);
        curColor = curColor == red ? black : red;
        winners = getWinners();
        printBoard();
    }
    declareWinners(winners);
}
