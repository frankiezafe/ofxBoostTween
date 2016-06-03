/* This file is part of Tween, a C++ template library for animation.
* 
* Tween is free library: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
*/

/* 
 * File:   Tween.h
 * Author: frankiezafe
 *
 * Created on May 9, 2016, 12:36 PM
 */

#ifndef TWEEN_H
#define TWEEN_H

#include <typeinfo>
#include <unistd.h>
#include "TweenConfig.h"
#include "TweenCaller.h"

namespace tween {

    class Tween : public TweenCaller {
    public:
        virtual bool update( double currentime ) = 0;
        virtual void apply() = 0;
        // GETTERS
        virtual bool alive() = 0;
        virtual bool active() = 0;
        virtual double percent() = 0;
        virtual double value() = 0;
        virtual const void * get_target_ptr() = 0;
    }; 
    
    template< typename TweenType >
    class Tween_ : public Tween, public TweenConfig< TweenType >  {
    public:
        
        Tween_< TweenType >( const TweenConfig< TweenType > & conf, double now ) :
            TweenConfig<TweenType>() {

            _alive = true;
            _active = false;
            _percent = 0;
            
            _target_ptr = nullptr;
            _play_count = 0;

            (*( TweenConfig<TweenType> * ) this ) = conf;

            _offset = TweenConfig<TweenType>::_offset_millis * MILLIS2MIRCOS;
            _duration = TweenConfig<TweenType>::_duration_millis * MILLIS2MIRCOS;
            _value = 0;

            initTime( now );

            if ( _starttime > _endtime ) {
                std::cerr << "Tween::Tween, wrong instanciation of Tween, endtime must be >= to starttime" << std::endl;
                _alive = false;
                fire( TWEEN_EVENT_ERROR );
            }
        }
            
        Tween_< TweenType >( TweenType * ptr, const TweenConfig< TweenType > & conf, double now ) :
            TweenConfig<TweenType>() {

            _alive = true;
            _active = false;
            _percent = 0;

            _target_ptr = ptr;
            
            _play_count = 0;

            (*( TweenConfig<TweenType> * ) this ) = conf;

            _offset = TweenConfig<TweenType>::_offset_millis * MILLIS2MIRCOS;
            _duration = TweenConfig<TweenType>::_duration_millis * MILLIS2MIRCOS;
            _value = 0;
            
            initTime( now );

            if ( _starttime > _endtime ) {
                std::cerr << "Tween::Tween, wrong instanciation of Tween, endtime must be >= to starttime" << std::endl;
                _alive = false;
                fire( TWEEN_EVENT_ERROR );
            }
        }
        
        virtual ~Tween_< TweenType >() {}
        
        void initTime( double now ) {
            
            if ( TweenConfig<TweenType>::_start_at_value && _play_count == 0 ) {
                TweenConfig<TweenType>::_start_value = *((TweenType*) _target_ptr);
            } else {
                TweenConfig<TweenType>::_start_value = TweenConfig<TweenType>::_min;
            }

            if ( TweenConfig<TweenType>::_rotation ) {
                validateRotations();
            }
            
            _starttime = _offset + now;
            _endtime = _offset + now + _duration;
            _active = false;
            
        }
        
        bool update( double now ) {
            
            if ( _alive ) {
                
                if ( now >= _endtime ) {

                    // depending on the loop, action may differ

                    if ( TweenConfig<TweenType>::_loop == TWEEN_LOOP_ONCE ) {

                        // finishing here
                        _active = false;
                        _alive = false;
                        _percent = 1;
                        _value = 1;

                        fire( TWEEN_EVENT_STOP );

                        return false;

                    } else {

                        ++_play_count;

                        if ( TweenConfig<TweenType>::_repeat_count != -1 && _play_count > TweenConfig<TweenType>::_repeat_count ) {

                            // finishing here
                            _active = false;
                            _alive = false;
                            fire( TWEEN_EVENT_STOP, now );
                            return false;

                        }
                        
                        _percent = 1;
                        _value = 1;

                        if ( TweenConfig<TweenType>::_loop == TWEEN_LOOP_PALINDROME ) {
                            TweenConfig<TweenType>::invert();
                        }

                        double overlap = now - _endtime;
                        initTime( now - overlap );

                        fire( TWEEN_EVENT_LOOP, now );
                        
                        return true;

                    }

                }

                if ( !_active && now >= _starttime ) {

                    _active = true;
                    fire( TWEEN_EVENT_START, now );

                }

                if ( _active ) {
                    
                    _percent = ( now - _starttime ) * 1.0 / _duration;
                    
                    double k = 0;
                    double s = 0;
                    double a = 0;
                    double p = 0;

                    switch( TweenConfig<TweenType>::_interpolation ) {
                        
                        case TWEEN_INTERPOLATION_CONSTANT:
                            if ( _percent == 1 ) {
                                _value = 1;
                            } else {
                                _value = 0;
                            }
                            break;
                            
                        case TWEEN_INTERPOLATION_LINEAR:
                            _value = _percent;
                            break;
                            
                        case TWEEN_INTERPOLATION_SINUS:
                            _value = ( sin( -( M_PI * 0.5 ) + _percent * M_PI ) + 1 ) * 0.5;
                            break;
                            
                        case TWEEN_INTERPOLATION_SINUS_IN:
                            _value = 1 - cos( _percent * M_PI * 0.5 );
                            break;
                            
                        case TWEEN_INTERPOLATION_SINUS_OUT:
                            _value = sin( _percent * M_PI * 0.5 );
                            break;
                            
                        case TWEEN_INTERPOLATION_QUADRATIC:
                            k = _percent * 2;
                            if ( k < 1 ) {
                               _value = 0.5 * k * k; 
                            } else {
                                _value = -0.5 * (--k * (k - 2) - 1 ); 
                            }
                            break;
                            
                        case TWEEN_INTERPOLATION_QUADRATIC_IN:
                            k = _percent;
                            _value = k * k; 
                            break;
                            
                        case TWEEN_INTERPOLATION_QUADRATIC_OUT:
                            k = _percent;
                            _value = k * (2 - k); 
                            break;
                            
                        case TWEEN_INTERPOLATION_CUBIC:
                            k = _percent * 2;
                            if ( k < 1 ) {
                               _value = 0.5 * k * k * k; 
                            } else {
                                k -= 2;
                                _value = 0.5 * ( k * k * k + 2 ); 
                            }
                            break;
                            
                        case TWEEN_INTERPOLATION_CUBIC_IN:
                            k = _percent;
                            _value = k * k * k; 
                            break;
                            
                        case TWEEN_INTERPOLATION_CUBIC_OUT:
                            k = _percent;
                            _value = --k * k * k + 1; 
                            break;
                            
                        case TWEEN_INTERPOLATION_EXPONENTIAL:
                            if ( _percent == 0 || _percent == 1 ) {
                                _value = _percent;
                            } else if ( _percent < 0.5 ) {
                                k = _percent * 2;
                                _value = 0.5 * pow( 1024, k - 1 );
                            } else {
                                k = _percent * 2;
                                _value = 0.5 * ( -pow( 2, -10 * ( k - 1 ) ) + 2 );
                            }
                            break;
                            
                        case TWEEN_INTERPOLATION_EXPONENTIAL_IN:
                            if ( _percent == 0 ) {
                                _value = 0;
                            } else {
                                k = _percent;
                                _value = pow( 1024, k - 1 );
                            }
                            break;
                            
                        case TWEEN_INTERPOLATION_EXPONENTIAL_OUT:
                            if ( _percent == 1 ) {
                                _value = 0;
                            } else {
                                k = _percent;
                                _value = 1 - pow( 2, -10 * k );
                            }
                            break;
                            
                        case TWEEN_INTERPOLATION_BACK:
                            s = 1.70158 * 1.525;
                            k = _percent * 2;
                            if ( k < 1 ) {
                                _value = 0.5 * ( k * k * ( (s + 1) * k - s ) );
                            } else {
                                k -= 2;
                                _value = 0.5 * ( k * k * ( (s + 1) * k + s ) + 2 );
                            }
                            break;
                            
                        case TWEEN_INTERPOLATION_BACK_IN:
                            s = 1.70158;
                            _value = ( _percent * _percent * ( (s + 1) * _percent - s ) );
                            break;
                            
                        case TWEEN_INTERPOLATION_BACK_OUT:
                            s = 1.70158;
                            k = _percent;
                            _value = --k * k * ((s + 1) * k + s) + 1;
                            break;
                            
                        case TWEEN_INTERPOLATION_ELASTIC_IN:
                            if ( _percent == 0 || _percent == 1 ) {
                                _value = _percent;
                            } else {
                                a = 0.1;
                                p = 0.4;
                                if ( !a || a < 1 ) {
                                    a = 1;
                                    s = p / 4;
                                } else {
                                    s = p * asin(1 / a) / ( 2 * M_PI );
                                }
                                k = _percent  - 1;
                                _value = -( a * pow( 2, 10 * k ) * sin( ( k - s ) * ( 2 * M_PI) / p ) );
                            }
                            break;
                            
                        case TWEEN_INTERPOLATION_ELASTIC_OUT:
                            if ( _percent == 0 || _percent == 1 ) {
                                _value = _percent;
                            } else {
                                a = 0.1;
                                p = 0.4;
                                if ( !a || a < 1 ) {
                                    a = 1;
                                    s = p / 4;
                                } else {
                                    s = p * asin( 1 / a ) / ( 2 * M_PI );
                                }
                                k = _percent;
                                _value = a * pow( 2, - 10 * k ) * sin( ( k - s ) * ( 2 * M_PI ) / p ) + 1;
                            }
                            break;
                            
                        case TWEEN_INTERPOLATION_BOUNCE_IN:
                        case TWEEN_INTERPOLATION_BOUNCE_OUT:
                            k = _percent;
                            if ( TweenConfig<TweenType>::_interpolation == TWEEN_INTERPOLATION_BOUNCE_IN ) {
                                k = 1 - k;
                            }
                            if ( k < ( 1 / 2.75 ) ) {
                                k = 7.5625 * k * k;
                            } else if ( k < ( 2 / 2.75 ) ) {
                                k = 7.5625 * ( k -= ( 1.5 / 2.75 ) ) * k + 0.75;
                            } else if ( k < ( 2.5 / 2.75 ) ) {
                                k = 7.5625 * ( k -= ( 2.25 / 2.75 ) ) * k + 0.9375;
                            } else {
                                k = 7.5625 * ( k -= ( 2.625 / 2.75 ) ) * k + 0.984375;
                            }
                            if ( TweenConfig<TweenType>::_interpolation == TWEEN_INTERPOLATION_BOUNCE_IN ) {
                                _value = 1 - k;
                            } else {
                                _value = k;
                            }
                            break;
                            
                        default:
                            break;
                            
                    }

                    fire( TWEEN_EVENT_CHANGE, now );

                }

            }

            return _alive;

        }
        
        void apply() {
            
            if ( _target_ptr == nullptr ) return;

            *( (TweenType*) _target_ptr ) = 
                TweenConfig<TweenType>::_start_value + 
                _value * ( 
                    TweenConfig<TweenType>::_max - 
                    TweenConfig<TweenType>::_start_value
                );
                
        }
        
        bool alive() { 
            return _alive;
        }
        bool active() { 
            return _active;
        }
        double percent() { 
            return _percent;
        }
        double value() { 
            return _value;
        }
        void * get_target_ptr() { 
            return _target_ptr;
        }
        
        // do nothing by default
        // see float and double specialisation
        void validateRotations() {
            std::cerr << "tween::Tween_<" << typeid( TweenType ).name() << ">::validateRotations(), impossible to consider this type as a rotation" << std::endl;
        };
    
    protected:
        
        void fire( TweenEventType t, double now = 0 ) {
            TweenEvent twe;
            twe.type = t;
            twe.time = now;
            twe.counter = _play_count;
            twe.tweener = nullptr;
            twe.tween = this;
            twe.ptr = _target_ptr;
            TweenCaller::fire( twe );
        }
        
    private:
        
        TweenType * _target_ptr;
        
        // time in nanos, by opposite to TweenConfig
        // where it is defined in millis
        double _starttime;
        double _endtime;
        double _offset;
        double _duration;
        bool _alive;
        bool _active;
        double _percent;
        double _value;
        uint16_t _play_count;
        
    };
    
    // specialisation, see .cpp for implementation
    template <> void Tween_<bool>::apply();
    template <> void Tween_<std::string>::apply();
    template <> void Tween_<Callback>::apply();
    template <> void Tween_<CallbackValue>::apply();
    
    template <> void Tween_<float>::validateRotations();
    template <> void Tween_<double>::validateRotations();
    
}
#endif /* TWEEN_H */

