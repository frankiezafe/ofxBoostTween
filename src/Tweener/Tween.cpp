/* This file is part of Tween, a C++ template library for animation.
* 
* Tween is free library: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
*/


#include <complex>

#include "Tween.h"

using namespace std;
using namespace tween;

template <> 
void Tween_<Callback>::apply() {
    if ( _percent == 1 ) {
        _target_ptr->call();
    }
}

template <> 
void Tween_<CallbackValue>::apply() {
    _target_ptr->call( _value );
}

template <> 
void Tween_<bool>::apply() {
    if ( _percent == 1 ) {
        *_target_ptr = _max;
    } else {
        *_target_ptr = _min;
    }
}

template <> 
void Tween_<std::string>::apply() {
    if ( _percent == 1 ) {
        *_target_ptr = _max;
    } else {
        *_target_ptr = _min;
    }
}

template <> 
void Tween_<float>::validateRotations() {
    
//    cout << "validateRotations, float" << endl;
    
    // rotation enabled
    double min_2PI = 0;
    double max_2PI = 0;
    double start_2PI = 0;
    
    _min = TweenUtils::reduceToPI( (double) _min, min_2PI );
    _max = TweenUtils::reduceToPI( (double) _max, max_2PI );
    _start_value = TweenUtils::reduceToPI( (double) _start_value, start_2PI );
        
    if ( abs<float>( _max -_min ) > M_PI ) {
        if ( _max > 0 ) {
            _max -= M_PI * 2;
        } else {
            _max += M_PI * 2;
        }
    }
    
    if ( _play_count == 0 && _start_at_value ) {
        _max = TweenUtils::reduceToPI( (double) _max, max_2PI );
        if ( abs<float>( _max -_start_value ) > M_PI ) {
            if ( _start_value > 0 ) {
                _start_value -= M_PI * 2;
            } else {
                _start_value += M_PI * 2;
            }
        }
    }

}

template <> 
void Tween_<double>::validateRotations() {
    
//    cout << "validateRotations, float" << endl;
    
    // rotation enabled
    double min_2PI = 0;
    double max_2PI = 0;
    double start_2PI = 0;
    
    _min = TweenUtils::reduceToPI( _min, min_2PI );
    _max = TweenUtils::reduceToPI( _max, max_2PI );
    _start_value = TweenUtils::reduceToPI( _start_value, start_2PI );
        
    if ( abs<float>( _max -_min ) > M_PI ) {
        if ( _max > 0 ) {
            _max -= M_PI * 2;
        } else {
            _max += M_PI * 2;
        }
    }
    
    if ( _play_count == 0 && _start_at_value ) {
        _max = TweenUtils::reduceToPI( _max, max_2PI );
        if ( abs<float>( _max -_start_value ) > M_PI ) {
            if ( _start_value > 0 ) {
                _start_value -= M_PI * 2;
            } else {
                _start_value += M_PI * 2;
            }
        }
    }

}
