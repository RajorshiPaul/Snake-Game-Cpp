/*
 * The file has been created solely by Rajorshi Paul. No external
 * assistance or collaboration was used during the creation of this
 * file.
 */

#include <iostream>
#include "vector.h"
#include "stack.h"
#include "queue.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "snake.h"
#include "stack.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * This function takes a Snake object and returns a vector containing
 * all valid neighbors of the head of the snake. This is done by checking
 * if each of the four possible neighbors lie within the grid and are
 * not contained by the body of the snake.
 */
Vector<GridLocation> findValidNeighbors(Snake snake) {
    if(snake.body.isEmpty()){
        error("body attribute is not initialized!");
    }
    Vector<GridLocation> neighbors;
    GridLocation cur = snake.head();
    GridLocation north(cur.row -1, cur.col);
    GridLocation south(cur.row +1, cur.col);
    GridLocation east(cur.row, cur.col + 1);
    GridLocation west(cur.row, cur.col - 1);

    if (north.row >= 0){
        if (!snake.contains(north)){
            neighbors.add(north);
        }
    }

    if (south.row < snake.boardSize){
        if (!snake.contains(south)){
            neighbors.add(south);
        }
    }

    if (east.col < snake.boardSize){
        if (!snake.contains(east)){
            neighbors.add(east);
        }
    }

    if (west.col >= 0){
        if (!snake.contains(west)){
            neighbors.add(west);
        }
    }
    return neighbors;
}


/*
 * This function takes a Snake object snake and the coordinates of the apple as
 * the argument and uses BFS to determine the shortest path from the head of the
 * snake to the apple. First, the relevant variables are initialized. Next, we
 * apply the BFS algorithm. We start by initializing a queue allPaths to the
 * coordinates of the head of the snake. We then determine all valid paths from
 * the head using the findValidNeighbors function. We extend the current path by
 * adding all valid neighbors to the end of the queue allPaths. If any of the valid
 * neighbors is the apple, we are done. Else, we continue to iterate through the
 * queue and adding more paths until we find the path which ends at the apple. We
 * record all visited locations in a set to ensure that we do not back track. For
 * every step, we update the snake by using moving it a step along the path. This
 * is done to ensure that we keep track of the dynamic nature of the obstacles.
 */
Stack<GridLocation> bfs(Snake& snake, GridLocation appleLocation){
    Stack<GridLocation> currentPath;
    GridLocation currentLocation = snake.head();
    Queue<Stack<GridLocation>> allPaths;
    Set<GridLocation> seenLocations;
    currentPath.push(currentLocation);
    allPaths.enqueue(currentPath);

    // BFS to determine shortest path
    while(!allPaths.isEmpty()){

        //Current path under examination
        currentPath = allPaths.peek();
        currentLocation = currentPath.peek();

        if (currentLocation == appleLocation){
            return currentPath;
        }

        //Generate valid extensions to current path to create a new path
        //and add to end of queue
        Snake tempSnake = snake;
        tempSnake.move(currentPath);
        Vector<GridLocation> neighbors = findValidNeighbors(tempSnake);
        for (GridLocation neighbor : neighbors){
            if (!seenLocations.contains(neighbor)){ //Prevents backtracking
                Stack<GridLocation> newPath = currentPath;
                newPath.push(neighbor);
                allPaths.enqueue(newPath);
            }
        }
        allPaths.dequeue();
        seenLocations.add(currentLocation);
    }
    return {};
}

/*
 * This function takes a vector containing paths (Stack<GridLocation>) as argument
 * and returns the shortest path.
 */
Stack<GridLocation> shortestPath(Vector<Stack<GridLocation>> allPaths){
    Stack<GridLocation> shortPath = allPaths[0];
    for (Stack<GridLocation> path : allPaths){
        if (path.size() < shortPath.size()){
            shortPath = path;
        }
    }
    return shortPath;
}

/*
 * This function takes a stack (GridLocation) by reference and the GridLocation loc
 * as arguments and adds loc to the bottom of the stack.
 */
void addToBack(Stack<GridLocation>& stack, GridLocation loc){
    Stack<GridLocation> temp;
    while(!stack.isEmpty()){
        GridLocation element = stack.pop();
        temp.push(element);
    }
    stack.push(loc);
    while(!temp.isEmpty()){
        GridLocation element = temp.pop();
        stack.push(element);
    }
}

/*
 * This function takes coordinates of two points (GridLocation) and returns the
 * distance between them.
 */
int dist(GridLocation a, GridLocation b){
    return abs(a.row - b.row) + abs(a.col - b.col);
}

/*
 * This function takes of vector (GridLocation) v by reference and a GridLocation
 * target as arguments and sorts the vector in ascending order of distance of the
 * elements of v from the target. The runtime complexity of the sorting is O(n^2).
 */
void sortByDist(Vector<GridLocation>& v, GridLocation target){
    Vector<int> d;
    for (GridLocation element : v){
        d.add(dist(element, target));
    }
    for (int j = 0; j < v.size(); j ++){
        for (int i = 0; i < v.size() - 1; i ++){
            if (d[i] > d[i + 1]){
                d[i + 1] = d[i] - d[i + 1];
                d[i] = d[i] - d[i + 1];
                d[i + 1] = d[i] + d[i + 1];

                GridLocation temp = v[i + 1];
                v[i + 1] = v[i];
                v[i] =  temp;
            }
        }
    }
}

/*
 * This function takes a Snake object snake and the coordinates of the apple as
 * the argument and uses recursion to determine the shortest path from the head
 * of the snake to the apple. The base cases are as follows: if the head of the
 * snake does not have any valid neighbors, we return an empty path. If the head
 * coincides with the coordinates of the apple, we return the coordinates of the
 * apple as the path. The resursive case is as follows: We first obtain the valid
 * neighbors of the head of the snake using the findValidNeighbors function. Then
 * we sort the neighbors based on their distance from the location of the apple
 * using the sortByDist function. For each neighbor, we make a copy of the snake
 * tempSnake and move it forward one step using the move method, and recursively
 * call the function on the updated copy of the snake. If the recursive call
 * returns a non-empty path, we add the neighbor to the bottom of the stack and
 * return the path. If the returned path is empty, we go to the next valid neighbor.
 * This process allows us to explore paths in order of their length. The returned
 * path would be the shortest in length.
 */
Stack<GridLocation> recursion(Snake& snake, GridLocation appleLocation){
    if (findValidNeighbors(snake).isEmpty()){
        return {};
    }
    if (snake.contains(appleLocation)){
        return {appleLocation};
    }

    Vector<GridLocation> neighbors = findValidNeighbors(snake);
    sortByDist(neighbors, appleLocation);
    for (GridLocation neighbor : neighbors){
        Snake tempSnake = snake;
        tempSnake.move(neighbor);
        Stack<GridLocation> path = recursion(tempSnake, appleLocation);
        if (!path.isEmpty()){
            addToBack(path, snake.head());
            return path;
        }
    }
    return {};
}

PROVIDED_TEST("Test findValidNeighbors()"){
    Snake snake;
    snake.boardSize = 5;
    EXPECT_ERROR(findValidNeighbors(snake));
    snake.body = {{2, 0}, {3, 0}, {4, 0}};
    EXPECT_EQUAL(findValidNeighbors(snake), {{4, 1}});
    snake.body = {{3, 0}, {3, 1}, {4, 1}, {4, 0}};
    EXPECT_EQUAL(findValidNeighbors(snake), {});
}

PROVIDED_TEST("Test bfs"){
    Snake snake;
    snake.boardSize = 5;
    snake.body = {{2, 0}, {3, 0}, {4, 0}};
    GridLocation appleLocation = {4, 3};
    Stack<GridLocation> path = {{4, 0}, {4, 1}, {4, 2}, {4, 3}};
    EXPECT_EQUAL(bfs(snake, appleLocation), path);
    snake.body = {{3, 0}, {3, 1}, {4, 1}, {4, 0}};
    EXPECT_EQUAL(bfs(snake, appleLocation), {});
}

PROVIDED_TEST("Test sortByDist"){
    Snake snake;
    snake.boardSize = 5;
    snake.body = {{2, 1}, {2, 2}};
    GridLocation appleLocation = {2, 4};
    Vector<GridLocation> neighbors = findValidNeighbors(snake);
    sortByDist(neighbors, appleLocation);
    EXPECT_EQUAL(neighbors, {{2, 3}, {1, 2}, {3, 2}});
}

PROVIDED_TEST("Test recursion"){
    Snake snake;
    snake.boardSize = 5;
    snake.body = {{2, 0}, {3, 0}, {4, 0}};
    GridLocation appleLocation = {4, 3};
    Stack<GridLocation> path = {{4, 0}, {4, 1}, {4, 2}, {4, 3}};
    EXPECT_EQUAL(recursion(snake, appleLocation), path);
    snake.body = {{3, 0}, {3, 1}, {4, 1}, {4, 0}};
    EXPECT_EQUAL(recursion(snake, appleLocation), {});
}

PROVIDED_TEST("Time BFS"){
    Vector<int> gridSize = {5, 8, 10, 12};
    for (int g : gridSize){
        Snake snake;
        snake.boardSize = g;
        snake.body = {{0, 0}, {0, 1}};
        GridLocation appleLocation = {g - 1, g - 1};
        TIME_OPERATION(g, bfs(snake, appleLocation));
    }
}

PROVIDED_TEST("Time Recursion"){
    for (int i = 200; i < 2000; i *=2){
        Snake snake;
        snake.boardSize = i;
        snake.body = {{0, 0}, {0, 1}};
        GridLocation appleLocation = {i - 1, i - 1};
        TIME_OPERATION(i, recursion(snake, appleLocation));
    }
}

