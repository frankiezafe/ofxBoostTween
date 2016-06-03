/* This file is part of Tween, a C++ template library for animation.
* 
* Tween is free library: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
*/

/* 
 * File:   TweenClock.h
 * Author: frankiezafe
 *
 * Created on May 20, 2016, 3:42 PM
 */

#ifndef BTHREAD_H
#define BTHREAD_H

#include <iostream>  
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/asio.hpp>

namespace tween {

    class TweenClock {

    public:

        bool & runs;

        TweenClock( uint16_t fps = 100 );
        ~TweenClock();
        
        virtual bool start();
        virtual void stop();
        
        // setters
        inline void setFps( uint16_t fps ) { 
            _frame_duration = 1000000.0 / fps;
        }
        inline void setTimeSpeed( float ts ) { 
            _innertime_speed = ts;
        }
        
        // getters
        inline bool running() { 
            return _runs;
        }
        inline bool routine_loop() { 
            return _routine_loop;
        }
        inline bool run_locked() { 
            if ( _run_mutex.try_lock() ) {
                _run_mutex.unlock();
                return false;
            }
            return true;
        }
        inline bool routine_locked() { 
            if ( _routine_mutex.try_lock() ) {
                _routine_mutex.unlock();
                return false;
            }
            return true;
        }
        inline bool job_locked() { 
            if ( _job_mutex.try_lock() ) {
                _job_mutex.unlock();
                return false;
            }
            return true;
        }
        inline uint64_t count() { 
            return _counter;
        }
        inline double frame_duration() { 
            return _frame_duration;
        }
        inline double innertime() { 
            return _innertime;
        }
        inline double deltatime() { 
            return _innertime_delta;
        }
        inline double timespeed() { 
            return _innertime_speed;
        }
        inline float fps() {
            return _instant_fps;
        }

    protected:

        bool _runs;
        bool _routine_loop;
        boost::mutex _run_mutex;            // used only to clear
        boost::mutex _routine_mutex;        // used to lock routine
        boost::mutex _job_mutex;            // used to verify job is done when receiving request
        boost::thread * _job_thread;        // holds job()
        boost::thread * _routine_thread;    // holds routine()
        boost::thread * _io_thread;         // keep the service active
        uint64_t _counter;

        boost::asio::io_service * _io;
        boost::asio::deadline_timer * _timer;

        double _frame_duration;
        double _request_wait;
        boost::posix_time::ptime _request_last;
        boost::posix_time::ptime _request_now;
        boost::posix_time::time_duration _request_delta;

        // inner time
        double _innertime;
        double _innertime_since_start;
        double _innertime_last;
        double _innertime_delta;
        float _innertime_speed;
        float _instant_fps;

        // routine time measure
        boost::posix_time::ptime _timestart;
        boost::posix_time::ptime _timecurrent;
        bool _first_request;

        // active methods, overload process() to specialise the class
        void purge();
        void request();
        // concurrent processes
        void job();
        void routine();
        void run();
        
        void now();
        // overload to specialise the clock
        virtual void process();

    };
    
};


#endif /* BTHREAD_H */

