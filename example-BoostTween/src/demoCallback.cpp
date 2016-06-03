#include "demoCallback.h"

void demoCallback::setup( ) {

    ofSetFrameRate( 60 );
    
    stringstream ss;
    ss << "Tweens, callback interpolations demo";
    ofSetWindowTitle( ss.str() );
    
    _tweener = new tween::Tweener( 100 );
    _tweener->addListener( this );
        
    lognum = 7;
    logs = new string[ lognum ];
    logi = 0;
           
    cb = new tween::Callback( boost::bind( &demoCallback::callback, this ) );
    tween::TweenConfig<tween::Callback> tccb;
    tccb.forever( false );
    _tweener->addTween<tween::Callback>( tccb, cb );
    
    cbv = new tween::CallbackValue( boost::bind( &demoCallback::callback_value, this, _1 ) );
    tween::TweenConfig<tween::CallbackValue> tccbv;
    tccbv.forever( true );
    _tweener->addTween<tween::CallbackValue>( tccbv, cbv );
   
    _tweener->start();
    
}

void demoCallback::callback() {
    addlog( "callback(), end of tween" );
    ofToggleFullscreen();
}

void demoCallback::callback_value( double i ) {
    stringstream ss;
    ss << "callback(double), value of tween: " << i;
    addlog( ss.str() );
}

void demoCallback::update( ) {

}

void demoCallback::draw( ) {

    int tx = 0;
    int ty = 0;
    
    ofBackground( 0, 0, 0 );
    
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

void demoCallback::keyPressed( int key ) {

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

void demoCallback::keyReleased( int key ) {}

void demoCallback::addlog( string s ) {
    
    ++logi;
    if ( logi >= lognum ) {
        for ( int i = 1; i < lognum; ++i ) {
            logs[ i - 1 ] = logs[ i ];
        }
        logi = lognum - 1;
    }
    logs[ logi ] = s;
    
}

void demoCallback::tweenEvent( tween::TweenEvent & evt ) {
    
    if ( evt.tweener != nullptr ) {
        stringstream ss;
        ss << evt.time << " >> tweener, type: " << tween::TweenUtils::tweenTypetoString( evt.type ) << ", " << evt.tweener;
        addlog( ss.str() );
        if ( evt.type == tween::TWEEN_EVENT_ALL_TWEENS_DONE ) { ; }
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
