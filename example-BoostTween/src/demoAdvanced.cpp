#include "demoAdvanced.h"


// overload of tween_::apply
template <> 
void tween::Tween_<ofQuaternion>::apply() {
    _target_ptr->slerp( _value, _start_value, _max );
}
template <> 
void tween::Tween_<ofMatrix4x4>::apply() {

    ofMatrix4x4 * sstart = (ofMatrix4x4*) &_start_value;
    ofMatrix4x4 * smax = (ofMatrix4x4*) &_max;
    
    ofQuaternion q;
    q.slerp( _value, sstart->getRotate(), smax->getRotate() );
    ofMatrix4x4 work;
    
    work.makeIdentityMatrix();
    work.rotate( q );
    work.translate( sstart->getTranslation().getInterpolated( smax->getTranslation(), _value ) );
    work.scale( sstart->getScale().getInterpolated( smax->getScale(), _value ) );

    *_target_ptr = work;

}


void demoAdvanced::setup( ) {

    ofSetFrameRate( 60 );
    
    stringstream ss;
    ss << "Tweens, advanced interpolations demo";
    ofSetWindowTitle( ss.str() );
    
    _tweener = new tween::Tweener( 100 );
    _tweener->addListener( this );
    
    lognum = 4;
    logs = new string[ lognum ];
    logi = 0;
    
    // quaternion
    quaternion.makeRotate( ofRandomf() * 180, ofRandomf(), ofRandomf(), ofRandomf() );
    tween::TweenConfig<ofQuaternion> qcf(
        ofQuaternion( PI, -1,0,1 ),
        ofQuaternion( -PI, 1,0.5,1 ),
        tween::TWEEN_INTERPOLATION_ELASTIC_OUT,
        500, 2000, true
    );
    qcf.forever( true );
    _tweener->addTween( qcf, &quaternion );
    
    // matrix
    mat.makeIdentityMatrix();
    mat.rotate( ofRandomf() * 180, ofRandomf(), ofRandomf(), ofRandomf() );
    mat.translate( -200, 0, 0 );
    mat.scale(  1,2,0.2  );
    
    ofMatrix4x4 mmin;
    mmin.makeIdentityMatrix();
    mmin.rotate( -90, 1, 0, 0.2 );
    mmin.translate( -300, 0, 0 );
    
    ofMatrix4x4 mmax;
    mmax.makeIdentityMatrix();
    mmax.rotate( 90, 1, 0, 0 );
    mmax.translate( 300, 0, 0 );
    mmax.scale(  1,3,3  );
    
//    mat = mmax;
    
    tween::TweenConfig<ofMatrix4x4> mcf(
        mmin,
        mmax,
        tween::TWEEN_INTERPOLATION_ELASTIC_OUT,
        200, 3000, true
    );
    mcf.forever( true );
    _tweener->addTween( mcf, &mat );
    
    _tweener->start();
    
}

void demoAdvanced::update( ) {
    
}

void demoAdvanced::draw( ) {

    int tx = 0;
    int ty = 0;
    
    ofBackground( 0, 0, 0 );
    
    // quaternion
    ofPushMatrix();

        ofTranslate( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, -200 );
        
        ofNoFill();
        ofSetColor( 255,255,255 );
        float rotationAmount;
        ofVec3f rotationAngle;
        quaternion.getRotate(rotationAmount, rotationAngle);
        ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
        ofBox( 0,0,0, 100, 100, 100 );
        
        ofSetColor( 255,0,0 );
        ofLine( 0,0,0, 100,0,0 );
        ofSetColor( 0,255,0 );
        ofLine( 0,0,0, 0,100,0 );
        ofSetColor( 0,0,255 );
        ofLine( 0,0,0, 0,0,100 );
    
    ofPopMatrix();
    
    // matrix 4x4
    ofPushMatrix();

        ofTranslate( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, -200 );
        
        ofNoFill();
        ofSetColor( 255,255,255 );
        
        ofMultMatrix( mat );
        ofBox( 0,0,0, 200, 200, 200 );
        
        ofSetColor( 255,0,0 );
        ofLine( 0,0,0, 100,0,0 );
        ofSetColor( 0,255,0 );
        ofLine( 0,0,0, 0,100,0 );
        ofSetColor( 0,0,255 );
        ofLine( 0,0,0, 0,0,100 );
    
    ofPopMatrix();
    
    
    
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

void demoAdvanced::keyPressed( int key ) {

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

void demoAdvanced::keyReleased( int key ) {}

void demoAdvanced::addlog( string s ) {
    
    ++logi;
    if ( logi >= lognum ) {
        for ( int i = 1; i < lognum; ++i ) {
            logs[ i - 1 ] = logs[ i ];
        }
        logi = lognum - 1;
    }
    logs[ logi ] = s;
    
}

void demoAdvanced::tweenEvent( tween::TweenEvent & evt ) {
    
    if ( evt.tweener != nullptr ) {
        if ( evt.type == tween::TWEEN_EVENT_ALL_TWEENS_DONE ) {
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
