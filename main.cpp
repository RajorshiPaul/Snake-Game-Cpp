/*
 * The file has been created solely by Rajorshi Paul. No external
 * assistance or collaboration was used during the creation of this
 * file.
 */

#include <iostream>
#include "console.h"
#include "gridlocation.h"
#include "stack.h"
#include "snake.h"
#include "algorithms.h"
#include "testing/SimpleTest.h"
using namespace std;

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    //Snake Game
    Snake snake;
    snake.boardSize = 10;
    snake.initialize();
    GridLocation appleLocation = snake.getApple();

    int score = 0;

    Stack<GridLocation> path = snake.body;

    while (!path.isEmpty()){
        //BFS
        path = bfs(snake, appleLocation);
        //Recursion
//        path = recursion(snake, appleLocation);
        snake.graphicsss(path, appleLocation);
        appleLocation = snake.getApple();
        score ++;
    }
    cout << "The final score is " << score << endl;
    cout << "All done, exiting" << endl;
    return 0;
}

