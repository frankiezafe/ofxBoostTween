#pragma once

#include <strstream>
#include "ofMain.h"
#include "Tracer.h"
#include "Tweener/Tweener.h"

class demoStack : public ofBaseApp, public tween::TweenListener {

public:
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    
    void tweenEvent( tween::TweenEvent & evt );
    
private:
    
    void addlog( string s );
    
    float val1;
    float val2;
    float val3;
    
    tween::TweenConfig<float> conf1;
    tween::TweenConfig<float> conf2;
    tween::TweenConfig<float> conf_back;
    tween::TweenConfig<float> conf_always;
    
    string * logs;
    int lognum;
    int logi;
    tween::Tweener * _tweener;
    
};
