#include "Project08.h"
#include "Project08.h2"

/**************************************************************
 * mouse_init()
 * This function is called each time a maze map is loaded.
 **************************************************************/
void mouse_init(int x,int y)
{ 

// Set mouse position

  col = x;
  row = y;
}

/**************************************************************
 * mouse_move()
 * This function is called each timer tick.
 **************************************************************/
void mouse_move()
{ int r;
  cell map[3][3];

// Look around

  mouse_look(map);
  
  // If we have queued directions
	  // Follow the next queued direction
  // Otherwise
	  // If no cheese nearby:

		// Determine which paths are not backtracking (we just came from it)
		// Pick one of the not-just-traveled-on paths, and go.
		// If no other paths, then backtrack

		// Remember where we are -- use a static variable
	  
		// Update our position
	  // If cheese nearby
		// Determine the shortest path to collect all the seen cheeses.
		// Enqueue the steps to reach those cheeses
		// Move based on the first item in the queue
	  // If exit nearby
	    // Find path to the exit and enqueue it
		// Pull the first item in the queue and move

/**
  r = rand()%4;

  if(r==0) if(mouse_north()) row--;
  if(r==1) if(mouse_south()) row++;
  if(r==2) if(mouse_east()) col++;
  if(r==3) if(mouse_west()) col--;
*/
}
