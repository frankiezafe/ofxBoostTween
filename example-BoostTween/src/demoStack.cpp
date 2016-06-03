#include "demoStack.h"

void demoStack::setup( ) {

    ofSetFrameRate( 60 );
    
    stringstream ss;
    ss << "synch updates, tweens stack and precise destruction";
    ofSetWindowTitle( ss.str() );
    
    _tweener = new tween::Tweener( 100 );
    _tweener->addListener( this );
    
    lognum = 7;
    logs = new string[ lognum ];
    logi = 0;
    
    conf1.set( 100, 800, tween::TWEEN_INTERPOLATION_BACK, 200, 1000 );
    // default in a config
    conf1.setStack( tween::TWEEN_STACK_DELAY );
    
    conf2.set( 100, 800, tween::TWEEN_INTERPOLATION_BACK, 200, 1000 );
    // avoid any other tweens to be appends
    conf2.setStack( tween::TWEEN_STACK_AVOID );
    
    conf_back.set( 800, 100, tween::TWEEN_INTERPOLATION_BACK, 0, 300 );
    
    conf_always = conf1;
    // calling forever set yout tween's srtack to AVOID
    conf_always.forever( true );
    
    _tweener->addTween<float>( conf1, &val1 );
    _tweener->addTween<float>( conf_back, &val1 );
    _tweener->addTween<float>( conf1, &val1 );
    _tweener->addTween<float>( conf_back, &val1 );
    
    _tweener->addTween<float>( conf1, &val2 );
    _tweener->addTween<float>( conf_back, &val2 );
    _tweener->addTween<float>( conf2, &val2 );
    _tweener->addTween<float>( conf_back, &val2 );
    
    _tweener->addTween<float>( conf1, &val3 );
    _tweener->addTween<float>( conf_back, &val3 );
    _tweener->addTween<float>( conf_always, &val3 );
    _tweener->addTween<float>( conf_back, &val3 );
    
//    _tweener->start();
    
}

void demoStack::update( ) {

    // calling tweener update manually
    _tweener->update();
    
}

void demoStack::draw( ) {

    int tx = 0;
    int ty = 0;
    
    ofBackground( 0, 0, 0 );
    
    ofSetColor( 255,0,0 );
    ofCircle( val1, 150, 20 );
    ofCircle( val2, 200, 20 );
    ofCircle( val3, 250, 20 );
    
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

void demoStack::keyPressed( int key ) {

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
        
        if ( _tweener->getTweenCount( &val3 ) == 0 ) {
            _tweener->addTween<float>( conf1, &val3 );
            _tweener->addTween<float>( conf_back, &val3 );
            _tweener->addTween<float>( conf_always, &val3 );
            addlog( "starting tweens for val3" );
        } else {
            _tweener->removeTween<float>(&val3);
            addlog( "removeTween attached to val3" );
        }
        
    } else {
        
        cout << key << ", " << (char) key << endl;
        
    }
    
}

void demoStack::keyReleased( int key ) {}

void demoStack::addlog( string s ) {
    
    ++logi;
    if ( logi >= lognum ) {
        for ( int i = 1; i < lognum; ++i ) {
            logs[ i - 1 ] = logs[ i ];
        }
        logi = lognum - 1;
    }
    logs[ logi ] = s;
    
}

void demoStack::tweenEvent( tween::TweenEvent & evt ) {
    
    if ( evt.tweener != nullptr ) {
        stringstream ss;
        ss << evt.time << " >> tweener, type: " << tween::TweenUtils::tweenTypetoString( evt.type ) << ", " << evt.tweener;
        addlog( ss.str() );
        if ( evt.type == tween::TWEEN_EVENT_ALL_TWEENS_DONE ) { ; }
        else if ( evt.type == tween::TWEEN_EVENT_LAST ) { 
            stringstream ss;
            ss << evt.time << " > last tween finished for pointer " << evt.ptr;
            addlog( ss.str() );
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
