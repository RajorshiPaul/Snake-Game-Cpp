/*
 * The file has been created solely by Rajorshi Paul. No external
 * assistance or collaboration was used during the creation of this
 * file.
 */

#pragma once
#include "gridlocation.h"
#include "grid.h"
#include "snake.h"
#include "stack.h"

Stack<GridLocation> bfs(Snake& snake, GridLocation appleLocation);
Stack<GridLocation> recursion(Snake& snake, GridLocation appleLocation);
