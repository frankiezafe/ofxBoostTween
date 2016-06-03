#pragma once

#include <strstream>
#include "ofMain.h"
#include "Tracer.h"
#include "Tweener/Tweener.h"

class demoBoolString : public ofBaseApp, public tween::TweenListener {

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
    bool * bvalues;
    std::string * svalues;
    tween::TweenConfig<bool> * tween_bool_configs;
    tween::TweenConfig<std::string> * tween_string_configs;
    bool allow_restart;
    
    string * logs;
    int lognum;
    int logi;
    tween::Tweener * _tweener;
    
};
