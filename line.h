#pragma once
#include "cardinal.h"

typedef struct
{
    int x;
    int y;
} coordinate; // An x-y tuple

typedef struct
{
    float m; // slope of the line
    float b; // y-intercept of the line
    int x; // One x-coordinate on the line (used for vertical lines for which slope-intercept doesn't work)
    // IMPORTANT: if x >= 0, the inLine algorithm will assume that the line is vertical with eq. x = some constant
    coordinate one;
    coordinate two;
} line; // each line is represented by its slope-intercept equation

/*
 * getSlope() - gets the slope of a line from two points on that line
 */
float getSlope(coordinate a, coordinate b)
{
    if (b.x - a.x == 0) return 0;
    return ((float)(b.y - a.y))/((float)(b.x - a.x));
}

/*
 * getYIntercept() - gets the y-intercept b of a line from a point and its slope
 */
float getYIntercept(coordinate a, float m)
{
    //y = mx + b => b = y-mx
    return (float)a.y-m*a.x;
}

/*
 * inLine() - determines whether a point (x,y) is in a line l
 */
bool inLine(coordinate a, line * l)
{
    if (l->x >= 0) // l.x is nonnegative, that is, l is a vertical line
    {
        if (a.x == l->x)
        {
            if ((l->one.y <= a.y && a.y <= l->two.y) || (l->two.y <= a.y && a.y <= l->one.y))
            return true;
        }
    }
    else
    {
        // (x,y) is in l if it satisfies the equation y = l.m*x + l.b
        float tY = l->m * (float)a.x + l->b;
        // also check x = (y-l.b)/(l.m)
        float tX = ((float)a.y - l->b)/l->m;
        if (((float)a.y == floor(tY) || (float)a.y == ceil(tY)) || ((float)a.x == floor(tX) || (float)a.x == ceil(tX)))
        //if (((float)y == floor(tY)) || ((float)x == floor(tX)))
        {
            if ((l->one.x <= a.x && a.x <= l->two.x) || (l->two.x <= a.x && a.x <= l->one.x))
            {
                if ((l->one.y <= a.y && a.y <= l->two.y) || (l->two.y <= a.y && a.y <= l->one.y))
                return true;
            }
        }
    }
    return false;
}
