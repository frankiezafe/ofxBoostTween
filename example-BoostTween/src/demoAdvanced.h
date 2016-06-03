#pragma once

#include <strstream>
#include "ofMain.h"
#include "Tweener/Tweener.h"

class demoAdvanced : public ofBaseApp, public tween::TweenListener {

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
    tween::Tweener * _tweener;
    
    ofQuaternion quaternion;
    ofMatrix4x4 mat;
    
};

// to specialise tweens, just overload the apply method
// see lines [5,26] in demoAdvanced.cpp for implementation
namespace tween {
    template <> void Tween_<ofQuaternion>::apply();
    template <> void Tween_<ofMatrix4x4>::apply();
}