/* This file is part of Tween, a C++ template library for animation.
 * 
 * Tween is free library: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 */

/* 
 * File:   Common.h
 * Author: frankiezafe
 *
 * Created on May 9, 2016, 12:58 PM
 */

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <map>
#include <vector>
#include <math.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>  

#define _USE_MATH_DEFINES

#define MILLIS2NANOS        1000000.0
#define SECONDS2NANOS       1000000000.0
#define MILLIS2MIRCOS       1000.0
#define SECONDS2MIRCOS      1000000.0

namespace tween {

    enum TweenEventType {
        TWEEN_EVENT_UNDEFINED =                 0,
        TWEEN_EVENT_START =                     1,
        TWEEN_EVENT_STOP =                      2,
        TWEEN_EVENT_CHANGE =                    3,
        TWEEN_EVENT_PROCESSDONE =               4,
        TWEEN_EVENT_ALL_TWEENS_DONE =           5,
        TWEEN_EVENT_LOOP =                      6,
        TWEEN_EVENT_LAST =                      7,
        TWEEN_EVENT_UPDATE =                    8,
        TWEEN_EVENT_ERROR =                     100
    };

    enum TweenInterpolation {
        TWEEN_INTERPOLATION_CONSTANT =          0,
        TWEEN_INTERPOLATION_LINEAR =            1,
        TWEEN_INTERPOLATION_SINUS =             2,
        TWEEN_INTERPOLATION_SINUS_IN =          3,
        TWEEN_INTERPOLATION_SINUS_OUT =         4,
        TWEEN_INTERPOLATION_QUADRATIC =         5,
        TWEEN_INTERPOLATION_QUADRATIC_IN =      6,
        TWEEN_INTERPOLATION_QUADRATIC_OUT =     7,
        TWEEN_INTERPOLATION_CUBIC =             8,
        TWEEN_INTERPOLATION_CUBIC_IN =          9,
        TWEEN_INTERPOLATION_CUBIC_OUT =         10,
        TWEEN_INTERPOLATION_EXPONENTIAL =       11,
        TWEEN_INTERPOLATION_EXPONENTIAL_IN =    12,
        TWEEN_INTERPOLATION_EXPONENTIAL_OUT =   13,
        TWEEN_INTERPOLATION_BACK =              14,
        TWEEN_INTERPOLATION_BACK_IN =           15,
        TWEEN_INTERPOLATION_BACK_OUT =          16,
        TWEEN_INTERPOLATION_ELASTIC_IN =        17,
        TWEEN_INTERPOLATION_ELASTIC_OUT =       18,
        TWEEN_INTERPOLATION_BOUNCE_IN =         19,
        TWEEN_INTERPOLATION_BOUNCE_OUT =        20,
        TWEEN_INTERPOLATION_COUNT =             21,
        TWEEN_INTERPOLATION_UNDEFINED =         22
    };

    enum TweenLoop {
        TWEEN_LOOP_ONCE,
        TWEEN_LOOP_REPEAT,
        TWEEN_LOOP_PALINDROME
    };
    
    enum TweenStack {
        TWEEN_STACK_DELAY,          // default
        TWEEN_STACK_AVOID,          // refuse new tween if pointer is already binded to a tween
        TWEEN_STACK_DISABLED        // no stack management, use at your own risk!
    };

    struct TweenEvent {
        
        TweenEventType type;
        uint64_t time;
        uint16_t counter;
        void * tweener;
        void * tween;
        const void * ptr;
        
        TweenEvent() {
            type = TWEEN_EVENT_UNDEFINED;
            tweener = nullptr;
            tween = nullptr;
            ptr = nullptr;
            counter = 0;
            time = 0;
        };
        
    };

    // only static methods here!
    // hidden singleton based Kerry Barnes idea, http://www.codeguru.com/cpp/cpp/cpp_mfc/singletons/article.php/c4005/Static-Parameter-Class.htm

    class TweenUtils {
    public:

        static TweenInterpolation invertInterpolation(TweenInterpolation ti) {
            if (getInstance().interpolation_opposites.find(ti) == getInstance().interpolation_opposites.end()) {
                return TWEEN_INTERPOLATION_UNDEFINED;
            }
            return getInstance().interpolation_opposites[ ti ];
        }

        static std::string interpolationToString(TweenInterpolation i) {
            switch (i) {
                case TWEEN_INTERPOLATION_CONSTANT:
                    return "constant";
                case TWEEN_INTERPOLATION_LINEAR:
                    return "linear";
                case TWEEN_INTERPOLATION_SINUS:
                    return "sinus";
                case TWEEN_INTERPOLATION_SINUS_IN:
                    return "sinus in";
                case TWEEN_INTERPOLATION_SINUS_OUT:
                    return "sinus out";
                case TWEEN_INTERPOLATION_QUADRATIC:
                    return "quadratic";
                case TWEEN_INTERPOLATION_QUADRATIC_IN:
                    return "quadratic in";
                case TWEEN_INTERPOLATION_QUADRATIC_OUT:
                    return "quadratic out";
                case TWEEN_INTERPOLATION_CUBIC:
                    return "cubic";
                case TWEEN_INTERPOLATION_CUBIC_IN:
                    return "cubic in";
                case TWEEN_INTERPOLATION_CUBIC_OUT:
                    return "cubic out";
                case TWEEN_INTERPOLATION_EXPONENTIAL:
                    return "exponential";
                case TWEEN_INTERPOLATION_EXPONENTIAL_IN:
                    return "exponential in";
                case TWEEN_INTERPOLATION_EXPONENTIAL_OUT:
                    return "exponential out";
                case TWEEN_INTERPOLATION_BACK:
                    return "back";
                case TWEEN_INTERPOLATION_BACK_IN:
                    return "back in";
                case TWEEN_INTERPOLATION_BACK_OUT:
                    return "back out";
                case TWEEN_INTERPOLATION_ELASTIC_IN:
                    return "elastic in";
                case TWEEN_INTERPOLATION_ELASTIC_OUT:
                    return "elastic out";
                case TWEEN_INTERPOLATION_BOUNCE_IN:
                    return "bounce in";
                case TWEEN_INTERPOLATION_BOUNCE_OUT:
                    return "bounce out";
                default:
                    return "undefined";
            }
        }
            
        static std::string tweenTypetoString( TweenEventType t ) {
            switch ( t ) {
                case TWEEN_EVENT_ERROR:
                    return "error";
                case TWEEN_EVENT_UNDEFINED:
                    return "undefined";
                case TWEEN_EVENT_START:
                    return "start";
                case TWEEN_EVENT_STOP:
                    return "stop";
                case TWEEN_EVENT_CHANGE:
                    return "change";
                case TWEEN_EVENT_PROCESSDONE:
                    return "process done";
                case TWEEN_EVENT_ALL_TWEENS_DONE:
                    return "all tweens done";
                case TWEEN_EVENT_LOOP:
                    return "loop";
                case TWEEN_EVENT_LAST:
                    return "last";
                case TWEEN_EVENT_UPDATE:
                    return "update";
                default:
                    return "[error, event type undefined]";
            }
        }

        static double reduceToPI( double v, double & count ) {
            double a = v;
            while( a > M_PI ) {
                a -= M_PI * 2;
                ++count;
            }
            while( a < -M_PI ) {
                a += M_PI * 2;
                --count;
            }
            return a;
        }
        
        static double add2PI( double v, double count ) {
            return v + M_PI * 2 * count;
        }
        
    protected:

        std::map< TweenInterpolation, TweenInterpolation > interpolation_opposites;

        TweenUtils() {
            interpolation_opposites[ TWEEN_INTERPOLATION_CONSTANT ] = TWEEN_INTERPOLATION_CONSTANT;
            interpolation_opposites[ TWEEN_INTERPOLATION_LINEAR ] = TWEEN_INTERPOLATION_LINEAR;
            interpolation_opposites[ TWEEN_INTERPOLATION_SINUS ] = TWEEN_INTERPOLATION_SINUS;
            interpolation_opposites[ TWEEN_INTERPOLATION_SINUS_IN ] = TWEEN_INTERPOLATION_SINUS_OUT;
            interpolation_opposites[ TWEEN_INTERPOLATION_SINUS_OUT ] = TWEEN_INTERPOLATION_SINUS_IN;
            interpolation_opposites[ TWEEN_INTERPOLATION_QUADRATIC ] = TWEEN_INTERPOLATION_QUADRATIC;
            interpolation_opposites[ TWEEN_INTERPOLATION_QUADRATIC_IN ] = TWEEN_INTERPOLATION_QUADRATIC_OUT;
            interpolation_opposites[ TWEEN_INTERPOLATION_QUADRATIC_OUT ] = TWEEN_INTERPOLATION_QUADRATIC_IN;
            interpolation_opposites[ TWEEN_INTERPOLATION_CUBIC ] = TWEEN_INTERPOLATION_CUBIC;
            interpolation_opposites[ TWEEN_INTERPOLATION_CUBIC_IN ] = TWEEN_INTERPOLATION_CUBIC_OUT;
            interpolation_opposites[ TWEEN_INTERPOLATION_CUBIC_OUT ] = TWEEN_INTERPOLATION_CUBIC_IN;
            interpolation_opposites[ TWEEN_INTERPOLATION_EXPONENTIAL ] = TWEEN_INTERPOLATION_EXPONENTIAL;
            interpolation_opposites[ TWEEN_INTERPOLATION_EXPONENTIAL_IN ] = TWEEN_INTERPOLATION_EXPONENTIAL_OUT;
            interpolation_opposites[ TWEEN_INTERPOLATION_EXPONENTIAL_OUT ] = TWEEN_INTERPOLATION_EXPONENTIAL_IN;
            interpolation_opposites[ TWEEN_INTERPOLATION_BACK ] = TWEEN_INTERPOLATION_BACK;
            interpolation_opposites[ TWEEN_INTERPOLATION_BACK_IN ] = TWEEN_INTERPOLATION_BACK_OUT;
            interpolation_opposites[ TWEEN_INTERPOLATION_BACK_OUT ] = TWEEN_INTERPOLATION_BACK_IN;
            interpolation_opposites[ TWEEN_INTERPOLATION_ELASTIC_IN ] = TWEEN_INTERPOLATION_ELASTIC_OUT;
            interpolation_opposites[ TWEEN_INTERPOLATION_ELASTIC_OUT ] = TWEEN_INTERPOLATION_ELASTIC_IN;
            interpolation_opposites[ TWEEN_INTERPOLATION_BOUNCE_IN ] = TWEEN_INTERPOLATION_BOUNCE_OUT;
            interpolation_opposites[ TWEEN_INTERPOLATION_BOUNCE_OUT ] = TWEEN_INTERPOLATION_BOUNCE_IN;
        }

        static TweenUtils & getInstance() {
            static TweenUtils _instance;
            return _instance;
        }

    };
    
    class Callback {
        
    public:
        
        Callback() {
            declared = false;
        }
        Callback( boost::function<void()> f ) {
            wrapper = f;
            declared = true;
        };
        virtual void call() {
            if ( !declared ) {
                std::cerr << "tween::TweenCallback::apply(), callback method has not been declared!" << std::endl;
            }
            wrapper();
        }
    protected:
        
        boost::function<void()> wrapper;
        bool declared;
        
    };
    
    class CallbackValue {
        
    public:
        
        CallbackValue() {
            declared = false;
        }
        CallbackValue( boost::function<void(double)> f ) {
            wrapper = f;
            declared = true;
        };
        virtual void call( double value ) {
            if ( !declared ) {
                std::cerr << "tween::TweenCallback::apply(), callback method has not been declared!" << std::endl;
            }
            wrapper( value );
        }
    protected:
        
        boost::function<void(double)> wrapper;
        bool declared;
        
    };
    
};

#endif /* COMMON_H */

