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

#include "common.h"
#include "mazegeneration.h"
#include "gamelogic.h"

static Window  *menu_window;
static Layer   *menu_layer;
static GBitmap *menu_image;

//menu drawing
static void menu_update_callback(Layer *me, GContext* ctx) {
    GRect bounds = menu_image->bounds;
    
    graphics_draw_bitmap_in_rect(ctx,
                                 menu_image,
                                 (GRect) { .origin = { 0, 0 }, .size = bounds.size });
}

//handle game start

static void menu_click_handler(ClickRecognizerRef recognizer, Window *window) {
    
    window_stack_push(game_window, true);
}

static void menu_config_provider(Window *window) {
    
    window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) menu_click_handler);
    
}

static void init(void) {
    
    //GAME WINDOW///////////////
    game_window = window_create();
    
    window_set_window_handlers(game_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload
    });
    
    //144 168
    window_set_fullscreen(game_window, true);
    window_set_background_color(game_window, GColorWhite);
    
    
    //MENU WINDOW///////////////
    menu_window = window_create();
    window_set_fullscreen(menu_window, true);
    window_stack_push(menu_window, true);
    window_set_background_color(menu_window, GColorWhite);
    
    Layer *window_layer = window_get_root_layer(menu_window);
    GRect bounds = layer_get_frame(window_layer);
    menu_layer = layer_create(bounds);
    layer_set_update_proc(menu_layer, menu_update_callback);
    layer_add_child(window_layer, menu_layer);
    
    menu_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MENU);
    
    window_set_click_config_provider(menu_window, (ClickConfigProvider) menu_config_provider);
}

static void deinit(void) {
    
    gbitmap_destroy(menu_image);
    
    window_destroy(menu_window);
    layer_destroy(menu_layer);
    
    window_destroy(game_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
