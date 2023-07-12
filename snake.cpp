/*
 * The file has been created solely by Rajorshi Paul. No external
 * assistance or collaboration was used during the creation of this
 * file.
 */

#include "snake.h"
#include "stack.h"
#include "gridlocation.h"
#include "grid.h"
#include "console.h"
#include "graphics.h"
#include "testing/SimpleTest.h"
using namespace std;
using namespace SnakeGraphics;

Snake::Snake(){

}

/*
 * This method returns the coordinate of the head of the snake by using
 * the peek() method on the body attribute.
 */
GridLocation Snake::head(){
    if (body.isEmpty()){
        error("body attribute not initialized!");
    }
    return body.peek();
}

/*
 * This method returns the coordinate of the tail of the snake by copying
 * the body attribute to a temporary variable and popping the elements from
 * the top until the bottom of the stack is reached.
 */
GridLocation Snake::tail(){
    if (body.isEmpty()){
        error("body attribute not initialized!");
    }
    Stack<GridLocation> temp = body;
    GridLocation element;
    while (!temp.isEmpty()){
        element = temp.pop();
    }
    return element;
}

/*
 * This method takes a given coordinate as an argument and adds it to the head
 * of the snake using the push() method on the body attribute.
 */
void Snake::addHead(GridLocation loc){
    if (body.isEmpty()){
        return body.push(loc);
    }
    Vector<GridLocation> neighbors = validNeighbors(head());
    for (GridLocation neighbor : neighbors){
        if(neighbor == loc){
            return body.push(loc);
        }
    }
    error("Cannot add to head!");
}

/*
 * This method deletes the tail of the snake by popping the elements of the body
 * and pushing them into a temporary stack variable. The elements from the temporary
 * variable without the tail are then pushed back into the body attribute.
 */
void Snake::deleteTail(){
    if (body.isEmpty()){
        error("body attribute is not initialized!");
    }
    Stack<GridLocation> temp;

    while(body.size() > 1){
        GridLocation element = body.pop();
        temp.push(element);
    }
    body.pop();
    while (!temp.isEmpty()){
        GridLocation element = temp.pop();
        body.push(element);
    }
}

/*
 * This method generates the coordinate of an apple at a random location
 * on the grid. We make sure that the apple does not coincide with the
 * snake by using the contains method.
 */
GridLocation Snake::getApple(){
    GridLocation appleLocation;
    appleLocation.row = randomInteger(0, boardSize - 1);
    appleLocation.col = randomInteger(0, boardSize - 1);
    while (contains(appleLocation)){
        appleLocation.row = randomInteger(0, boardSize - 1);
        appleLocation.col = randomInteger(0, boardSize - 1);
    }
    return appleLocation;
}

/*
 * This method checks if the given coordinate loc is contained inside the
 * snake.
 */
bool Snake::contains(GridLocation loc){
    Stack<GridLocation> temp = body;
    while (!temp.isEmpty()){
        if(temp.pop() == loc){
            return true;
        }
    }
    return false;
}

/*
 * This method creates a snake of length 2 at a random location on the grid.
 * The orientation of the snake is also randomly chosen. The body attribute
 * is updated with the coordinates.
 */
void Snake::initialize(){
    GridLocation point;
    point.row = randomInteger(0, boardSize - 1);
    point.col = randomInteger(0, boardSize - 1);
    body.push(point);
    Vector<GridLocation> neighbors = validNeighbors(point);
    int idx = randomInteger(0, neighbors.size() - 1);
    body.push(neighbors[idx]);
}

/*
 * This method takes a given coordinate and moves the snake to the coordinate.
 * The given coordinate must be a valid neighbor of the head of the snake. To
 * move the snake, we add the coordinate to the add of the snake by using the
 * addhead method and delete the tail using the deleteTail method.
 */
void Snake::move(GridLocation loc){
    if (body.isEmpty()){
        error("body attribute is not initialized!");
    }
    Vector<GridLocation> neighbors = validNeighbors(head());
    for (GridLocation neighbor : neighbors){
        if(neighbor == loc){
            addHead(loc);
            deleteTail();
            return;
        }
    }
    error("Invalid destination!");
}

/*
 * This method takes a path and moves the snake along the path. The valid path
 * starts at the head of the snake and ends at the destination which is at the
 * top of the stack. First, we reverse the stack using the reversePath method.
 * The method throws an error if the path does not start at the head of the snake.
 * Next, we go through the stack and use the move method (above) to move the snake
 * one coordinate at a time until we empty the stack.
 */
void Snake::move(Stack<GridLocation> path){
    if (body.isEmpty()){
        error("body attribute is not initialized!");
    }
    reversePath(path);
    if (head() != path.pop()){
        error("Invalid path!");
    }
    if (path.size() == 0){
        return;
    }
    int n = path.size();

    for (int i = 0; i < n; i ++){
        GridLocation loc = path.pop();
        move(loc);
    }
}

/*
 * This method takes the path of the snake and the coordinate of the apple
 * and generates the graphic of the snake following the path on the grid.
 * This method uses functions from graphics.h. First, we create a grid to
 * represent the grid for the game using the drawGrid function. Then we
 * iterate through the coordinates of the path stack (GridLocation). For
 * each coordinate, we use the move method to move the snake by one step.
 * We represent the snake and the apple using the highlightPath function.
 * Note, we used the reversePath function to reverse the stack path before
 * iterating through the elements.Finally, we increase the size of the snake
 * by 1 and update the body attribute.
 */
void Snake::graphicsss(Stack<GridLocation> path, GridLocation appleLocation){
    reversePath(path);
    Grid<bool> board(boardSize, boardSize, true);
    Stack<GridLocation> apple;
    drawGrid(board);
    if (path.size() > 0){
        path.pop();
    }
    while (path.size() > 1){
        apple = {appleLocation};
        move(path.pop());

        highlightPath(body, "green", appleLocation, "red", 200);
    }
    if (path.size() > 0){
        addHead(path.pop());
    }
}

/*
 * This method takes a given coordinate as argument and returns a vector with
 * all of its valid neighbors. This is done by checking if each of the four
 * possible neighbors lie within the grid and are not contained by the body
 * of the snake.
 */
Vector<GridLocation> Snake:: validNeighbors(GridLocation loc){
    Vector<GridLocation> neighbors;
    GridLocation north(loc.row -1, loc.col);
    GridLocation south(loc.row +1, loc.col);
    GridLocation east(loc.row, loc.col + 1);
    GridLocation west(loc.row, loc.col - 1);

    if (north.row >= 0){
        neighbors.add(north);
    }
    if (south.row < boardSize){
        neighbors.add(south);
    }

    if (west.col >= 0){
        neighbors.add(west);
    }
    if (east.col < boardSize){
        neighbors.add(east);
    }

    return neighbors;
}

/*
 * This method takes a Stack path by reference and reverses it.
 */
void Snake::reversePath(Stack<GridLocation>& path){
    Stack<GridLocation> temp;
    while(!path.isEmpty()){
        temp.push(path.pop());
    }
    path = temp;
}

PROVIDED_TEST("Test head()"){
    Snake snake;
    snake.boardSize = 5;
    EXPECT_ERROR(snake.head());
    snake.body = {{0, 2}, {0, 3}, {0, 4}};
    EXPECT_EQUAL(snake.head(), {0, 4});
}

PROVIDED_TEST("Test tail()"){
    Snake snake;
    snake.boardSize = 5;
    EXPECT_ERROR(snake.tail());
    snake.body = {{0, 2}};
    EXPECT_EQUAL(snake.tail(), {0, 2});
    snake.body = {{0, 2}, {0, 3}, {0, 4}};
    EXPECT_EQUAL(snake.tail(), {0, 2});
}

PROVIDED_TEST("Test addHead()"){
    Snake snake;
    snake.boardSize = 5;
    snake.addHead({0, 4});
    EXPECT_EQUAL(snake.head(), {0, 4});
    snake.body = {{0, 2}, {0, 3}};
    snake.addHead({0, 4});
    EXPECT_EQUAL(snake.head(), {0, 4});
    snake.body = {{0, 2}, {0, 3}};
    EXPECT_ERROR(snake.addHead({0, 5}));
}

PROVIDED_TEST("Test deleteTail()"){
    Snake snake;
    snake.boardSize = 5;
    EXPECT_ERROR(snake.deleteTail());
    snake.body = {{0, 2}};
    snake.deleteTail();
    EXPECT_EQUAL(snake.body, {});
    snake.body = {{0, 2}, {0, 3}, {0, 4}};
    snake.deleteTail();
    EXPECT_EQUAL(snake.body, {{0, 3}, {0, 4}});
}

PROVIDED_TEST("Test contains()"){
    Snake snake;
    snake.boardSize = 5;
    EXPECT_EQUAL(snake.contains({0, 0}), false);

    snake.body = {{0, 2}, {0, 3}, {0, 4}};
    EXPECT_EQUAL(snake.contains({0, 0}), false);
    EXPECT_EQUAL(snake.contains({0, 3}), true);
    EXPECT_EQUAL(snake.contains({0, 4}), true);
    EXPECT_EQUAL(snake.contains({0, 2}), true);
}

PROVIDED_TEST("Test initialize()"){
    Snake snake;
    snake.boardSize = 5;
    snake.initialize();
    EXPECT_EQUAL(snake.body.isEmpty(), false);
}

PROVIDED_TEST("Test move(loc)"){
    Snake snake;
    snake.boardSize = 5;

    EXPECT_ERROR(snake.move({0, 2}));
    snake.body = {{0, 2}, {0, 3}, {0, 4}};
    EXPECT_ERROR(snake.move({0, 1}));
    cout << snake.body << endl;
    snake.move({1, 4});
    EXPECT_EQUAL(snake.body, {{0, 3}, {0, 4}, {1, 4}});
}

PROVIDED_TEST("Test move(path)"){
    Snake snake;
    snake.boardSize = 5;
    EXPECT_ERROR(snake.move({{0, 2}}));
    Stack<GridLocation> path = {{1, 4}, {1, 3}, {2, 3}};
    snake.body = {{0, 2}, {0, 3}, {0, 4}};
    EXPECT_ERROR(snake.move(path));
    path = {{0, 4}, {1, 4}, {1, 3}, {2, 3}};
    snake.move(path);
    EXPECT_EQUAL(snake.body, {{1, 4}, {1, 3}, {2, 3}});
}



