/*
 * The file has been created solely by Rajorshi Paul. No external
 * assistance or collaboration was used during the creation of this
 * file.
 */

#pragma once

#include "stack.h"
#include "gridlocation.h"

class Snake
{
public:
    /*
     * Creates a newistance of the Snake class.
     */
    Snake();

    /*
     * This attribute stores the coordinates of the snake as a stack with
     * the head of the snake being on top of the stack.
     */
    Stack<GridLocation> body;

    /*
     * This attribute stores the size of the square grid. This attribute
     * must be initialized before calling the methods.
     */
    int boardSize;

    /*
     * This method returns the coordinates of the head of the snake.
     */
    GridLocation head();

    /*
     * This method returns the coordinates of the tail of the snake.
     */
    GridLocation tail();

    /*
     * This method returns the coordinates of the apple. The coordinates are
     * randomly allocated.
     */
    GridLocation getApple();

    /*
     * This method checks if the body of the snake contains a given coordinates.
     */
    bool contains(GridLocation loc);

    /*
     * This method randomly generates a snake of length 2 and updates the body
     * attribute.
     */
    void initialize();

    /*
     * This method adds a given coordinate to the head of the snake.
     */
    void addHead(GridLocation head);

    /*
     * This method removes the coordinate from the tail of the snake.
     */
    void deleteTail();

    /*
     * This method moves the snake by one coordinate specified by the given
     * coordinate and updates the body attribute.
     */
    void move(GridLocation loc);

    /*
     * This method moves the snake along a given path and updates the body
     * attribute. The path is a stack containing the destination coordinate
     * at the top and the head of the snake at the bottom. The method throws
     * an error for an invalid path.
     */
    void move(Stack<GridLocation> path);

    /*
     * The method takes the path of the snake and the coordinate of the apple
     * and generates the graphic of the snake following the path on the grid.
     * The method increases the size of the snake by 1 and updates the body
     * attribute.
     */
    void graphicsss(Stack<GridLocation> path, GridLocation appleLocation);

private:

    Vector<GridLocation> validNeighbors(GridLocation loc);
    void reversePath(Stack<GridLocation>& path);
};

