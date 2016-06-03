#pragma once

#include <strstream>
#include "ofMain.h"
#include "Tracer.h"
#include "Tweener/Tweener.h"

class demoRotation : public ofBaseApp, public tween::TweenListener {

public:
    
    void setup();
    void update();
    void draw();

    void mousePressed( int x, int y, int button );
    void mouseDragged( int x, int y, int button );
    void mouseReleased( int x, int y, int button );
    void keyPressed(int key);
    
    void tweenEvent( tween::TweenEvent & evt );
    
private:
    
    void addlog( string s );
    
    float thetaA;
    float thetaB;
    
    float theta_min;
    float theta_max;
    bool theta_min_grabbed;
    bool theta_max_grabbed;
    bool left_grab;
    
    tween::TweenConfig<float> tcA;
    tween::TweenConfig<float> tcB;
    bool restart;
    
    string * logs;
    int lognum;
    int logi;
    tween::Tweener * _tweener;
    
};
