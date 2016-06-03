#include "clockTester.h"

void clockTester::setup( ) {

//    ofSetFrameRate( 60 );
    ofSetWindowTitle( "TweenClock, performance test (creation and destruction)" );
        
    lognum = 50;
    logs = new string[ lognum ];
    logi = 0;    
    
    tween_precise_clock = new tween::TweenClock();
    tween_precise_clock->start();
    
    totalcount = 0;
    
    minfps = -1;
    maxfps = -1;
    
}

void clockTester::update( ) {

    if ( ofGetFrameNum() > 10 ) {
        float f = tween_precise_clock->fps();
        if ( minfps == -1 && f > 0 ) {
            minfps = tween_precise_clock->fps();
            maxfps = tween_precise_clock->fps();
        } else if ( f > 0 ) {
            if ( minfps > f ) { minfps = f; }
            if ( maxfps < f ) { maxfps = f; }
        }
    }
    
    if ( ofGetFrameNum() % 20 == 19 ) {
        stringstream ss;
        ss << "stopping and restarting a new TweenClock at " << ofGetFrameNum();
        addlog( ss.str() );
//        tween_precise_clock->stop();
//        totalcount += tween_precise_clock->count();
//        delete tween_precise_clock;
//        cout << "delete & new" << endl;
//        tween_precise_clock = new tween::TweenClock();
//        tween_precise_clock->start();
    }

}

void clockTester::draw( ) {

    int tx = 0;
    int ty = 0;
    
    ofBackground( 0, 0, 0 );
    
    ofSetColor( 255,255,255 );
    ty = 25;
    ofDrawBitmapString( "millis: " + ofToString( ofGetElapsedTimeMillis() ), 10, ty ); ty += 15;
    ofDrawBitmapString( "of fps: " + ofToString( ofGetFrameRate() ), 10, ty ); ty += 15;
    ofDrawBitmapString( "clock total count: " + ofToString( totalcount ), 10, ty ); ty += 15;
    ofDrawBitmapString( "clock instant fps (no stabilisation): " + ofToString( tween_precise_clock->fps() ), 10, ty ); ty += 15;
    ofDrawBitmapString( "clock min fps: " + ofToString( minfps ), 10, ty ); ty += 15;
    ofDrawBitmapString( "clock max fps: " + ofToString( maxfps ), 10, ty ); ty += 15;
    ofDrawBitmapString( "clock innertime: " + ofToString( tween_precise_clock->innertime() ), 10, ty ); ty += 15;
    
    ofSetColor( 255,255,100 );
    stringstream ss;
    for ( int i = 1; i < lognum; ++i ) {
        ss << logs[ i ] << endl;
    }
    ofDrawBitmapString( ss.str(), 400, 25 );
    
}

void clockTester::keyPressed( int key ) {

    cout << key << ", " << (char) key << endl;
    
}

void clockTester::keyReleased( int key ) {}

void clockTester::addlog( string s ) {
    
    ++logi;
    if ( logi >= lognum ) {
        for ( int i = 1; i < lognum; ++i ) {
            logs[ i - 1 ] = logs[ i ];
        }
        logi = lognum - 1;
    }
    logs[ logi ] = s;
    
}

void clockTester::tweenEvent( tween::TweenEvent & evt ) {
    
    if ( evt.tweener != nullptr ) {
        if ( evt.type == tween::TWEEN_EVENT_ALL_TWEENS_DONE ) {}
    }
    
    if ( evt.tween != nullptr ) {
        tween::Tween * tw = (tween::Tween *) evt.tween;
        if( 
            evt.type == tween::TWEEN_EVENT_START || 
            evt.type == tween::TWEEN_EVENT_STOP || 
            evt.type == tween::TWEEN_EVENT_LOOP
        )
        {}
    }
    
};