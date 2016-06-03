#include "demoInt.h"

void demoInt::setup( ) {

    ofSetFrameRate( 60 );
    
    stringstream ss;
    ss << "Tweens, \"" << typeid( value_type ).name() << "\" interpolations demo";
    ofSetWindowTitle( ss.str() );
    
    _tweener = new tween::Tweener( 100 );
    _tweener->addListener( this );
    
    interpolation_count = (int) tween::TWEEN_INTERPOLATION_COUNT;
    
    lognum = 4;
    logs = new string[ lognum ];
    logi = 0;
    
    values = new value_type[ interpolation_count ];
    tracers = new Tracer<value_type>[ interpolation_count ];
    tween_int_configs = new  tween::TweenConfig<value_type>[ interpolation_count ];
    tween::TweenConfig<value_type> conf( 20, 300, tween::TWEEN_INTERPOLATION_LINEAR, 500, 1000, true );
    conf.forever( true ); // palindrome
    for ( int i = 0; i < interpolation_count; ++i ) {
        tween_int_configs[ i ] = conf;
        tween_int_configs[ i ].setInterpolation( ( tween::TweenInterpolation ) i );
        values[ i ] = 0;
        tracers[ i ].init( &values[ i ], 250, conf.min, conf.max );
        tracers[ i ].setName( tween::TweenUtils::interpolationToString( tween_int_configs[ i ].interpolation ) );
        _tweener->addTween( tween_int_configs[ i ], &values[ i ] );
    }
    
    _tweener->start();
    
    allow_restart = false;
    
}

void demoInt::update( ) {

    for ( int i = 0; i < interpolation_count; ++i ) {
        tracers[ i ].update();
    }
    
}

void demoInt::draw( ) {

    int tx = 0;
    int ty = 0;
    
    ofBackground( 0, 0, 0 );
    
    ty = 150;
    for ( int i = 0; i < interpolation_count; ++i ) {
        ofSetColor( 255,0,0 );
        ofCircle( values[ i ], ty, 0, 10 ); 
        ofSetColor( 255,255,255 );
        ofDrawBitmapString( ofToString( values[ i ] ) + " > " + tracers[ i ].getName(), values[ i ] + 15, ty + 7 );
        ty += 25;
    }

    tx = 500;
    ty = 100;
    int tsize = 125;
    for ( int i = 0; i < interpolation_count; ++i ) {
        tracers[ i ].draw( tx, ty, tsize, tsize );
        tx += tsize + 5;
        if ( tx > ofGetWidth() - tsize + 5 ) {
            tx = 500;
            ty += tsize + 5;
        }
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

void demoInt::keyPressed( int key ) {

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
            for ( int i = 0; i < interpolation_count; ++i ) {
                _tweener->addTween( tween_int_configs[ i ], &values[ i ] );
            }
            allow_restart = false;
        }
        
        addlog( "restart all tweens" );
        
    } else {
        
        cout << key << ", " << (char) key << endl;
        
    }
    
}

void demoInt::keyReleased( int key ) {}

void demoInt::addlog( string s ) {
    
    ++logi;
    if ( logi >= lognum ) {
        for ( int i = 1; i < lognum; ++i ) {
            logs[ i - 1 ] = logs[ i ];
        }
        logi = lognum - 1;
    }
    logs[ logi ] = s;
    
}

void demoInt::tweenEvent( tween::TweenEvent & evt ) {
    
    if ( evt.tweener != nullptr ) {
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
