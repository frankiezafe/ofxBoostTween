/* This file is part of Tween, a C++ template library for animation.
* 
* Tween is free library: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
*/

/* 
 * File:   Tweener.cpp
 * Author: frankiezafe
 * 
 * Created on May 9, 2016, 8:55 AM
 */

#include <memory>

#include "Tweener.h"

using namespace tween;
using namespace std;

Tweener::Tweener( uint16_t fps, bool use_thread ) :
        _jobdoneEventsEnabled( false )
{
    
    _thread_enabled = use_thread;
    _paused = false;
    setFps( fps );

}

Tweener::~Tweener( ) {
}

bool Tweener::start() {
    
    if ( !_thread_enabled ) {
        cerr << "tween::Tweener::start(), threading is disabled" << endl;
        return false;
    }
    _paused = false;
    TweenClock::start();
    fire( TWEEN_EVENT_START );
    
}

void Tweener::stop() {
    
    if ( !_thread_enabled ) {
        cerr << "tween::Tweener::stop(), threading is disabled" << endl;
        return;
    }
    TweenClock::stop();
    fire( TWEEN_EVENT_STOP );
    
}

void Tweener::killall() {
    
    // have to be done outside of the job ( job calls process )
    _job_mutex.lock();
    vector< Tween * > zombies;
    _tweens.swap( zombies );
    clearZombies( zombies );
    zombies.clear();
    _tweens_to_append.swap( zombies );
    clearZombies( zombies );
    _job_mutex.unlock();
    
}

void Tweener::update() {
    
    if ( _io != nullptr ) {
        cerr << "tween::Tweener::update(), thread is running >> explicit updates disabled" << endl;
        return;
    }
    
    bool lp = _paused;
    _job_mutex.lock();
    _paused = false;
    now();
    process();
    _paused = lp;
    _job_mutex.unlock();
    fire( TWEEN_EVENT_UPDATE );
    
}

void Tweener::process() {
        
    if ( _paused ) {
        return;
    }
    
    // checking if there rew new tweens in the queue
    if ( !_tweens_to_append.empty() ) {
        _tweens.insert( _tweens.end(), _tweens_to_append.begin(), _tweens_to_append.end() );
        _tweens_to_append.clear();
    }
    
    vector< Tween * > alives;
    vector< Tween * > zombies;
    
    // updating + collecting deads and alives
    for( Tween * atw : _tweens ) {
        
        atw->update( _innertime );
        if ( atw->active() ) {
            atw->apply();
        }
        
        if ( !atw->alive() ) {
            zombies.push_back( atw );
        } else {
            alives.push_back( atw );
        }
    }
    
    // cleaning dead ones
    if ( !zombies.empty() ) { 
        _tweens.swap( alives );
        clearZombies( zombies );
        if ( _tweens.empty() ) {
            fire( TWEEN_EVENT_ALL_TWEENS_DONE );
        }
    }
    
    if ( _jobdoneEventsEnabled ) { 
        fire( TWEEN_EVENT_PROCESSDONE );
    }
    
}

void Tweener::clearZombies( std::vector< Tween * > & zombies ) {
    
    for ( Tween * atw : zombies ) {
    
//        cout << "bye bye zombie: " << atw << " pointing to: " << atw->get_target_ptr() << endl;
        
        if ( atw->get_target_ptr() != nullptr ) {
            const void * vptr = atw->get_target_ptr();
            for ( vector< Tween * >::iterator atw_mapped = _tween_map[ vptr ].begin(); atw_mapped != _tween_map[ vptr ].end(); ++atw_mapped ) {
                if ( (*atw_mapped) == atw ) {
                    _tween_map[ vptr ].erase( atw_mapped );
//                    cout << "removed entries for " << vptr << ", remains " << _tween_map[ vptr ].size() << " tweens" << endl;
                    if ( _tween_map[ vptr ].empty() ) {
                        fire( TWEEN_EVENT_LAST, vptr );
                        _tween_map.erase( vptr );
                    }
                    break;
                }
            }
        }

        // removing entry form map
        delete atw;

    }

}   

void Tweener::fire( TweenEventType t, const void * ptr ) {

    _fire_mutex.lock();

    TweenEvent twe;
    twe.type = t;
    twe.time = _innertime;
    twe.counter = 0;
    twe.tweener = this;
    twe.tween = nullptr;
    twe.ptr = ptr;
    TweenCaller::fire( twe );

    _fire_mutex.unlock();

}

int Tweener::activeTweens() {
    
    int tcount = 0;
    
    _job_mutex.lock();
    
    tcount = _tweens.size() + _tweens_to_append.size();
    
    _job_mutex.unlock();
    
    return tcount;
    
}

bool Tweener::addListener( TweenListener * tl ) {
    
    _fire_mutex.lock();
    
    bool success = TweenCaller::addListener( tl );
    // synching all tweens managed
    for ( auto itt : _tweens ) {
        itt->cloneListener( listeners );
    }
    
    _fire_mutex.unlock();
    
    return success;
    
}

bool Tweener::removeListener( TweenListener * tl ) {
    
    _fire_mutex.lock();

    bool success = TweenCaller::removeListener( tl );
    // synching all tweens managed
    for ( auto itt : _tweens ) {
        itt->cloneListener( listeners );
    }

    _fire_mutex.unlock();
    
    return success;

}

bool Tweener::removeAllListener( TweenListener * tl ) {
    
    _fire_mutex.lock();
    
    bool success = TweenCaller::removeAllListener();
    // synching all tweens managed
    for ( auto itt : _tweens ) {
        itt->removeAllListener();
    }

    _fire_mutex.unlock();
    
    return success;
    
}

void Tweener::addTween( uint16_t time_offset_millis, uint16_t duration_millis ) {
    
    TweenConfig<double> tc( _defaultconfig );
    tc.setOffset( time_offset_millis );
    tc.setDuration( duration_millis );
    return addTween( tc );
    
}

void Tweener::addTween( const TweenConfig<double> & conf ) {

    Tween_<double> * tw = new Tween_<double>( conf, _innertime );
    tw->cloneListener( listeners );
    
    _job_mutex.lock();
    _tweens_to_append.push_back( tw );
    _job_mutex.unlock();
    
}