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

int intersectlinecircle(Vec2d location, double radius, Vec2d lineFrom, Vec2d lineTo)
{
    float a = lineTo.x-lineFrom.x;
    float b = lineTo.y-lineFrom.y;
    float c = location.x-lineFrom.x;
    float d = location.y-lineFrom.y;
    float r = radius;
    
    bool startInside = false;
    bool endInside = false;
    bool middleInside = false;
    
    if ((d*a - c*b)*(d*a - c*b) <= r*r*(a*a + b*b)) {
        
        if (c*c + d*d <= r*r) {
            startInside = true;
        }
        
        if ((a-c)*(a-c) + (b-d)*(b-d) <= r*r) {
            endInside = true;
        }
        
        if (!startInside && !endInside && c*a + d*b >= 0 && c*a + d*b <= a*a + b*b) {
            middleInside = true;
        }
    }
    
    return (startInside || endInside || middleInside);
}
