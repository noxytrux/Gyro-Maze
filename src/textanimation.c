//
//  textanimation.c
//  GyroMaze
//
//  Created by Marcin Pędzimąż on 15.03.2014.
//  Copyright (c) 2014 inFullMobile sp. z o. o. All rights reserved.
//

#include "textanimation.h"

PropertyAnimation *prop_animation;
TextLayer *finish_text;

void animation_proceed(void) {
    
    Layer *layer = text_layer_get_layer(finish_text);
    
    GRect middle_rect = GRect(50, 74, 100, 24);
    
    destroy_property_animation(&prop_animation);
    
    prop_animation = property_animation_create_layer_frame(layer, NULL, &middle_rect);
    animation_set_duration((Animation*) prop_animation, 500);
    animation_set_curve((Animation*) prop_animation, AnimationCurveEaseInOut);
    
    animation_set_handlers((Animation*) prop_animation, (AnimationHandlers) {
        .started = (AnimationStartedHandler) animation_started,
        .stopped = (AnimationStoppedHandler) animation_stopped,
    }, NULL /* callback data */);
    
    animation_schedule((Animation*) prop_animation);
}

void animation_reset(void) {
    
    Layer *layer = text_layer_get_layer(finish_text);
    
    GRect start_frame = GRect(-40, 74, 0, 24);
    
    prop_animation = property_animation_create_layer_frame(layer, NULL, &start_frame);
    animation_schedule((Animation*) prop_animation);
}

void animation_started(Animation *animation, void *data) {
    
}

void animation_stopped(Animation *animation, bool finished, void *data) {

    Layer *layer = text_layer_get_layer(finish_text);
    
    GRect end_rect = GRect(22, 74, 100, 24);
    
    destroy_property_animation(&prop_animation);
    
    prop_animation = property_animation_create_layer_frame(layer, NULL, &end_rect);
    animation_set_duration((Animation*) prop_animation, 350);
    animation_set_curve((Animation*) prop_animation, AnimationCurveEaseInOut);
    
    animation_schedule((Animation*) prop_animation);
}

void destroy_property_animation(PropertyAnimation **prop_animation) {
    if (*prop_animation == NULL) {
        return;
    }
    
    if (animation_is_scheduled((Animation*) *prop_animation)) {
        animation_unschedule((Animation*) *prop_animation);
    }
    
    property_animation_destroy(*prop_animation);
    *prop_animation = NULL;
}