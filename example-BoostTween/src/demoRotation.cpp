#include "demoRotation.h"

void demoRotation::setup( ) {

    ofSetFrameRate( 60 );
    
    stringstream ss;
    ss << "Tweens, rotation interpolations demo";
    ofSetWindowTitle( ss.str() );
    
    _tweener = new tween::Tweener( 100 );
    _tweener->addListener( this );
    
    thetaA = M_PI * -0.5;
    thetaB = M_PI * -0.5;
    
    theta_min = -M_PI * 0.75;
    theta_max = M_PI * 0.75;
    
    tcA.set( theta_min, theta_max, tween::TWEEN_INTERPOLATION_BACK, 200, 1000 );
    tcA.setStartAtValue( true );
    tcA.repeat( 1, true );
    tcB = tcA;
    tcB.setRotation( true );
        
    _tweener->addTween( tcA, &thetaA );
    _tweener->addTween( tcB, &thetaB );
    
    lognum = 7;
    logs = new string[ lognum ];
    logi = 0;
    
    ofSetCircleResolution( 100 );
    
    _tweener->start();
    
    theta_min_grabbed = false;
    theta_max_grabbed = false;
    left_grab = false;
    restart = false;
        
}

void demoRotation::update( ) {
    
    if ( restart ) {
        tcA.setMin( theta_min );
        tcA.setMax( theta_max );
        tcB.setMin( theta_min );
        tcB.setMax( theta_max );
        _tweener->addTween( tcA, &thetaA );
        _tweener->addTween( tcB, &thetaB );
        restart = false;
    }
    
}

void demoRotation::draw( ) {

    int tx = 0;
    int ty = 0;
    
    ofBackground( 0, 0, 0 );
    
    ofNoFill();
    ofSetColor( 255,255,255,120 );
    
    float cx = ofGetWidth() * 1 / 3.f;
    float cy = ofGetHeight() * 0.5f;
    float r = ofGetWidth() * 0.125f;
    if ( r > ofGetHeight() * 0.4f ) {
        r = ofGetHeight() * 0.4f;
    }
    
    ofCircle( cx, cy, r );
    ofCircle( cx * 2, cy, r );
    ofDrawBitmapString( "normal tweener", cx, cy );
    ofDrawBitmapString( "rotation tweener", cx * 2, cy );
    
    
    ofNoFill();
    float cos_min_tcA = cos( tcA.min );
    float sin_min_tcA = sin( tcA.min );
    float cos_max_tcA = cos( tcA.max );
    float sin_max_tcA = sin( tcA.max );
    
    ofFill();
    ofSetColor( 255,0,255 );
    ofCircle( cx + cos_min_tcA * r, cy + sin_min_tcA * r, 3 );
    ofCircle( cx * 2 + cos_min_tcA * r, cy + sin_min_tcA * r, 3 );
    ofCircle( cx + cos_max_tcA * r, cy + sin_max_tcA * r, 3 );
    ofCircle( cx * 2 + cos_max_tcA * r, cy + sin_max_tcA * r, 3 );
    
    float cosA = cos( thetaA );
    float sinA = sin( thetaA );
    float cosB = cos( thetaB );
    float sinB = sin( thetaB );
    
    float cos1 = cos( theta_min );
    float sin1 = sin( theta_min );
    float cos2 = cos( theta_max );
    float sin2 = sin( theta_max );
    
    ofSetColor( 255,0,0 );
    ofLine(
        cx, cy,
        cx + cosA * r, 
        cy + sinA * r
    );
    ofLine(
        cx * 2, cy,
        cx * 2 + cosB * r, 
        cy + sinB * r
    );
    
    
    if ( theta_min_grabbed ) {
        ofFill();
    } else {
        ofNoFill();
    }
    ofSetColor( 255,0,0 );
    ofCircle( cx + cos1 * r, cy + sin1 * r, 5 );
    ofCircle( cx * 2 + cos1 * r, cy + sin1 * r, 5 );
    ofSetColor( 255,255,255 );
    ofDrawBitmapString( "start " + ofToString( ofRadToDeg( theta_min ) ), 10 + cx + cos1 * r, 4 + cy + sin1 * r );
    ofDrawBitmapString( "start " + ofToString( ofRadToDeg( theta_min ) ), 10 + cx * 2 + cos1 * r, 4 + cy + sin1 * r );
    
    if ( theta_max_grabbed ) {
        ofFill();
    } else {
        ofNoFill();
    }
    ofSetColor( 255,0,0 );
    ofCircle( cx + cos2 * r, cy + sin2 * r, 5 );
    ofCircle( cx * 2 + cos2 * r, cy + sin2 * r, 5 );
    ofSetColor( 255,255,255 );
    ofDrawBitmapString( "end " + ofToString( ofRadToDeg( theta_max ) ), 10 + cx + cos2 * r, 4 + cy + sin2 * r );
    ofDrawBitmapString( "end " + ofToString( ofRadToDeg( theta_max ) ), 10 + cx * 2 + cos2 * r, 4 + cy + sin2 * r );
    
    
    ofSetColor( 255,255,255 );
    ty = 25;
    ofDrawBitmapString( "tweener fps: " + ofToString( _tweener->fps() ), 10, ty ); ty += 15;
    ofDrawBitmapString( "tweener time speed: " + ofToString( _tweener->timespeed() ), 10, ty ); ty += 15;
    ofDrawBitmapString( "tweener inner time: " + ofToString( _tweener->innertime() ), 10, ty ); ty += 15;
    if ( _tweener->running() ) {
        ofDrawBitmapString( "tweener is playing", 10, ty ); ty += 15;
    } else {
        ofDrawBitmapString( "tweener is paused", 10, ty ); ty += 15;
    }
    ofDrawBitmapString( "millis: " + ofToString( ofGetElapsedTimeMillis() ), 10, ty ); ty += 15;
    ofDrawBitmapString( "of fps: " + ofToString( ofGetFrameRate() ), 10, ty ); ty += 15;
    
    ofSetColor( 255,255,100 );
    stringstream ss;
    for ( int i = 1; i < lognum; ++i ) {
        ss << logs[ i ] << endl;
    }
    ofDrawBitmapString( ss.str(), 300, 25 );
    
}

void demoRotation::mousePressed( int x, int y, int button ) {
    
    float cx = ofGetWidth() * 1 / 3.f;
    float cy = ofGetHeight() * 0.5f;
    float r = ofGetWidth() * 0.125f;
    if ( r > ofGetHeight() * 0.4f ) {
        r = ofGetHeight() * 0.4f;
    }
    float cos1 = cos( theta_min );
    float sin1 = sin( theta_min );
    float cos2 = cos( theta_max );
    float sin2 = sin( theta_max );
    
    if ( ofDist( x, y, 0, cx * 2 + cos1 * r, cy + sin1 * r, 0 ) <= 5 ) {
        theta_min_grabbed = true;
    } else if ( ofDist( x, y, 0, cx + cos1 * r, cy + sin1 * r, 0 ) <= 5 ) {
        theta_min_grabbed = true;
        left_grab = true;
    } else if ( ofDist( x, y, 0, cx * 2 + cos2 * r, cy + sin2 * r, 0 ) <= 5 ) {
        theta_max_grabbed = true;
    } else if ( ofDist( x, y, 0, cx + cos2 * r, cy + sin2 * r, 0 ) <= 5 ) {
        theta_max_grabbed = true;
        left_grab = true;
    }
    
}

void demoRotation::mouseDragged( int x, int y, int button ) {

    if ( theta_min_grabbed ) {
        float cx = ofGetWidth() * 1 / 3.f;
        float cy = ofGetHeight() * 0.5f;
        if ( !left_grab ) {
            cx *= 2;
        }
        theta_min = atan2( y - cy, x - cx );
    } else if ( theta_max_grabbed ) {
        float cx = ofGetWidth() * 1 / 3.f;
        float cy = ofGetHeight() * 0.5f;
        if ( !left_grab ) {
            cx *= 2;
        }
        theta_max = atan2( y - cy, x - cx );
    }
    
}

void demoRotation::mouseReleased( int x, int y, int button ) {

    left_grab = false;
    theta_min_grabbed = false;
    theta_max_grabbed = false;
    
}

void demoRotation::keyPressed( int key ) {

    if ( key == 357 ) {
        
        _tweener->setTimeSpeed( _tweener->timespeed() + 0.02 );
        
        addlog( "increasing tweener speed" );
        
    } else if ( key == 359 ) {
        
        _tweener->setTimeSpeed( _tweener->timespeed() - 0.02 );
        
        addlog( "decreasing tweener speed" );
        
    } else if ( key == 'p' ) {
        
        _tweener->togglePlay();
        
        addlog( "toggle tweener play" );
        
    } else {
        
        cout << key << ", " << (char) key << endl;
        
    }
    
}

void demoRotation::addlog( string s ) {
    
    ++logi;
    if ( logi >= lognum ) {
        for ( int i = 1; i < lognum; ++i ) {
            logs[ i - 1 ] = logs[ i ];
        }
        logi = lognum - 1;
    }
    logs[ logi ] = s;
    
}

void demoRotation::tweenEvent( tween::TweenEvent & evt ) {
    
    if ( evt.tweener != nullptr ) {
        stringstream ss;
        ss << evt.time << " >> tweener, type: " << tween::TweenUtils::tweenTypetoString( evt.type ) << ", " << evt.tweener;
        addlog( ss.str() );
        if ( evt.type == tween::TWEEN_EVENT_ALL_TWEENS_DONE ) {
            restart = true;
        }
    }
    
    if ( evt.tween != nullptr ) {
        tween::Tween * tw = (tween::Tween *) evt.tween;
        if( 
            evt.type == tween::TWEEN_EVENT_START || 
            evt.type == tween::TWEEN_EVENT_STOP || 
            evt.type == tween::TWEEN_EVENT_LOOP
        )
        {
//            stringstream ss;
//            ss << evt.time << " > tween " << tw << ", count: " << evt.counter << ", type: " << tweenTypetoString( evt.type ) << ", value: " << tw->get_target_ptr();
//            addlog( ss.str() );
        }
    }
    
};
