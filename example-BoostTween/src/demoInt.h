#pragma once

#include <strstream>
#include "ofMain.h"
#include "Tracer.h"
#include "Tweener/Tweener.h"

#define value_type int

class demoInt : public ofBaseApp, public tween::TweenListener {

public:
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    
    void tweenEvent( tween::TweenEvent & evt );
    
private:
    
    void addlog( string s );
    
    bool fps30;
    
    int interpolation_count;
    value_type * values;
    Tracer<value_type> * tracers;
    tween::TweenConfig<value_type> * tween_int_configs;
    bool allow_restart;
    
    string * logs;
    int lognum;
    int logi;
    tween::Tweener * _tweener;
    
};
