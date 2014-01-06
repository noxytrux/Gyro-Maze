//
//  GyroMaze.c
//  GyroMaze
//
//  Created by Marcin Pędzimąż on 02.12.2013.
//  Copyright (c) 2013 Marcin Pędzimąż. All rights reserved.
//
//  This is simple Maze Game done for Pebble Competition
//  i tried to do it fun :) but it was hard to achieve good physic at all ;/
//  But overfall im happy i made it and i hope you will enjoy it.


#include "pebble.h"

#define MAX(a,b) ( (a) > (b) ? (a) : (b) )
#define MIN(a,b) ( (a) < (b) ? (a) : (b) )
#define ABS( a ) ( (a) < 0 ? (-a) : (a) )

const int kMaxSQRTSteps = 40;
const float kMathPi = 3.14159265358979323846264338327950288;
const int kFrameRate = 20; //Miliseconds
const float kDensity = 0.25;
const int kMoveVal = 1;
const int kAnimationSpeed = 150;

char timeBuff[6] = {"00:00"};

//simple 2D vector implementation
typedef struct Vec2d {
    float x;
    float y;
} Vec2d;

//structure that descibes our BALL
typedef struct Disc {
    Vec2d pos;
    Vec2d vel;
    float radius;
} Disc;

static Disc ball;

//////////////////////////////////////////////////////////////////////
//PERFECT MAZE

#define kMaxData 21  // 10 * 2 + 1
#define CELL 100  // 10 * 10
#define WALL 1
#define PATH 0

const int kMazeSize = 10;

int mazeData[kMaxData][kMaxData];

float rect_w = 0;
float rect_h = 0;
float rect_radius = 0;

Vec2d key_pos;
Vec2d exit_pos;

int frames = 0;

#define STATE_FIND_KEY 1
#define STATE_FIND_EXIT 2
#define STATE_END 3

int state = STATE_FIND_KEY;

int timeSum = 0;

const float kOffsetY = 26;
const float kOffsetX = 2;

//declaration
static void init_maze(int maze[kMaxData][kMaxData]);
static void maze_generator(int indeks, int maze[kMaxData][kMaxData], int backtrack_x[CELL], int bactrack_y[CELL], int x, int y, int n, int visited);
static int is_closed(int maze[kMaxData][kMaxData], int x, int y);
static void draw_maze(GContext *ctx, int maze[kMaxData][kMaxData], int maze_size);

static void restart(void);

static void make_maze(void)
{
    srand((unsigned)time(NULL));
    
    int size = kMazeSize;
    int indeks = 0;
    
    int backtrack_x[CELL];
    int backtrack_y[CELL];
    
    init_maze(mazeData);
    
    backtrack_x[indeks] = 1;
    backtrack_y[indeks] = 1;
    
    maze_generator(indeks, mazeData, backtrack_x, backtrack_y, 1, 1, size, 1);
}

//definitions
static void init_maze(int maze[kMaxData][kMaxData])
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

static void maze_generator(int indeks, int maze[kMaxData][kMaxData], int backtrack_x[CELL], int backtrack_y[CELL], int x, int y, int n, int visited)
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

static int is_closed(int maze[kMaxData][kMaxData], int x, int y)
{
    if(maze[x - 1][y]  == WALL
       && maze[x][y - 1] == WALL
       && maze[x][y + 1] == WALL
       && maze[x + 1][y] == WALL
       )
        return 1;
    
    return 0;
}

//////////////////////////////////////////////////////////////////////

static Window *window;
static GRect window_frame;
static Layer *mainLayer;
static AppTimer *timer;

static TextLayer *text_layer;
static TextLayer *text_time_layer;

static TextLayer *finish_text;

int finishAnimationX = 0;
int finishAnimSize = 2;
bool bonceBack = false;
bool animDone = false;

//MATH FUNCTIONS //////////////////////////////////////////////////////

static bool circlesColliding(int x1,int y1,int radius1, int x2,int y2,int radius2)
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

static int intersectlinecircle(Vec2d location, double radius, Vec2d lineFrom, Vec2d lineTo)
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

//SETUP CODE //////////////////////////////////////////////////////////

static void disc_init(Disc *disc) {
    srand(time(NULL));
    
    disc->pos.x = (kMazeSize * 2 - 1) * rect_w;
    disc->pos.y = (kMazeSize * 2 + 1) * rect_h;
    disc->vel.x = 0.0f;
    disc->vel.y = 0.0f;
    disc->radius = 3.0;
    
    key_pos.x = kOffsetX + ((( (rand() % (kMazeSize * 2 - 1)) + 1) * rect_w) + (rect_w*0.5));
    key_pos.y = kOffsetY + ((( (rand() % (kMazeSize * 2 - 1)) + 1) * rect_h) + (rect_h*0.5));
    
    exit_pos.x = kOffsetX + ((( (rand() % (kMazeSize * 2 - 1)) + 1) * rect_w) + (rect_w*0.5));
    exit_pos.y = kOffsetY + ((( (rand() % (kMazeSize * 2 - 1)) + 1) * rect_h) + (rect_h*0.5));
}

//LOGIC //////////////////////////////////////////////////////////////

static void disc_apply_force(Disc *disc, Vec2d force) {
    disc->vel.x = force.x;
    disc->vel.y = force.y;
}

static void disc_apply_accel(Disc *disc, AccelData accel) {
    Vec2d force;
    
    force.x = ( accel.x > 0 ? kMoveVal : -kMoveVal );
    force.y = (-accel.y > 0 ? kMoveVal : -kMoveVal );
    
    disc_apply_force(disc, force);
}

/*
    This is man of this house calculates our collisons and add physic"
 */

static void disc_update(Disc *disc) {
    
    int val = 1;
    
    int mx = kMazeSize * 2 + 1;
    int my = kMazeSize * 2 + 1;

    disc->pos.x += disc->vel.x;
    disc->pos.y += disc->vel.y;

    Vec2d pos = disc->pos;
    float radius = disc->radius+1;
    
    for(int i = 0; i < mx; i++)
    {
        for(int j = 0; j < my; j++)
        {
            if(mazeData[i][j] == WALL) {
                
                if (i == (mx-1)) {
                    
                    if(j < my){
                        
                        if (intersectlinecircle(pos,
                                                radius,
                                                (Vec2d){kOffsetX + i*rect_w, kOffsetY + j*rect_h},
                                                (Vec2d){kOffsetX + i*rect_w, kOffsetY + (j+1)*rect_h})) {
                          
                            disc->pos.x += -disc->vel.x*val;
                            //disc->vel.x = 0;
                        }
                        
                    }
                    
                    continue;
                }
                
                if (j == (my-1)) {
                    
                    if (i < mx) {
                        
                        if (intersectlinecircle(pos,
                                                radius,
                                                (Vec2d){kOffsetX + i*rect_w, kOffsetY + j*rect_h},
                                                (Vec2d){kOffsetX + (i+1)*rect_w, kOffsetY + j*rect_h})) {
                            
                            disc->pos.y += -disc->vel.y*val;
                            //disc->vel.y = 0;
                        }
                        
                    }
                    
                    continue;
                }
                
                if (mazeData[i+1][j] == WALL) {
                    
                    if (intersectlinecircle(pos,
                                            radius,
                                            (Vec2d){kOffsetX + i*rect_w, kOffsetY + j*rect_h},
                                            (Vec2d){kOffsetX + (i+1)*rect_w, kOffsetY + j*rect_h})) {
                       
                        disc->pos.y += -disc->vel.y*val;
                        //disc->vel.y = 0;
                    }
                    
                }
                
                if (mazeData[i][j+1] == WALL) {
                    
                    if (intersectlinecircle(pos,
                                            radius,
                                            (Vec2d){kOffsetX + i*rect_w, kOffsetY + j*rect_h},
                                            (Vec2d){kOffsetX + i*rect_w, kOffsetY + (j+1)*rect_h})) {
                        
                        disc->pos.x += -disc->vel.x*val;
                        //disc->vel.x = 0;
                    }
                    
                }
                
            }
        }
    }
    
    if(state == STATE_FIND_KEY){
        if(circlesColliding(pos.x, pos.y, radius, key_pos.x, key_pos.y, 3)) {
            
            state = STATE_FIND_EXIT;
            text_layer_set_text(text_layer, "Go to Exit!");
            
            vibes_short_pulse();
            
        }
    }
    else if(state == STATE_FIND_EXIT){
       
        if(circlesColliding(pos.x, pos.y, radius, exit_pos.x, exit_pos.y, 3)) {
            
            state = STATE_END;
            
            text_layer_set_text(text_layer, "You did it :)");
            
            vibes_long_pulse();
            
        }
    }
    
}

//DRAWING //////////////////////////////////////////////////////////////

static void draw_maze(GContext *ctx, int maze[kMaxData][kMaxData], int maze_size)
{
    int mx = maze_size * 2 + 1;
    int my = maze_size * 2 + 1;
    
    for(int i = 0; i < mx; i++)
    {
        for(int j = 0; j < my; j++)
        {
            if(maze[i][j] == WALL) {
                
                if (i == (mx-1)) {
                    
                    if(j < my){
                        graphics_draw_line(ctx, GPoint(kOffsetX + i*rect_w, kOffsetY + j*rect_h), GPoint(kOffsetX + i*rect_w, kOffsetY + (j+1)*rect_h));
                    }
                    
                    continue;
                }
                
                if (j == (my-1)) {
                    
                    if (i < mx) {
                        graphics_draw_line(ctx, GPoint(kOffsetX + i*rect_w, kOffsetY + j*rect_h), GPoint(kOffsetX + (i+1)*rect_w, kOffsetY + j*rect_h));
                    }
                    
                    continue;
                }
                
                if (maze[i+1][j] == WALL) {
                    
                    graphics_draw_line(ctx, GPoint(kOffsetX + i*rect_w, kOffsetY + j*rect_h), GPoint(kOffsetX + (i+1)*rect_w, kOffsetY + j*rect_h));
                    
                }
                
                if (maze[i][j+1] == WALL) {
                    
                    graphics_draw_line(ctx, GPoint(kOffsetX + i*rect_w, kOffsetY + j*rect_h), GPoint(kOffsetX + i*rect_w, kOffsetY + (j+1)*rect_h));
                
                }
                
            }
        }
    }
}


//FRAME UPDATE /////////////////////////////////////////////////////////

static void disc_layer_update_callback(Layer *me, GContext *ctx)
{
    
    graphics_context_set_fill_color(ctx, GColorBlack);
    draw_maze(ctx, mazeData, kMazeSize);
    
    graphics_fill_circle(ctx, GPoint(ball.pos.x, ball.pos.y), ball.radius);

    frames++;
    
    if (frames == 10000) {
        frames = 0;
    }
    
    if (state == STATE_FIND_KEY) {
        static bool grow = true;
        static uint16_t keySize = 1;
        
        if (grow) {
            
            if (frames % kAnimationSpeed) {
                keySize++;
            
                if (keySize == 5) {
                    grow = false;
                }
            }
        }
        else{
        
            if (frames % kAnimationSpeed) {
                keySize--;
                
                if (keySize == 1) {
                    grow = true;
                }
            }
        }
        
        graphics_fill_circle(ctx, GPoint(key_pos.x, key_pos.y), keySize);
        
    }
    else if(state == STATE_FIND_EXIT){
        
        static bool growexit = true;
        static uint16_t exitSize = 1;
        
        if (growexit) {
            
            if (frames % 100) {
                exitSize++;
                
                if (exitSize == 5) {
                    growexit = false;
                }
            }
        }
        else{
            
            if (frames % 100) {
                exitSize--;
                
                if (exitSize == 1) {
                    growexit = true;
                }
            }
        }
        
        graphics_draw_circle(ctx, GPoint(exit_pos.x, exit_pos.y), exitSize);
    }
    else if(state == STATE_END){
        
        if (!animDone) {
            
            if (bonceBack) {
                finishAnimationX --;
                if (finishAnimationX == 22) {
                    animDone = true;
                    timer = NULL;
                    
                    layer_add_child(mainLayer, text_layer_get_layer(finish_text));
                }
            }
            else{
                finishAnimationX ++;
                
                if (finishAnimationX > 40) {
                    bonceBack = true;
                }
            }
        }
        
        if (finishAnimSize < 100) {
            finishAnimSize+=2;
        }
        
        graphics_fill_rect(ctx, GRect(finishAnimationX, 72, finishAnimSize, 24), 5, GCornersAll);
    }
    
}

static void timer_callback(void *data)
{
    if (state != STATE_END) {
        AccelData accel = { 0, 0, 0 };
        
        accel_service_peek(&accel);
        
        Disc *disc = &ball;
        disc_apply_accel(disc, accel);
        disc_update(disc);
        
        timeSum += kFrameRate;
        
        //update timer buffer
        
        int sec = (timeSum / 1000) % 60;
        int min = (timeSum / 1000) / 60;
        
        int a = sec / 10;
        int b = sec % 10;
        
        int c = min / 10;
        int d = min % 10;
        
        timeBuff[3] = 48 + a;
        timeBuff[4] = 48 + b;
        timeBuff[0] = 48 + c;
        timeBuff[1] = 48 + d;
        
        text_layer_set_text(text_time_layer, timeBuff);
    }
    
    layer_mark_dirty(mainLayer);
    
    timer = app_timer_register(kFrameRate, timer_callback, NULL);
}

void restart(void)
{
    layer_remove_from_parent(text_layer_get_layer(finish_text));
    
    light_enable(true);
    
    finishAnimationX = 0;
    bonceBack = false;
    animDone = false;
    finishAnimSize = 2;
    
    state = STATE_FIND_KEY;
    
    disc_init(&ball);
    make_maze();
    timeSum = 0;
    
    timeBuff[0] = 48;
    timeBuff[1] = 48;
    timeBuff[3] = 48;
    timeBuff[4] = 48;

    text_layer_set_text(text_time_layer, timeBuff);
}

//COMMON ///////////////////////////////////////////////////

static void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
   
    if(state == STATE_END){
        restart();
    }

}

static void handle_accel(AccelData *accel_data, uint32_t num_samples) {
    // do nothing
}

static void config_provider(Window *window) {
    
    window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_single_click_handler);

}

static void window_load(Window *window) {
    
    Layer *window_layer = window_get_root_layer(window);
    GRect frame = window_frame = layer_get_frame(window_layer);
    
    rect_w = (140/(kMazeSize * 2));
    rect_h = (140/(kMazeSize * 2));
    
    rect_radius = MIN(rect_w,rect_h) * 0.5;
    
    state = STATE_FIND_KEY;
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "frame: %d %d %d %d", frame.origin.x, frame.origin.y, frame.size.w, frame.size.h);
    
    mainLayer = layer_create(frame);
    layer_set_update_proc(mainLayer, disc_layer_update_callback);
    
    text_layer = text_layer_create(GRect(5, 0, 70, 24));
    text_layer_set_text(text_layer, "Find Key!");
    text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
    
    text_time_layer = text_layer_create(GRect(80, 0, 60, 24));
    text_layer_set_text(text_time_layer, timeBuff);
    text_layer_set_text_alignment(text_time_layer, GTextAlignmentRight);
    
    layer_add_child(mainLayer, text_layer_get_layer(text_layer));
    layer_add_child(mainLayer, text_layer_get_layer(text_time_layer));
    
    finish_text  = text_layer_create(GRect(22, 74, 100, 24));
    text_layer_set_text(finish_text, "AGAIN? >>>");
    text_layer_set_text_alignment(finish_text, GTextAlignmentCenter);
    text_layer_set_text_color(finish_text, GColorWhite);
    text_layer_set_background_color(finish_text, GColorBlack);

    layer_add_child(window_layer, mainLayer);
    
    light_enable(true);
    
    disc_init(&ball);
    
}

static void window_unload(Window *window) {
    
    light_enable(false);
    
    text_layer_destroy(text_layer);
    text_layer_destroy(text_time_layer);
    
    text_layer_destroy(finish_text);
    
    layer_destroy(mainLayer);
}

static void init(void) {
    window = window_create();
    
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload
    });
    
    //144 168
    window_set_fullscreen(window, true);
    window_stack_push(window, true);
    window_set_background_color(window, GColorWhite);
    
    accel_data_service_subscribe(0, handle_accel);
    
    window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
    
    make_maze();

    timeSum = 0;
    timer = app_timer_register(kFrameRate, timer_callback, NULL);
}

static void deinit(void) {
    accel_data_service_unsubscribe();
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
