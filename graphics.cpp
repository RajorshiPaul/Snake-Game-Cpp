/*
 * The file has been modified by Rajorshi Paul from the original file
 * created by Julie Zelenski.
 */

#include "gwindow.h"
#include <iomanip>  // for setw, setfill
#include "error.h"
#include "gthread.h"
#include "map.h"
#include "graphics.h"
using namespace std;

namespace SnakeGraphics {

static bool intializedOnce = false;
static GWindow *window;
static Map<bool, std::string> colors;

struct cellT {
    GRect *square;
    GOval *dot;
    bool marked;
};
static Grid<cellT> cells;

void initialize()
{
    window = new GWindow(1, 1);
    colors[false] = "Dark Gray";
    colors[true] = "White";
    window->setVisible(false);
    window->setWindowTitle("Snake game");
    window->setRepaintImmediately(false);
    window->setAutoRepaint(false);
    window->setExitOnClose(false);
    window->setLocation(10, 10);
    window->setResizable(false);
    intializedOnce = true;
}

void changeDimensions(int numRows, int numCols)
{
    static const int kMinWindowSize = 200, kMaxWindowSize = 1800;
    static const int kDefaultCellSize = 40;

    if (!intializedOnce) initialize();
    window->setVisible(false);
    window->clear();
    cells.clear();
    cells.resize(numRows, numCols);
    int cellSize = kDefaultCellSize;
    if (kDefaultCellSize*min(numRows, numCols) < kMinWindowSize)
        cellSize = min(kMinWindowSize/min(numRows, numCols), kMaxWindowSize/max(numRows, numCols));
    int dotSize = int(cellSize * 0.6);
    int margin = (cellSize - dotSize)/2;
    window->setCanvasSize(numCols*cellSize, numRows*cellSize);
    for (const auto& loc : cells.locations()) {
        cells[loc].square = new GRect(loc.col * cellSize, loc.row * cellSize, cellSize, cellSize);
        cells[loc].square->setVisible(false);
        cells[loc].dot = new GOval(loc.col * cellSize + margin, loc.row * cellSize + margin, dotSize , dotSize);
        cells[loc].dot->setVisible(false);
        cells[loc].marked = false;
        window->add(cells[loc].square);
        window->add(cells[loc].dot);
    }
}

void drawGrid(const Grid<bool>& g)
{
    if (g.numRows() != cells.numRows() || g.numCols() != cells.numCols()) {
        changeDimensions(g.numRows(), g.numCols());
    }
    for (const auto& loc : cells.locations()) {
        int val = g[loc];
        cells[loc].dot->setVisible(false);
        if (val) {
            cells[loc].square->setVisible(false);
       } else {
            cells[loc].square->setColor(colors[val]);
            cells[loc].square->setFillColor(colors[val]);
            cells[loc].square->setVisible(true);
        }
    }
    window->setVisible(true);
    GThread::runOnQtGuiThread([] { window->repaint(); });
//    pause(500); // wait a half-sec here
}

void highlightPath(Stack<GridLocation> path, string colorPath, GridLocation apple, string colorApple, int msecsToPause)
{
    if (cells.isEmpty()) error("MazeGraphics::highlightPath called without previous call to MazeGraphics::drawGrid");

    for (auto& c : cells) c.marked = false;
    while (!path.isEmpty()) {
        GridLocation loc = path.pop();
        if (!cells.inBounds(loc)) error("MazeGraphics asked to highlight path location: " + loc.toString() + " that is out of bounds for drawn grid.");
        cells[loc].marked = true;
    }
    for (auto& c : cells) {
        if (c.marked) {
            c.dot->setVisible(true);
            c.dot->setColor(colorPath);
            c.dot->setFillColor(colorPath);
        } else {
            c.dot->setVisible(false);
        }
    }
    cells[apple].dot->setVisible(true);
    cells[apple].dot->setColor(colorApple);
    cells[apple].dot->setFillColor(colorApple);

    GThread::runOnQtGuiThread([] {  window->repaint(); });
    pause(msecsToPause);
}

}
