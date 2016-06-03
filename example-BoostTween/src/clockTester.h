#pragma once

#include <strstream>
#include "ofMain.h"
#include "Tweener/Tweener.h"
#include "Tweener/TweenClock.h"

class clockTester : public ofBaseApp, public tween::TweenListener {

public:
    
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    
    void tweenEvent( tween::TweenEvent & evt );
    
private:
    
    void addlog( string s );
    
    string * logs;
    int lognum;
    int logi;
    
    uint64_t totalcount;
    float minfps;
    float maxfps;
    
    tween::TweenClock * tween_precise_clock;
    
};
