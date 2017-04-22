#include "Project08.h"
#include "Project08.h2"
#include <queue>

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
{
    // Initialize the variable so we can tell if we accidentally fell through without choosing a direction
    int r = -1;
    cell map[3][3];
    // This move queue is only in the scope of this function, and it allows us to enqueue commands between runs of this function.
    // Declared static so that we don't erase the data on every run of mouse_move().
    static std::queue<int> move_queue;

    // Look around

    mouse_look(map);

    // If we have queued directions
    if (!move_queue.empty())
    {
	// Follow the next queued direction
	r = move_queue.front();
	// Remove the element we fetched.
	move_queue.pop();
    }
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

    // If we didn't choose a direction, select one randomly.
    // This is a fallback to handle accidental code fallthrough, not a viable move strategy
    if (r == -1)
    {
	r = rand() % 4;
    }
    if(r==0) if(mouse_north()) row--;
    if(r==1) if(mouse_south()) row++;
    if(r==2) if(mouse_east()) col++;
    if(r==3) if(mouse_west()) col--;

}
