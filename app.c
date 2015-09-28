#include "cardinal.h"
#include "polygon.h"
#include "queue.h"
#include "stack.h"
#include "slist.h"
#include <time.h> // clock_t, clock(), CLOCKS_PER_SEC

#define H 200
#define W 400
//#define DEBUG

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

#define STRINGMAX 100

// Error codes
#define ERR_INPUTFILE_CANNOTOPEN 404

// Global variables
int grid[H][W];
coordinate pred[H][W]; // Used to keep track of the traversal
//pred[i][j] = (x,y) means that (i,j) comes after (x,y) in our path

void setTile(unsigned int x, unsigned int y, unsigned int s);
unsigned int getTile(unsigned int x, unsigned int y);
void setPred(unsigned int x, unsigned int y, unsigned int px, unsigned int py);
coordinate getPred(unsigned int x, unsigned int y);
coordinate teleport(coordinate current, coordinate target);
void drawGrid();
int absval(int x);
void BFS(Queue * fringe, coordinate current);
void DFS(Stack * fringe, coordinate current);
int h(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void Astar(SortedList * fringe, coordinate current, unsigned int g, coordinate goal);

int main()
{
    printf("\n=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n");
	printf("    2D Path Finding w/ Search || CS 180 Machine Problem 1\n");
	printf(" Vincent Paul F. Fiestada | 201369155 | vffiestada@up.edu.ph\n");
	printf("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n");
    #ifdef DEBUG
        printf("\n\n ------- DEBUG Mode ------\n\n");
    #endif

    // Declare iterators
    unsigned int i,j,k;

    // Create grid
    for (i = 0; i < H; i++)
    {
        for (j = 0; j < W; j++)
        {
            grid[i][j] = UNEXPLORED;
            // Set all predecessors to (-1,-1) (i.e., not part of the discovered path)
            setPred(j, i, -1, -1);
        }
    }

    // Open and parse input file
    // Get input file's filename
    char inputFilename[STRINGMAX] = "";
    printf("\nInput Filename (Max %d characters): ", STRINGMAX - 1);
    scanf("%s", &inputFilename);
    printf("\nOpening file '%s'...", inputFilename);
    printf("\n--------------------------------\n");

    FILE * inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) // If inputFile is null, then the stream couldn't be opened; report error
	{
		fprintf(stderr,"\nFATAL ERROR!\nFailed to open '%s'. ", inputFilename);
		exit(ERR_INPUTFILE_CANNOTOPEN); // exit with appropriate error code
	}
    // Set starting point and goal
    coordinate current;
    coordinate goal;

    fscanf(inputFile, "%d %d %d %d", &(current.x), &(current.y), &(goal.x), &(goal.y));
    setTile(current.x, current.y, CURRENT);
    setTile(goal.x, goal.y, GOAL);

    printf("\nFind Path from (%d, %d) to (%d, %d).", current.x, current.y, goal.x, goal.y);
    printf("\nObstacles:\n");
    unsigned int tempInt;
    while (fscanf(inputFile, "%d", &tempInt) > 0) // Number of vertices for this polygon
    {
        /*  FORMAT OF POLYGONS
         *  number_of_vertices x1 y1 x2 y2 ...
         */
         // Get coordinates of vertices
        coordinate vertices[tempInt];
        for (i = 0; i < tempInt; i++)
        {
            fscanf(inputFile, "%d %d", &(vertices[i].x), &(vertices[i].y));
        }
        /* >>>>>>>>> Create Polyon >>>>>>>> */
        // Determines lines/edges
        line e[tempInt];
        for (i = 0; i < tempInt; i++)
        {
            coordinate nv; // Get next vertex
            if (i < tempInt - 1)
            {
                nv.x = vertices[i+1].x;
                nv.y = vertices[i+1].y;
            }
            else
            {
                nv.x = vertices[0].x;
                nv.y = vertices[0].y;
            }
            e[i].m = getSlope(vertices[i], nv);
            e[i].b = getYIntercept(vertices[i], e[i].m);
            e[i].x = -1; // The line is NOT vertical
            // if the line is vertical, store the constant X because slope-intercept won't work
            if (nv.x == vertices[i].x)
            {
                e[i].x = nv.x;
            }
            e[i].one.x = vertices[i].x;
            e[i].one.y = vertices[i].y;
            e[i].two.x = nv.x;
            e[i].two.y = nv.y;
        }
        // Create new polygon
        polygon p;
        p.edges = e;
        // >>  Set blocked tiles <<
        // For each point,
        for (i = 0; i < H; i++)
        {
            for (j = 0; j < W; j++)
            {
                // For each edge of the polygon
                for (k = 0; k < tempInt; k++)
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
        }
        // Print obstacle vertices
        for (i = 0; i < tempInt; i++)
        {
            printf("(%d %d) ", vertices[i].x, vertices[i].y);
        }
        printf("\n");
    }

    i = 0; // keeps track of number of expanded nodes
    #ifdef DEBUG
        drawGrid();
    #endif

    int strategy;
    printf("\nChoose a Search Strategy\n1 - BFS\n2 - DFS\nOther - A* Search\n>>> Enter Choice: ");
    scanf("%d", &strategy);

    printf("\nStarting Search...\n");
    clock_t t = clock(); // For keeping track of running time
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
                // We're gonna move from current to the target tile
                coordinate target = Dequeue(fringe);
                current = teleport(current, target);
            }
            else
            {
                printf("\n\n <!> No solution path found.");
                break;
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
            else
            {
                printf("\n\n <!> No solution path found.");
                break;
            }
            i++;
        } while(1);

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
           CLEAN UP: Delete dynamically allocated objs
          <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

        AnnihilateStack(fringe);
    }
    else // Use A* as default strategy
    {
        // Create fringe sorted doubly linked list
        SortedList * fringe = CreateNewSortedList();
        do
        {
            // Check if we've found the goal
            if (getTile(current.x, current.y) == GOAL)
            {
                break;
            }
            // Get A* search successors (automagically sorted)
            // i = current level - 1 = g(n)
            Astar(fringe, current, i, goal);
            // If fringe is nonempty, advance to next tile in the fringe
            if (fringe->Head != NULL)
            {
                // We're gonna move from current to the target tile
                coordinate target = PopFromSortedList(fringe);
                current = teleport(current, target);
            }
            else
            {
                printf("\n\n <!> No solution path found.");
                break;
            }
            i++;
        } while(1);

        /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
           CLEAN UP: Delete dynamically allocated objs
          <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

        AnnihilateSortedList(fringe);

    }
    // Build the path by tracing back our footsteps
    Stack * path = CreateNewStack();
    // Push into the stack the final tile, which is the goal (also the current) tile
    PushToStack(path, current.x, current.y);
    while(1)
    {
        // Get predecessor of current top of stack
        coordinate p = getPred(path->Top->Data.x, path->Top->Data.y);
        if (p.x == -1 || p.y == -1) break;
        PushToStack(path, p.x, p.y);
    }
    // Get number of clock ticks since last check to detection of final soln
    t = clock() - t;
    #ifdef DEBUG
        drawGrid();
    #endif
    printf("\n--------------------------------------------------\n");
    printf("Final Location is (%d, %d)", current.x, current.y);
    if (getTile(current.x, current.y) == GOAL) printf(" which is a GOAL point.");
    printf("\n\n");
    printf("Traced Path ");
    switch(strategy)
    {
        case STRAT_BFS:
            printf("(BFS): ");
            break;
        case STRAT_DFS:
            printf("(DFS): ");
            break;
        default:
            printf("(A*): ");
    }
    PrintStack(path);
    printf("\n\n*Includes initial and final positions.");
    printf("\n\n----------------------------------------\nNumber of expanded nodes: %d", i);
    printf("\nSolution cost: %d (Cost is 1 per step)", path->Depth - 1);
    printf("\nRunning time: %f s (for the search part only)\n\n", ((float)t)/CLOCKS_PER_SEC);

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
       CLEAN UP: Delete dynamically allocated objs
      <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

    AnnihilateStack(path);

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
 * setPred() - Set the tile visited before, i.e., predecessor to a tile with coordinates (x,y)
 */
void setPred(unsigned int x, unsigned int y, unsigned int px, unsigned int py)
{
    pred[y][x].x = px;
    pred[y][x].y = py;
}

/*
 * getPred() - Get the tile visited before, i.e., predecessor to a tile with coordinates (x,y)
 *             Returns the coordinate
 */
coordinate getPred(unsigned int x, unsigned int y)
{
    return pred[y][x];
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
    int i, j;
    printf("\n");
    for (i = 0; i < H; i++)
    {
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
        // We MIGHT move from current tile to this tile
        // Set tile's predecessor to the current tile
        setPred(current.x + 1, current.y, current.x, current.y);
    }
    // Check LEFT
    if (current.x > 0 && getTile(current.x - 1, current.y) >= UNEXPLORED)
    {
        Enqueue(fringe, current.x - 1, current.y);
        if(getTile(current.x - 1, current.y) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x - 1, current.y, QUEUED);
        }
        // Set tile's predecessor to the current tile
        setPred(current.x - 1, current.y, current.x, current.y);
    }
    // Check UP (but remember, in our grid system, up means lower y)
    if (current.y > 0 && getTile(current.x, current.y - 1) >= UNEXPLORED)
    {
        Enqueue(fringe, current.x, current.y - 1);
        if(getTile(current.x, current.y - 1) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x, current.y - 1, QUEUED);
        }
        // Set tile's predecessor to the current tile
        setPred(current.x, current.y - 1, current.x, current.y);
    }
    // Check DOWN
    if (current.y < H - 1 && getTile(current.x, current.y + 1) >= UNEXPLORED)
    {
        Enqueue(fringe, current.x, current.y + 1);
        if(getTile(current.x, current.y + 1) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x, current.y + 1, QUEUED);
        }
        // Set tile's predecessor to the current tile
        setPred(current.x, current.y + 1, current.x, current.y);
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
        // We MIGHT move from current tile to this tile
        // Set tile's predecessor to the current tile
        setPred(current.x, current.y + 1, current.x, current.y);
    }
    // Check UP (but remember, in our grid system, up means lower y)
    if (current.y > 0 && getTile(current.x, current.y - 1) >= UNEXPLORED)
    {
        PushToStack(fringe, current.x, current.y - 1);
        if(getTile(current.x, current.y - 1) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x, current.y - 1, QUEUED);
        }
        // Set tile's predecessor to the current tile
        setPred(current.x, current.y - 1, current.x, current.y);
    }
    // Check LEFT
    if (current.x > 0 && getTile(current.x - 1, current.y) >= UNEXPLORED)
    {
        PushToStack(fringe, current.x - 1, current.y);
        if(getTile(current.x - 1, current.y) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x - 1, current.y, QUEUED);
        }
        // Set tile's predecessor to the current tile
        setPred(current.x - 1, current.y, current.x, current.y);
    }
    // Check if RIGHT successor is viable
    if (current.x < W - 1 && getTile(current.x + 1, current.y) >= UNEXPLORED)
    {
        PushToStack(fringe, current.x + 1, current.y);
        if(getTile(current.x + 1, current.y) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x + 1, current.y, QUEUED);
        }
        // Set tile's predecessor to the current tile
        setPred(current.x + 1, current.y, current.x, current.y);
    }
    return;
}

/*
 * absval() - Returns the absolute value of a function
 */
int absval(int x)
{
    return (x < 0) ? -1 * x : x;
}

/*
 * h() - Returns the estimated distance of a point alpha to the goal
 *                      - Uses the (optimistic) Manhattan distance as the heuristic
 */
int h(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    return absval(x2 - x1) + absval(y2 - y1);
}

/*
 * Astar() - "A* Search": Enqueue the A* successors of the current coordinate (sorted upon insertion)
 *         - arguments: fringe (SortedList) to insert successors into, current position of robot, and g(n) or the current *           level - 1, goal (coordinate), which is needed to compute h(n)
 */
void Astar(SortedList * fringe, coordinate current, unsigned int g, coordinate goal)
{
    // Order doesn't matter
    // Check if RIGHT successor is viable
    if (current.x < W - 1 && getTile(current.x + 1, current.y) >= UNEXPLORED)
    {
        int f = g + h(current.x + 1, current.y, goal.x, goal.y);
        InsertToSortedList(fringe, current.x + 1, current.y, f);
        if(getTile(current.x + 1, current.y) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x + 1, current.y, QUEUED);
        }
        // We MIGHT move from current tile to this tile
        // Set tile's predecessor to the current tile
        setPred(current.x + 1, current.y, current.x, current.y);
    }
    // Check LEFT
    if (current.x > 0 && getTile(current.x - 1, current.y) >= UNEXPLORED)
    {
        int f = g + h(current.x - 1, current.y, goal.x, goal.y);
        InsertToSortedList(fringe, current.x - 1, current.y, f);
        if(getTile(current.x - 1, current.y) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x - 1, current.y, QUEUED);
        }
        // Set tile's predecessor to the current tile
        setPred(current.x - 1, current.y, current.x, current.y);
    }
    // Check UP (but remember, in our grid system, up means lower y)
    if (current.y > 0 && getTile(current.x, current.y - 1) >= UNEXPLORED)
    {
        int f = g + h(current.x, current.y - 1, goal.x, goal.y);
        InsertToSortedList(fringe, current.x, current.y - 1, f);
        if(getTile(current.x, current.y - 1) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x, current.y - 1, QUEUED);
        }
        // Set tile's predecessor to the current tile
        setPred(current.x, current.y - 1, current.x, current.y);
    }
    // Check DOWN
    if (current.y < H - 1 && getTile(current.x, current.y + 1) >= UNEXPLORED)
    {
        int f = g + h(current.x, current.y + 1, goal.x, goal.y);
        InsertToSortedList(fringe, current.x, current.y + 1, f);
        if(getTile(current.x, current.y + 1) != GOAL) // GOAL Must supercede other QUEUED
        {
            setTile(current.x, current.y + 1, QUEUED);
        }
        // Set tile's predecessor to the current tile
        setPred(current.x, current.y + 1, current.x, current.y);
    }
    return;
}
