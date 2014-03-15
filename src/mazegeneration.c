//
//  mazegeneration.c
//  GyroMaze
//
//  Created by Marcin Pędzimąż on 15.03.2014.
//  Copyright (c) 2014 inFullMobile sp. z o. o. All rights reserved.
//

#include "common.h"
#include "mazegeneration.h"

const int kMazeSize = 10;

//definitions
void init_maze(int maze[kMaxData][kMaxData])
{
    for(int a = 0; a < kMaxData; a++)
    {
        for(int b = 0; b < kMaxData; b++)
        {
            if(a % 2 == 0 || b % 2 == 0)
                maze[a][b] = 1;
            else
                maze[a][b] = PATH;
        }
    }
}

void maze_generator(int indeks, int maze[kMaxData][kMaxData], int backtrack_x[CELL], int backtrack_y[CELL], int x, int y, int n, int visited)
{
    if(visited < n * n)
    {
        int neighbour_valid = -1;
        int neighbour_x[4];
        int neighbour_y[4];
        int step[4];
        
        int x_next;
        int y_next;
        
        if(x - 2 > 0 && is_closed(maze, x - 2, y))  // upside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x - 2;;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=1;
        }
        
        if(y - 2 > 0 && is_closed(maze, x, y - 2))  // leftside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y - 2;
            step[neighbour_valid]=2;
        }
        
        if(y + 2 < n * 2 + 1 && is_closed(maze, x, y + 2))  // rightside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y + 2;
            step[neighbour_valid]=3;
            
        }
        
        if(x + 2 < n * 2 + 1 && is_closed(maze, x + 2, y))  // downside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x+2;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=4;
        }
        
        if(neighbour_valid == -1)
        {
            // backtrack
            x_next = backtrack_x[indeks];
            y_next = backtrack_y[indeks];
            indeks--;
        }
        
        if(neighbour_valid!=-1)
        {
            int randomization = neighbour_valid + 1;
            int random = rand()%randomization;
            x_next = neighbour_x[random];
            y_next = neighbour_y[random];
            indeks++;
            backtrack_x[indeks] = x_next;
            backtrack_y[indeks] = y_next;
            
            int rstep = step[random];
            
            if(rstep == 1)
                maze[x_next+1][y_next] = PATH;
            else if(rstep == 2)
                maze[x_next][y_next + 1] = PATH;
            else if(rstep == 3)
                maze[x_next][y_next - 1] = PATH;
            else if(rstep == 4)
                maze[x_next - 1][y_next] = PATH;
            visited++;
        }
        
        maze_generator(indeks, maze, backtrack_x, backtrack_y, x_next, y_next, n, visited);
    }
}

int is_closed(int maze[kMaxData][kMaxData], int x, int y)
{
    if(maze[x - 1][y]  == WALL
       && maze[x][y - 1] == WALL
       && maze[x][y + 1] == WALL
       && maze[x + 1][y] == WALL
       )
        return 1;
    
    return 0;
}
