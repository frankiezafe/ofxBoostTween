/* This file is part of Tween, a C++ template library for animation.
* 
* Tween is free library: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
*/

/* 
 * File:   TweenCaller.h
 * Description: ABSTRACT FOR INTERNAL CLASSES ONLY
 * Author: frankiezafe
 *
 * Created on May 10, 2016, 12:21 AM
 */

#ifndef TWEENCALLER_H
#define TWEENCALLER_H

#include "Common.h"
#include "TweenListener.h"

namespace tween {

    class TweenCaller {
    public:
        
        virtual bool addListener( TweenListener * tl ) {
            for ( auto atl : listeners) {
                if ( atl == tl ) {
                    return false;
                }
            }
            listeners.push_back( tl );
            return true;
        };
        
        virtual bool cloneListener( std::vector< TweenListener * > & list ) {
            removeAllListener();
            // efficient copy method
            std::copy( list.begin(), list.end(), std::back_inserter( listeners ));
            return true;
        };
        
        virtual bool removeListener( TweenListener * tl ) {
            int i = 0;
            for ( auto atl : listeners ) {
                if ( atl == tl ) {
                    listeners.erase( listeners.begin() + i );
                    return true;
                }
                ++i;
            }
            return false;
        };
        
        virtual bool removeAllListener() {
            if ( !listeners.empty() ) {
                listeners.clear();
                return true;
            }
            return false;
        };
        
    protected:
        
        virtual void fire( TweenEvent & evt ) {
            for ( auto atl : listeners ) {
                atl->tweenEvent( evt );
            }
        }
        
        std::vector< TweenListener * > listeners;
        
    };
    
};

#endif /* TWEENCALLER_H */

