#include "demoVec2.h"

void demoVec2::setup( ) {

    ofSetFrameRate( 60 );
    
    stringstream ss;
    ss << "Tweens, ofVec2f interpolations demo";
    ofSetWindowTitle( ss.str() );
    
    _tweener = new tween::Tweener( 100 );
    _tweener->addListener( this );
    
    vnum = (int) tween::TWEEN_INTERPOLATION_COUNT;
    vec2_values = new ofVec2f[ vnum ];
    tween_vec2_configs = new tween::TweenConfig<ofVec2f>[ vnum ];
    
    for ( int i = 0; i < vnum; ++i ) {
    
        int duration = 10 + ( i * 100 );
                
        vec2_values[ i ].set( 
            100 + ofRandomuf() *  ( ofGetWidth() - 200 ),
            100 + ofRandomuf() *  ( ofGetHeight() - 200 )
        );
        
        tween_vec2_configs[ i ].set(
            ofVec2f( 
                100 + ofRandomuf() *  ( ofGetWidth() - 200 ), 
                100 + ofRandomuf() *  ( ofGetHeight() - 200 )
            ),
            ofVec2f( 
                ( ofGetWidth() - 200 ), 
                ( ofGetHeight() - 200 )
            ),
            (tween::TweenInterpolation) i,
            1000,
            (int) ( 300 + ofRandomuf() * 2000 )
        );
        
        cout << 
            tween::TweenUtils::interpolationToString( (tween::TweenInterpolation) i ) << ", " <<
            tween::TweenUtils::interpolationToString( tween::TweenUtils::invertInterpolation( (tween::TweenInterpolation) i ) ) << 
            endl;
        
        tween_vec2_configs[ i ].setStartAtValue( true );
        _tweener->addTween( tween_vec2_configs[ i ], &vec2_values[ i ] );
        
    }
    
        
    lognum = 7;
    logs = new string[ lognum ];
    logi = 0;
    
    _tweener->start();
    
    allow_restart = false;
    
}

void demoVec2::update( ) {
    
    if ( allow_restart ) {

        addlog( "randomise targets & restarting" );
        for ( int i = 0; i < vnum; ++i ) {
            tween_vec2_configs[ i ].setMax(
                ofVec2f( 
                    100 + ofRandomuf() *  ( ofGetWidth() - 200 ), 
                    100 + ofRandomuf() *  ( ofGetHeight() - 200 )
                )
            );
            tween_vec2_configs[ i ].setDuration( (int) ( 300 + ofRandomuf() * 2000 ) );
            _tweener->addTween( tween_vec2_configs[ i ], &vec2_values[ i ] );
        }

        allow_restart = false;
    }
    
}

void demoVec2::draw( ) {

    int tx = 0;
    int ty = 0;
    
    ofBackground( 0, 0, 0 );
    
    ofNoFill();
    for ( int i = 0; i < vnum; ++i ) {
        ofSetColor( 255,0,0 );
        ofCircle( vec2_values[ i ].x, vec2_values[ i ].y, 10, 10 );
        ofSetColor( 255,255,255,50 );
        ofLine(
            tween_vec2_configs[ i ].min.x, tween_vec2_configs[ i ].min.y,
            vec2_values[ i ].x, vec2_values[ i ].y
        );
        ofLine(
            tween_vec2_configs[ i ].max.x, tween_vec2_configs[ i ].max.y,
            vec2_values[ i ].x, vec2_values[ i ].y
        );
        ofSetColor( 255,255,255 );
        ofDrawBitmapString( tween::TweenUtils::interpolationToString( tween_vec2_configs[ i ].interpolation ), vec2_values[ i ].x + 15, vec2_values[ i ].y + 5 );
    }
    
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

void demoVec2::keyPressed( int key ) {

    if ( key == 357 ) {
        
        _tweener->setTimeSpeed( _tweener->timespeed() + 0.02 );
        
        addlog( "increasing tweener speed" );
        
    } else if ( key == 359 ) {
        
        _tweener->setTimeSpeed( _tweener->timespeed() - 0.02 );
        
        addlog( "decreasing tweener speed" );
        
    } else if ( key == 'p' ) {
        
        _tweener->togglePlay();
        
        addlog( "toggle tweener play" );
        
    } else if ( key == 'k' ) {
        
        _tweener->killall();
        allow_restart = true;
        
        addlog( "killing all tweens" );
        
    } else if ( key == 'r' ) {
        
        if ( allow_restart ) {
            for ( int i = 0; i < vnum; ++i ) {
                _tweener->addTween( tween_vec2_configs[ i ], &vec2_values[ i ] );
            }
            allow_restart = false;
        }
        
        addlog( "restart all tweens" );
        
    } else {
        
        cout << key << ", " << (char) key << endl;
        
    }
    
}

void demoVec2::keyReleased( int key ) {}

void demoVec2::addlog( string s ) {
    
    ++logi;
    if ( logi >= lognum ) {
        for ( int i = 1; i < lognum; ++i ) {
            logs[ i - 1 ] = logs[ i ];
        }
        logi = lognum - 1;
    }
    logs[ logi ] = s;
    
}

void demoVec2::tweenEvent( tween::TweenEvent & evt ) {
    
    if ( evt.tweener != nullptr ) {
        stringstream ss;
        ss << evt.time << " >> tweener, type: " << tween::TweenUtils::tweenTypetoString( evt.type ) << ", " << evt.tweener;
        addlog( ss.str() );
        if ( evt.type == tween::TWEEN_EVENT_ALL_TWEENS_DONE ) {
            allow_restart = true;
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
