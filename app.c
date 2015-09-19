#include "cardinal.h"
#include "polygon.h"

#define H 40
#define W 40

// Tile states
#define BLOCKED 1
#define UNEXPLORED 2
#define EXPLORED 3
#define CURRENT 4
#define GOAL 5

// Global variables
int grid[H][W];
polygon p;

void setTile(unsigned int x, unsigned int y, unsigned int s);
void drawGrid();

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

    drawGrid();

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
                    printf(" . ");
                    break;
                case BLOCKED:
                    printf("!@!");
                    break;
                case CURRENT:
                    printf(":) ");
                    break;
                default:
                    printf("   ");
            }
        }
        printf("\n");
    }
}
