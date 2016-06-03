/* This file is part of Tween, a C++ template library for animation.
* 
* Tween is free library: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
*/

/* 
 * File:   TweenListener.h
 * Author: frankiezafe
 *
 * Created on May 9, 2016, 12:37 PM
 */

#ifndef TWEENLISTENER_H
#define TWEENLISTENER_H

#include "Common.h"

namespace tween {

    // ABSTRACT FOR ANY EXTERNAL CLASSES
    class TweenListener {
    public:
        virtual void tweenEvent( TweenEvent & evt ) = 0;
    };
}

#endif /* TWEENLISTENER_H */

