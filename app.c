#include "cardinal.h"
#include "line.h"

#define H 40
#define W 40

#define BLOCKED 1
#define UNEXPLORED 2
#define EXPLORED 3

// Global variables
int grid[H][W];

int main()
{
    // Declare iterators
    unsigned int i,j,k;

    // Create grid
    for (i = 0; i < H; i++)
    {
        for (j = 0; j < W; j++)
        {
            grid[i][j] = BLOCKED;
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
        int nv[2]; // Get next vertex
        if (i < 3 - 1)
        {
            nv[0] = vertices[i+1][0];
            nv[1] = vertices[i+1][1];
        }
        else
        {
            nv[0] = vertices[0][0];
            nv[1] = vertices[0][1];
        }
        e[i].m = getSlope(vertices[i][0], vertices[i][1], nv[0], nv[1]);
        e[i].b = getYIntercept(vertices[i][0], vertices[i][1], e[i].m);
        e[i].x1 = vertices[i][0];
        e[i].y1 = vertices[i][1];
        e[i].x2 = nv[0];
        e[i].y2 = nv[1];
    }



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
            for (k = 0; k < 3; k++)
            {
                if (inLine(j, i, &e[k]))
                {
                    grid[i][j] = BLOCKED;
                    printf("!@!");
                    break;
                }
                else if (k == 3 - 1)
                {
                    printf("   ");
                }
            }
        }
        printf("\n");
    }

    return 0;
}
