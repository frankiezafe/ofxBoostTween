/* This file is part of Tween, a C++ template library for animation.
* 
* Tween is free library: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
*/

/* 
 * File:   TweenConfig.h
 * Author: frankiezafe
 *
 * Created on May 17, 2016, 8:10 AM
 */

#ifndef TWEENCONFIG_H
#define TWEENCONFIG_H

#include "Common.h"

namespace tween {
    
    template< typename TweenType >
    class TweenConfig {
    
    public:
        
        const TweenInterpolation & interpolation;
        const double & offset_millis;
        const double & duration_millis;
        const TweenType & min;
        const TweenType & start_value;
        const TweenType & max;
        const TweenLoop & loop;
        const int & repeat_count;
        const bool & start_at_value;
        const bool & rotation;
        const TweenStack & stack;
        
        TweenConfig< TweenType >() :
            interpolation(_interpolation),
            offset_millis(_offset_millis),
            duration_millis(_duration_millis),
            min(_min),
            max(_max),
            loop(_loop),
            repeat_count(_repeat_count),
            start_value( _start_value ),
            start_at_value( _start_at_value ),
            rotation( _rotation ),
            stack( _stack )
        {
            _interpolation =    TWEEN_INTERPOLATION_LINEAR;
            _offset_millis =    0;
            _duration_millis =  1000;
            _loop =             TWEEN_LOOP_ONCE;
            _repeat_count =     0;
            _start_at_value =   false;
            _rotation =         false;
        }
        
        TweenConfig< TweenType >(
            TweenType minimum,
            TweenType maximum,
            TweenInterpolation ti =     TWEEN_INTERPOLATION_LINEAR,
            double offsetms =           0,
            double durationms =         1000,
            bool startatvalue =         false,
            bool rotation =             false,
            TweenLoop l =               TWEEN_LOOP_ONCE,
            int repeat =                0,
            TweenStack ts =             TWEEN_STACK_DELAY
        ) :
            interpolation(_interpolation),
            offset_millis(_offset_millis),
            duration_millis(_duration_millis),
            min(_min),
            max(_max),
            loop(_loop),
            repeat_count(_repeat_count),
            start_value( _start_value ),
            start_at_value( _start_at_value ),
            rotation( _rotation ),
            stack( _stack )
        {
            _interpolation =    ti;
            _offset_millis =    offsetms;
            _duration_millis =  durationms;
            _min =              minimum;
            _start_value =      minimum;
            _max =              maximum;
            _loop =             l;
            _repeat_count =     repeat;
            _start_at_value =   startatvalue;
            _rotation =         rotation;
            _stack =            ts;
        };
        
        // gives problem with const binding...
        TweenConfig< TweenType >( const TweenConfig< TweenType > * src ) :
            interpolation(this->_interpolation),
            offset_millis(this->_offset_millis),
            duration_millis(this->_duration_millis),
            min(this->_min),
            max(this->_max),
            loop(this->_loop),
            repeat_count(this->_repeat_count),
            start_value(this->_start_value),
            start_at_value(this->_start_at_value),
            rotation(this->_rotation)
        {
            _interpolation = src->interpolation;
            _offset_millis = src->offset_millis;
            _duration_millis = src->duration_millis;
            _min = src->min;
            _start_value = src->start_value;
            _max = src->max;
            _loop = src->loop;
            _repeat_count = src->repeat_count;
            _start_at_value = src->start_at_value;
            _rotation = src->rotation;
        };
        
        inline void operator = ( const TweenConfig & src ) {
            _interpolation = src.interpolation;
            _offset_millis = src.offset_millis;
            _duration_millis = src.duration_millis;
            _min = src.min;
            _max = src.max;
            _loop = src.loop;
            _repeat_count = src.repeat_count;
            _start_value = src.start_value;
            _start_at_value = src.start_at_value;
            _rotation = src.rotation;
            _stack = src.stack;
        };
        
        virtual void invert() {
            TweenType d = _min;
            _min = _max;
            _max = d;
            _start_value = _min;
        }
        
        // SETTERS
        inline void set(
            TweenType minimum,
            TweenType maximum,
            TweenInterpolation ti =     TWEEN_INTERPOLATION_LINEAR,
            double offsetms =           0,
            double durationms =         1000,
            bool startatvalue =         false,
            bool rotation =             false,
            TweenLoop l =               TWEEN_LOOP_ONCE,
            int repeat =                0,
            TweenStack ts =             TWEEN_STACK_DELAY
        ) {
            _min =              minimum;
            _max =              maximum;
            _interpolation =    ti;
            _offset_millis =    offsetms;
            _duration_millis =  durationms;
            _loop =             l;
            _repeat_count =     repeat;
            _rotation =         rotation;
            _stack =            ts;
        };
        
        inline void setMin( TweenType minimum ) {
            _min = minimum;
        };
        
        inline void setMax( TweenType maximum ) {
            _max = maximum;
        };
        
        inline void setInterpolation( TweenInterpolation i ) {
            _interpolation = i;
        }
        
        inline void setOffset( uint16_t millis ) {
            _offset_millis = millis;
        }
        
        inline void setDuration( uint16_t millis ) {
            _duration_millis = millis;
        }
        
        inline void setLoop( TweenLoop l ) {
            _loop = l;
        }
        
        inline void setRepeatCount( int num ) {
            _repeat_count = num;
        }
        
        inline void setStartAtValue( bool enabled ) {
            _start_at_value = enabled;
        }
        
        inline void setRotation( bool enabled ) {
            _rotation = enabled;
        }
        
        inline void setStack( TweenStack ts ) {
            _stack = ts;
        }
        
        // HELPERS
        inline void repeat( int num, bool palindrome = false ) {
            if ( num == 0 ) {
                _loop = TWEEN_LOOP_ONCE;
                _repeat_count = 0;
            } else {
                if ( palindrome ) {
                    _loop = TWEEN_LOOP_PALINDROME;
                } else {
                    _loop = TWEEN_LOOP_REPEAT;
                }
                _repeat_count = num;
            }
        }
        
        void forever( bool palindrome = false ) {
            repeat( -1, palindrome );
            _stack = TWEEN_STACK_AVOID;
        }
        
    protected:
        
        TweenInterpolation _interpolation;
        double _offset_millis;
        double _duration_millis;
        TweenType _min;
        TweenType _start_value;
        TweenType _max;
        TweenLoop _loop;
        int _repeat_count;
        bool _start_at_value;
        bool _rotation;
        TweenStack _stack;
        
    };
    
}

#endif /* TWEENCONFIG_H */

