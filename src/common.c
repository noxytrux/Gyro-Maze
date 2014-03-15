//
//  common.c
//  GyroMaze
//
//  Created by Marcin Pędzimąż on 15.03.2014.
//  Copyright (c) 2014 inFullMobile sp. z o. o. All rights reserved.
//

#include "common.h"

//MATH FUNCTIONS //////////////////////////////////////////////////////

bool circlesColliding(int x1,int y1,int radius1, int x2,int y2,int radius2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    int radii = radius1 + radius2;
    
    if ( ( dx * dx )  + ( dy * dy ) < radii * radii )
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 Test collision between Circe and Line by solving the quadratic equation
 */

collisionside intersectlinecircle(Vec2d location, double radius, Vec2d lineFrom, Vec2d lineTo)
{
    float a = lineTo.x-lineFrom.x;
    float b = lineTo.y-lineFrom.y;
    float c = location.x-lineFrom.x;
    float d = location.y-lineFrom.y;
    float r = radius;
    
    collisionside retval = noneInside;
    
    if ((d*a - c*b)*(d*a - c*b) <= r*r*(a*a + b*b)) {
        
        if (c*c + d*d <= r*r) {
            retval |= startInside;
            
            if (retval & noneInside) {
                retval &= ~noneInside;
            }
        }
        
        if ((a-c)*(a-c) + (b-d)*(b-d) <= r*r) {
            retval |= endInside;
            
            if (retval & noneInside) {
                retval &= ~noneInside;
            }
        }
        
        if (c*a + d*b >= 0 && c*a + d*b <= a*a + b*b) {
            retval |= middleInside;
            
            if (retval & noneInside) {
                retval &= ~noneInside;
            }

        }
    }
        
    return retval;
}
