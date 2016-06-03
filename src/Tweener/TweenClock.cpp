/* This file is part of Tween, a C++ template library for animation.
* 
* Tween is free library: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
*/

#include "TweenClock.h"

using namespace tween;
using namespace std;

TweenClock::TweenClock( uint16_t fps ) :
runs( _runs ) {

    _runs = false;
    _routine_loop = false;
    _counter = 0;

    _io = nullptr;
    _timer = nullptr;
    _routine_thread = nullptr;
    _job_thread = nullptr;
    _io_thread = nullptr;

    _instant_fps = 0;
    _innertime = 0;
    _innertime_since_start = 0;
    _innertime_last = 0;
    _innertime_delta = 0;
    _innertime_speed = 1;
    _first_request = false;

    setFps( fps );
    
}

TweenClock::~TweenClock( ) {

    if ( _runs ) {
        cerr << "tween::TweenClock::~TweenClock(), threads are still running, call tween::TweenClock::stop() before destructor, or there will be memory leaks!" << endl;
    }

}

void TweenClock::purge( ) {
    
    _routine_loop = false;
    _runs = false;
    
    if ( _io != nullptr ) {
        _io->stop();
    }

    _job_mutex.lock();
    _routine_mutex.lock();
    _run_mutex.lock();
    
    if ( _io_thread != nullptr ) {
        
//        cout << "_io_thread delete" << endl;
        delete _io;
        delete _timer;
        _io_thread->interrupt( );
        _io_thread->detach( );
        delete _io_thread;
        
        if ( _routine_thread != nullptr ) {
    
//            cout << "_routine_thread delete" << endl;
            _routine_thread->interrupt( );
            _routine_thread->detach( );
            delete _routine_thread;
            
            if ( _job_thread != nullptr ) {
        
//                cout << "_job_thread delete" << endl; 
                _job_thread->interrupt( );
                _job_thread->detach( );
                delete _job_thread;
            
            }
            
        }
        
    }

    _io = nullptr;
    _timer = nullptr;
    _job_thread = nullptr;
    _routine_thread = nullptr;
    _io_thread = nullptr;

    // unlocking all mutex, just in case
    _run_mutex.unlock();
    _routine_mutex.unlock();
    _job_mutex.unlock();
        
}

bool TweenClock::start( ) {

    if ( _runs ) {
        return false;
    }
    
    purge( );
    
    _innertime = 0;
    _innertime_since_start = 0;
    _innertime_last = 0;
    _innertime_delta = 0;
    _first_request = true;
    _routine_loop = false;
    _runs = true;
    
    // unlocking all mutex, just in case
    _job_mutex.unlock();
    _routine_mutex.unlock();
    _run_mutex.unlock();
    
    _io = new boost::asio::io_service();
    _timestart = boost::posix_time::microsec_clock::local_time( );
    
    _io_thread = new boost::thread( &TweenClock::run, this );

    return true;
    
}

void TweenClock::stop( ) {

    if ( !_runs ) {
        return;
    }
    
    cout << "stopping: " << _io_thread << " & " << _routine_thread << " & " << _job_thread << endl;
    
    purge( );

}

void TweenClock::now() {
    
    // in case calling now without starting the clock
    if ( _timestart.is_not_a_date_time() ) {
        _timestart = boost::posix_time::microsec_clock::local_time( );
    }
    
    _timecurrent = boost::posix_time::microsec_clock::local_time( );
    boost::posix_time::time_duration absolute_delta = _timecurrent - _timestart;

    _innertime_last = _innertime_since_start;
    _innertime_since_start = (double) absolute_delta.total_microseconds( );
    _innertime_delta = _innertime_since_start - _innertime_last;

    _innertime += _innertime_delta * _innertime_speed;

    _instant_fps = ( round( 10000000.0 / _innertime_delta ) ) * 0.1;
    
}

void TweenClock::routine( ) {

    if ( !_runs ) {
        return;
    }
        
    while ( _routine_loop ) {
    
        _routine_mutex.lock( );

        if ( _job_mutex.try_lock() ) {

            _job_mutex.unlock();
                        
//            _timecurrent = boost::posix_time::microsec_clock::local_time( );
//            boost::posix_time::time_duration absolute_delta = _timecurrent - _timestart;
//
//            _innertime_last = _innertime_since_start;
//            _innertime_since_start = (double) absolute_delta.total_microseconds( );
//            _innertime_delta = _innertime_since_start - _innertime_last;
//            
//            _innertime += _innertime_delta * _innertime_speed;
//
//            _instant_fps = ( round( 10000000.0 / _innertime_delta ) ) * 0.1;
            
            now();
            
            _routine_loop = false;

            // done at first time routine is called, ensure threads consistency
            if ( _job_thread != nullptr ) {
                delete _job_thread;
            }
            _job_thread = new boost::thread( &TweenClock::job, this );
            
        } else if ( !_routine_loop ) {

            _routine_loop = true;

        }

        _routine_mutex.unlock( );

        // a previous job is still running, restarting it asap
        if ( _routine_loop && _runs ) {
            boost::this_thread::sleep( boost::posix_time::milliseconds( 1 ) );
        }
        
    }

}

void TweenClock::job( ) {

    if ( !_runs ) { 
        return;
    }
    
    _job_mutex.lock();
    process( );
    ++_counter;
    _job_mutex.unlock();

}

void TweenClock::process( ) {
    
    cout << "TweenClock::process() , count: " << _counter << ", deltat: " << _innertime_delta << " fps: " << _instant_fps << endl;

}


void TweenClock::run( ) {

    _run_mutex.lock();
    request( );
    // this never returns before _io->stop() is called
    _io->run( );
    _run_mutex.unlock();

}

void TweenClock::request( ) {
    
    if ( !_runs ) { 
        return;
    }
        
    // direct posting of a new request
    _request_wait = _frame_duration;
    _request_now = boost::posix_time::microsec_clock::local_time( );
    if ( !_first_request ) {
        _request_delta = _request_now - _request_last;
        if ( _request_delta.total_microseconds( ) > _frame_duration ) {
            _request_wait -= _request_delta.total_microseconds( ) - _frame_duration;
        }
    }
    
    _request_last = _request_now;
    
    if ( _timer != nullptr ) {
        delete _timer;
    }
    _timer = new boost::asio::deadline_timer( *_io );
    _timer->expires_from_now( boost::posix_time::microseconds( _request_wait ) );
    _timer->async_wait( boost::bind( &TweenClock::request, this ) );
    
    // calling routine if possible
    if ( !_routine_loop && _routine_mutex.try_lock() ) {
        _routine_mutex.unlock();
        // done at first time request is called, ensure threads consistency
        _routine_loop = true;
        if ( _routine_thread != nullptr ) {
            delete _routine_thread;
        }
        _routine_thread = new boost::thread( &TweenClock::routine, this );
    }
    
    _first_request = false;
    
}