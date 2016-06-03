#pragma once

#include <strstream>
#include "ofMain.h"
#include "Tracer.h"
#include "Tweener/Tweener.h"

class demoCallback : public ofBaseApp, public tween::TweenListener {

public:
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    
    void tweenEvent( tween::TweenEvent & evt );
    
    void callback();
    void callback_value( double i );
    
private:
    
    void addlog( string s );
    
    tween::Callback * cb;
    tween::CallbackValue * cbv;
    
    string * logs;
    int lognum;
    int logi;
    tween::Tweener * _tweener;
    
};
