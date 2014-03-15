//
//  mazegeneration.h
//  GyroMaze
//
//  Created by Marcin Pędzimąż on 15.03.2014.
//  Copyright (c) 2014 inFullMobile sp. z o. o. All rights reserved.
//

#ifndef GyroMaze_mazegeneration_h
#define GyroMaze_mazegeneration_h

//////////////////////////////////////////////////////////////////////
//PERFECT MAZE

#define kMaxData 21  // 10 * 2 + 1
#define CELL 100  // 10 * 10
#define WALL 1
#define PATH 0

extern const int kMazeSize;

//declaration
extern void init_maze(int maze[kMaxData][kMaxData]);
extern void maze_generator(int indeks, int maze[kMaxData][kMaxData], int backtrack_x[CELL], int bactrack_y[CELL], int x, int y, int n, int visited);
extern int is_closed(int maze[kMaxData][kMaxData], int x, int y);

#endif
