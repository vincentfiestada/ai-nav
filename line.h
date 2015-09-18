#pragma once
#include "cardinal.h"

typedef struct
{
    float m; // slope of the line
    float b; // y-intercept of the line
    int x1;
    int x2;
    int y1;
    int y2;
} line; // each line is represented by its slope-intercept equation

/*
 * getSlope() - gets the slope of a line from two points on that line
 */
float getSlope(int x1, int y1, int x2, int y2)
{
    return ((float)(y2-y1))/((float)(x2-x1));
}

/*
 * getYIntercept() - gets the y-intercept b of a line from a point and its slope
 */
float getYIntercept(int x, int y, float m)
{
    //y = mx + b => b = y-mx
    return (float)y-m*x;
}

/*
 * inLine() - determines whether a point (x,y) is in a line l
 */
bool inLine(int x, int y, line * l)
{
    // (x,y) is in l if it satisfies the equation y = l.m*x + l.b
    float tY = l->m * (float)x + l->b;
    // also check x = (y-l.b)/(l.m)
    float tX = ((float)y - l->b)/l->m;
    //printf("tY: %f\n floor(tY):%f\n ceil(tY):%f", tY, floor(tY), ceil(tY));
    if (((float)y == floor(tY) || (float)y == ceil(tY)) || ((float)x == floor(tX) || (float)x == ceil(tX)))
    //if (((float)y == floor(tY)) || ((float)x == floor(tX)))
    {
        if ((l->x1 <= x && x <= l->x2) || (l->x2 <= x && x <= l->x1))
        {
            if ((l->y1 <= y && y <= l->y2) || (l->y2 <= y && y <= l->y1))
            return true;
        }
    }
    return false;
}
