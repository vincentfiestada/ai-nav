#include "cardinal.h"
#include "polygon.h"
#include "queue.h"
#include "stack.h"

#define H 26
#define W 40

// Tile states
#define BLOCKED 1
#define CURRENT 2
#define EXPLORED 3
#define QUEUED 4
#define UNEXPLORED 5
#define GOAL 6

// Search strategies
#define STRAT_BFS 1
#define STRAT_DFS 2
#define STRAT_ASTAR 3

// Global variables
int grid[H][W];
polygon p;

void setTile(unsigned int x, unsigned int y, unsigned int s);
unsigned int getTile(unsigned int x, unsigned int y);
coordinate teleport(coordinate current, coordinate target);
void drawGrid();

void BFS(Queue * fringe, coordinate current);
void DFS(Stack * fringe, coordinate current);

int main()
{
    // Declare iterators
    unsigned int i,j,k;

    // Create grid
    for (i = 0; i < H; i++)
    {
        for (j = 0; j < W; j++)
        {
            grid[i][j] = UNEXPLORED;
        }
    }

    // Some sample vertices
    // Later, they should be read from an input file

    // A triangle (3 vertices)
    unsigned int vertices[3][2] =
    {{20, 4},
     {12, 12},
     {24, 23}
    };

    line e[3];
    for (i = 0; i < 3; i++)
    {
        coordinate nv; // Get next vertex
        if (i < 3 - 1)
        {
            nv.x = vertices[i+1][0];
            nv.y = vertices[i+1][1];
        }
        else
        {
            nv.x = vertices[0][0];
            nv.y = vertices[0][1];
        }
        coordinate c;
        c.x = vertices[i][0];
        c.y = vertices[i][1];
        e[i].m = getSlope(c, nv);
        e[i].b = getYIntercept(c, e[i].m);
        e[i].one.x = vertices[i][0];
        e[i].one.y = vertices[i][1];
        e[i].two.x = nv.x;
        e[i].two.y = nv.y;
    }

    p.edges = e;

    // Set blocked tiles
    for (i = 0; i < H; i++)
    {
        printf("%2d ", i);
        for (j = 0; j < W; j++)
        {
            for (k = 0; k < 3; k++)
            {
                coordinate c;
                c.x = j;
                c.y = i;
                line e = p.edges[k];
                if (inLine(c, &(e)))
                {
                    grid[i][j] = BLOCKED;
                    break;
                }
            }
        }
        printf("\n");
    }
    // Set starting point and goal
    coordinate current;
    current.x = 0;
    current.y = 0;
    setTile(0, 0, CURRENT);
    setTile(39 ,0, GOAL);

    i = 0;
    drawGrid();

    int strategy;
    printf("\nChoose a Search Strategy\n1 - BFS\n2 - DFS\n3 - A* Search\n>>> Enter Choice: ");
    scanf("%d", &strategy);

    if (strategy == STRAT_BFS)
    {
        // Create fringe queue
        Queue * fringe = CreateNewQueue();
        do
        {
            // Check if we've found the goal
            if (getTile(current.x, current.y) == GOAL)
            {
                break;
            }
            BFS(fringe, current);
            // If fringe is nonempty, advance to next tile in the fringe queue
            if (fringe->Head != NULL)
            {
                coordinate target = Dequeue(fringe);
                current = teleport(current, target);
            }
            i++;
        } while(1);

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
           CLEAN UP: Delete dynamically allocated objs
          <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

        AnnihilateQueue(fringe);

    }
    else if (strategy == STRAT_DFS)
    {
        // Create fringe stack
        Stack * fringe = CreateNewStack();
        do
        {
            // Check if we've found the goal
            if (getTile(current.x, current.y) == GOAL)
            {
                break;
            }
            DFS(fringe, current);
            // If fringe is not empty, move to next node in stack
            if (fringe->Top != NULL)
            {
                coordinate target = PopFromStack(fringe);
                current = teleport(current, target);
            }
            i++;
        } while(1);

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
           CLEAN UP: Delete dynamically allocated objs
          <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

        AnnihilateStack(fringe);
    }

    drawGrid();
    printf("\n--------------------------------------------------\n");
    printf("SUCCESS! Current Location is (%d, %d), which is a GOAL state. Took %d steps", current.x, current.y, i);

    return 0;
}

/*
 * setTile() - Set coordinates (x,y) to status s
 */
void setTile(unsigned int x, unsigned int y, unsigned int s)
{
    grid[y][x] = s;
}

/*
 * getTile() - Get status of tile at coordinates (x,y)
 */
unsigned int getTile(unsigned int x, unsigned int y)
{
    return grid[y][x];
}

/*
 * teleport() - Move into the given coordinates (x,y)
 *  returns new current coordinate
 */
coordinate teleport(coordinate current, coordinate target)
{
    setTile(current.x, current.y, EXPLORED);
    if (getTile(target.x, target.y) != GOAL) // Status GOAL MUST supercede CURRENT
    {
        setTile(target.x, target.y, CURRENT);
    }
    return target;
}

/*
 * drawGrid() - draw the grid with coordinates (Recommended only for W = 40 on most resolutions)
 */
void drawGrid()
{
    unsigned int i, j, k;
    printf("    ");
    for (j = 0; j < W; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    for (i = 0; i < H; i++)
    {
        printf("%2d ", i);
        for (j = 0; j < W; j++)
        {
            switch(grid[i][j])
            {
                case GOAL:
                    printf("[X]");
                    break;
                case EXPLORED:
                    printf(":::");
                    break;
                case BLOCKED:
                    printf("!@!");
                    break;
                case CURRENT:
                    printf("^0^");
                    break;
                case QUEUED:
                    printf(" . ");
                    break;
                default:
                    printf("   ");
            }
        }
        printf("\n");
    }
}

/*
 * BFS() - "Breadth-First": Enqueue the BFS successors of the current coordinate
 */
void BFS(Queue * fringe, coordinate current)
{
    // Assume BFS relative order to be: Right, Left, Up, Down
    // Check if RIGHT successor is viable
    if (current.x < W - 1 && getTile(current.x + 1, current.y) >= UNEXPLORED)
    {
        Enqueue(fringe, current.x + 1, current.y);
        if(getTile(current.x + 1, current.y) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x + 1, current.y, QUEUED);
        }
    }
    // Check LEFT
    if (current.x > 0 && getTile(current.x - 1, current.y) >= UNEXPLORED)
    {
        Enqueue(fringe, current.x - 1, current.y);
        if(getTile(current.x - 1, current.y) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x - 1, current.y, QUEUED);
        }
    }
    // Check UP (but remember, in our grid system, up means lower y)
    if (current.y > 0 && getTile(current.x, current.y - 1) >= UNEXPLORED)
    {
        Enqueue(fringe, current.x, current.y - 1);
        if(getTile(current.x, current.y - 1) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x, current.y - 1, QUEUED);
        }
    }
    // Check DOWN
    if (current.y < H - 1 && getTile(current.x, current.y + 1) >= UNEXPLORED)
    {
        Enqueue(fringe, current.x, current.y + 1);
        if(getTile(current.x, current.y + 1) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x, current.y + 1, QUEUED);
        }
    }
    return;
}

/*
 * DFS() - "Depth-First": Push into the stack the DFS successors of the current coordinate
 */
void DFS(Stack * fringe, coordinate current)
{
    // Assume DFS relative order to be: Right, Left, Up, Down
    // But then we have to push them into the stack in reverse order (i.e., Down, up, left, right)
    // Check DOWN successor
    if (current.y < H - 1 && getTile(current.x, current.y + 1) >= UNEXPLORED)
    {
        PushToStack(fringe, current.x, current.y + 1);
        if(getTile(current.x, current.y + 1) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x, current.y + 1, QUEUED);
        }
    }
    // Check UP (but remember, in our grid system, up means lower y)
    if (current.y > 0 && getTile(current.x, current.y - 1) >= UNEXPLORED)
    {
        PushToStack(fringe, current.x, current.y - 1);
        if(getTile(current.x, current.y - 1) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x, current.y - 1, QUEUED);
        }
    }
    // Check LEFT
    if (current.x > 0 && getTile(current.x - 1, current.y) >= UNEXPLORED)
    {
        PushToStack(fringe, current.x - 1, current.y);
        if(getTile(current.x - 1, current.y) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x - 1, current.y, QUEUED);
        }
    }
    // Check if RIGHT successor is viable
    if (current.x < W - 1 && getTile(current.x + 1, current.y) >= UNEXPLORED)
    {
        PushToStack(fringe, current.x + 1, current.y);
        if(getTile(current.x + 1, current.y) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x + 1, current.y, QUEUED);
        }
    }
    return;
}
