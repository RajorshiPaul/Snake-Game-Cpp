/*
 * The file has been modified by Rajorshi Paul from the original file
 * created by Julie Zelenski.
 */

#pragma once
#include <string>
#include "grid.h"
#include "stack.h"

namespace SnakeGraphics {


    /**
     * Draws a visual rendition of the Grid to a graphics windows.
     * Cell with value "true" is an open corridor, drawn in white.
     * Cell with value "false" is a wall, drawn in dark gray.
     */
    void drawGrid(const Grid<bool>& g);


    /**
     * Draws a filled dot in center of each location in path in
     * the specified color.
     * Color string is either a named color such as "red", "blue"
     * "green" or a hex color "#33ff33"
     *
     * highlightPath is used to add dots to the path locations
     * on an already drawn grid. It is an error to call highlightPath
     * without having previously called the drawGrid function.
     *
     * The optional fourth argument is the number of milliseconds to
     * pause after drawing, this slows down the animation so you can watch
     * while trying to debug.
     */
    void highlightPath(Stack<GridLocation> p, std::string colorP, GridLocation apple, std::string colorApple, int msecsToPause = 0);

 }





