#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>

// Defines

#define MAZE_ROWS  24
#define MAZE_COLS  32

// Cell types

enum cell { CELL_OPEN,CELL_WALL,CELL_CHEESE,CELL_EXIT };

// Library Functions

/******************************************************
 * Look around and return 3x3 array copy of the maze.
 * The mouse is at position map[1][1]
 *  |------|-------|------|
 *  |  NW  | North |  NE  |
 *  |------|-------|------|
 *  | West | Mouse | East |
 *  |------|-------|------|
 *  |  SW  | South |  SE  |
 *  |------|-------|------|
 *****************************************************/ 

void mouse_look(cell map[3][3]);

// Attempt to move NSEW.  If a wall is in the way, the mouse does not move

bool mouse_north();
bool mouse_south();
bool mouse_east();
bool mouse_west();
