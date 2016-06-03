#pragma once

#include <strstream>
#include "ofMain.h"
#include "Tracer.h"
#include "Tweener/Tweener.h"

class demoVec2 : public ofBaseApp, public tween::TweenListener {

public:
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    
    void tweenEvent( tween::TweenEvent & evt );
    
private:
    
    void addlog( string s );
    
    int vnum;
    ofVec2f * vec2_values;
    tween::TweenConfig<ofVec2f> * tween_vec2_configs;
    bool allow_restart;
    
    string * logs;
    int lognum;
    int logi;
    tween::Tweener * _tweener;
    
};
