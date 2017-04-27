#include "Project08.h"
#include "Project08.h2"
#include <queue>
#include <vector>
#include <cmath>

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

/**
 * get_dir_from_rel_loc()
 * Return the relative direction (as determined at the bottom of
 * mouse_move() to the direction moved) from the position in
 * the map returned from mouse_look().
 *
 * @param loc
 * The location we are calculating the direction for.
 *
 * @return
 * 0 for north, 1 for south, 2 for east, 3 for west
 *
 * @note
 * Behavior is not defined for diagonals.
 */
static int get_dir_from_rel_loc(const map_pos &loc)
{
	if (loc.x == 0)
		return 0; // If x is zero, we are moving up.
	else if (loc.x == 2)
		return 1; // If x is two, we are moving down.
	// In these cases, loc.x will be 1
	else if (loc.y == 0)
		return 3; // We move to the left if y is zero.
	// Last case: loc.x == 1 && loc.y == 2
	else
		return 2; // Move to the right
}

/**************************************************************
 * mouse_move()
 * This function is called each timer tick.
 **************************************************************/
void mouse_move()
{
    // Initialize the variable so we can tell if we accidentally fell through without choosing a direction
    int r = -1, move_index;
    cell map[3][3];
    // Declare some vectors so we can keep track of where cheese and exits are. Also one to determine where to go if nothing else is there
    std::vector<map_pos> cheese_loc, exit_loc, viable_loc;
    // Temp variable for determining direction based on the stored location.
    map_pos loc, old_loc = {-1, -1};
    int i, j;
    // This move queue is only in the scope of this function, and it allows us to enqueue commands between runs of this function.
    // Declared static so that we don't erase the data on every run of mouse_move().
    static std::queue<int> move_queue;
    // Declare our tracker for our last movement.
    static int last_move = -1;

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
    else
    {
		// Look for all nearby cheeses and exits
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				// Keep track of where nearby cheeses are
				if (map[i][j] == CELL_CHEESE)
				{
					cheese_loc.push_back({ i, j });
				}
				// Also track the exits nearby
				else if (map[i][j] == CELL_EXIT)
					exit_loc.push_back({ i, j });
			}
		}
		// If no cheese nearby and no exits nearby:
		if (cheese_loc.empty() == true && exit_loc.empty() == true)
		{
			// Figure out which directions are allowable directions
			if (map[1][0] == CELL_OPEN)
				viable_loc.push_back({ 1, 0 });
			if (map[0][1] == CELL_OPEN)
				viable_loc.push_back({ 0, 1 });
			if (map[1][2] == CELL_OPEN)
				viable_loc.push_back({ 1, 2 });
			if (map[2][1] == CELL_OPEN)
				viable_loc.push_back({ 2, 1 });

			// Only one option, then take it.
			if (viable_loc.size() == 1)
			{
				r = get_dir_from_rel_loc(viable_loc[0]);
			}
			// Special case for two options
			// Go the direction we didn't come from.
			else if (viable_loc.size() == 2)
			{
				if (get_dir_from_rel_loc(viable_loc[0]) == last_move)
					r = get_dir_from_rel_loc(viable_loc[1]);
				else
					r = get_dir_from_rel_loc(viable_loc[0]);
			}
			else
			{
				// Determine which paths are not backtracking (we just came from it)
				// Remove the path that can be equated to the direction we came from
				for (i = 0; i < viable_loc.size(); ++i)
					if (get_dir_from_rel_loc(viable_loc[i]) == last_move)
					{
						move_index = i;
						break; // We found it, don't keep searching
					}
				// Pick one of the not-just-traveled-on paths, and go.
				// Recycle i, since we only need a temp variable
				i = rand() % (viable_loc.size() - 1);
				// If the index is at or beyond the index we skipped, increment the index
				// to bypass the index we are skipping. also why we did MOD (LEN - 1)
				if (i >= move_index)
					++i;
				// Set our direction accordingly
				r = get_dir_from_rel_loc(viable_loc[i]);
				// TODO: Determine a way to handle an open room better
			}
		}
		// If exit nearby
		else if (exit_loc.empty() == false)
		{
			// Don't worry about multiple exits -- try to reach the first.
			loc = exit_loc[0];
			// The absolute value of the difference of the relative coordinates
			// is 1 for all cardinal direction movement
			if (abs(loc.x - loc.y) == 1)
				// The exit is next to us, just move to it.
				r = get_dir_from_rel_loc(loc);
			else
			{
				// Find path to the exit and enqueue it
				// First, handle the northwest and northeast corners
				if (loc.x == 0)
				{
					// Northwest corner
					if (loc.y == 0)
					{
						// Check the north and west directions for a path.
						// First, check west
						if (map[1][0] == CELL_OPEN)
						{
							r = 3;              // Go west
							move_queue.push(0); // Enqueue a north move
						}
						// Then check north
						else if (map[0][1] == CELL_OPEN)
						{
							r = 0;              // Go north
							move_queue.push(3); // Enqueue a west move
						}
					}
					else
					{
						// Check the north and east directions for a path
						// First, check east
						if (map[2][1] == CELL_OPEN)
						{
							r = 2;  	    // Go east
							move_queue.push(0); // Enqueue a north move
						}
						// Then check north
						else if (map[0][1] == CELL_OPEN)
						{
							r = 0;		   // Go north
							move_queue.push(2); // Enqueue an east move
						} 
					}
				}
				else
				{
					// Southwest corner
					if (loc.y == 0)
					{
						// Check the south and west directions for a path
						// First, check west
						if(map[1][0] == CELL_OPEN)
						{
							r = 3; 		    // Go west
							move_queue.push(1); // Enqueue a south move
						}
						// Then check south
						else if(map[1][2] == CELL_OPEN)
						{
							r = 1;		    // Go south
							move_queue.push(3); // Enqueue a west move
						}
					}
					else
					{
						// Check the south and east directions for a path
						// First, check east
						if(map[2][1] == CELL_OPEN)
						{
							r = 2;            // Go east
							move_queue.push(1); // Enqueue a south move
						}
						// Then check south
						else if(map[1][2] == CELL_OPEN)
						{
							r = 1;		    // Go south
							move_queue.push(2); // Enqueue an east move
						}
					}
				}
			}
		}
		// If cheese nearby and no exits
		else
		{
			if (cheese_loc.size() == 1)
			{
				// Handle like the exit -- path to the one item.
				// TODO: Implement
			}
			else
			{
				// Determine a path to collect all the seen cheeses.
				for (i = 0; i < cheese_loc; ++i)
				{
					// Enqueue the steps to reach those cheeses
					loc = cheese_loc[i];
					if (old_loc.x == -1)
					{
						// Handle from [1,1] if we haven't parsed any yet
						if (abs(loc.x - loc.y) == 1)
							move_queue.push(get_dir_from_rel_loc(loc));
						else
						{
							// Do like the exits for diagonals
							// Find path to the cheese and enqueue it
							// First, handle the northwest and northeast corners
							if (loc.x == 0)
							{
								// Northwest corner
								if (loc.y == 0)
								{
									// Check the north and west directions for a path.
									// First, check west
									if (map[1][0] == CELL_OPEN)
									{
										move_queue.push(3); // Go west
										move_queue.push(0); // Enqueue a north move
									}
									// Then check north
									else if (map[0][1] == CELL_OPEN)
									{
										move_queue.push(0); // Go north
										move_queue.push(3); // Enqueue a west move
									}
								}
								else
								{
									// Check the north and east directions for a path
									// First, check east
									if (map[2][1] == CELL_OPEN)
									{
										move_queue.push(2); // Go east
										move_queue.push(0); // Enqueue a north move
									}
									// Then check north
									else if (map[0][1] == CELL_OPEN)
									{
										move_queue.push(0); // Go north
										move_queue.push(2); // Enqueue an east move
									} 
								}
							}
							else
							{
								// Southwest corner
								if (loc.y == 0)
								{
									// Check the south and west directions for a path
									// First, check west
									if(map[1][0] == CELL_OPEN)
									{
										move_queue.push(3); // Go west
										move_queue.push(1); // Enqueue a south move
									}
									// Then check south
									else if(map[1][2] == CELL_OPEN)
									{
										move_queue.push(1); // Go south
										move_queue.push(3); // Enqueue a west move
									}
								}
								else
								{
									// Check the south and east directions for a path
									// First, check east
									if(map[2][1] == CELL_OPEN)
									{
										move_queue.push(2); // Go east
										move_queue.push(1); // Enqueue a south move
									}
									// Then check south
									else if(map[1][2] == CELL_OPEN)
									{
										move_queue.push(1);	// Go south
										move_queue.push(2); // Enqueue an east move
									}
								}
							}
						}
					}
					else
					{
						// We are going from old_loc to the next loc.
						// TODO: Determine how to calculate this
					}
				}
				// Move based on the first item in the queue
				r = move_queue.front();
				move_queue.pop();
			}
		}
    }
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
    // Update our last movement tracker. Keep track of the reverse operation of the move we did.
    // The toggles should work as below
	// (the & 4 ensures we only care about the first two bits) :
    // (0b11 ^ 1) & 4 == 0b10;
	// (0b10 ^ 1) & 4 == 0b11;
	// (0b01 ^ 1) & 4 == 0b00;
	// (0b00 ^ 1) & 4 == 0b01;
    last_move = (r ^ 1) & 4; // Flip that single bit to toggle north/south or east/west movement

}
