/* This file is part of Tween, a C++ template library for animation.
* 
* Tween is free library: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
*/

/* 
 * File:   Tweener.h
 * Author: frankiezafe
 *
 * Created on May 9, 2016, 8:55 AM
 */


#ifndef TWEENER_H
#define TWEENER_H

#include <mutex>
#include <map>

#include "TweenClock.h"
#include "Tween.h"

namespace tween {

    class Tweener: public TweenClock, public TweenCaller {
    public:

        Tweener( uint16_t frequency_millis, bool use_thread = true );
        virtual ~Tweener();
        
        // TweenClock overload
        virtual bool start();
        virtual void stop();
        virtual void update();

        // play / pause
        inline bool playing() { return !_paused; }
        inline bool togglePlay() { _paused = !_paused; return !_paused; }
        
        // tweens specific
        void addTween( uint16_t time_offset_millis, uint16_t duration_millis );
        void addTween( const TweenConfig<double> & conf );
        
        template< typename TweenType >
        void addTween( const TweenConfig<TweenType> & conf, TweenType * ptr ) {
            
            // temp config for delays
            TweenConfig<TweenType> innerconf; 
            innerconf = conf;
            
            // depends on last _tween_map[ptr]
            // getting the last tween for this ptr
            if ( _tween_map.find( ptr ) != _tween_map.end() ) {
                Tween_<TweenType> * tw = (Tween_<TweenType> *) ( _tween_map[ptr].back() );
                switch( tw->stack ) {
                    case TWEEN_STACK_AVOID:
                        std::cerr << "tween:Tweener::addTween, last tween for pointer (" << typeid(TweenType).name() << ") " << ptr << " is of type TWEEN_STACK_AVOID" << std::endl;
                        return;
                    case TWEEN_STACK_DELAY:
                        innerconf.setOffset(
                                innerconf.offset_millis +
                                tw->offset_millis + 
                                tw->duration_millis
                                );
                        std::cout << innerconf.offset_millis << " > tween delayed of " << ( innerconf.offset_millis - conf.offset_millis ) << " millis" << std::endl;
                        break;
                    case TWEEN_STACK_DISABLED:
                    default:
                        // no management of stack...
                        break;
                }
            }
            
            Tween_<TweenType> * tw = new Tween_<TweenType>( ptr, innerconf, _innertime );
            tw->cloneListener( listeners );

            _tween_map[ ptr ].push_back( tw );
            
//            std::cout << 
//                    "Tweener::addTween<" << 
//                    typeid( ptr ).name() << 
//                    ">, entries for " << 
//                    ptr << 
//                    ": " << 
//                    _tween_map[ ptr ].size() << 
//                    std::endl;

            _job_mutex.lock();
            _tweens_to_append.push_back( tw );
            _job_mutex.unlock();
            
        }
        
        // stopping all tweens for a specific pointer
        template< typename TweenType >
        void removeTween( const void * ptr ) {
            
            _job_mutex.lock();
            
            std::vector< Tween * > alives;
            std::vector< Tween * > zombies;
            
            // cleaning tweens already running
            for ( Tween * atw : _tweens ) {
                if ( atw->get_target_ptr() == ptr ) {
                    zombies.push_back( atw );
                } else {
                    alives.push_back( atw );
                }
            }
            _tweens.clear();
            _tweens.swap( alives );
            alives.clear();
            
            // cleaning pending tweens
            for ( Tween * atw : _tweens_to_append ) {
                if ( atw->get_target_ptr() == ptr ) {
                    zombies.push_back( atw );
                } else {
                    alives.push_back( atw );
                }
            }
            _tweens.clear();
            _tweens.swap( alives );
            clearZombies( zombies );
            
            _job_mutex.unlock();
            
        }
        
        unsigned int getTweenCount( const void * ptr ) {
            if ( _tween_map.find( ptr ) == _tween_map.end() ) {
                return 0;
            }
            return _tween_map[ ptr ].size();
        }
        
        // stop all tweens
        void killall();
        
        //SETTERS
        inline void enableJobdoneEvents( bool e ) { _jobdoneEventsEnabled = e; }
        
        // GETTERS
        int activeTweens();
        
        // TweenCaller overload
        virtual bool addListener( TweenListener * tl );
        virtual bool removeListener( TweenListener * tl );
        virtual bool removeAllListener( TweenListener * tl );
        
    protected:
        
        virtual void process();
        void clearZombies( std::vector< Tween * > & zombies );
        void fire( TweenEventType t, const void * ptr = nullptr );
        
    private:
        
        // avoid changing listeners list while firing
        boost::mutex _fire_mutex;
        
        TweenConfig<double> _defaultconfig;
        std::vector< Tween * > _tweens;
        std::vector< Tween * > _tweens_to_append;
        std::map< const void *, std::vector< Tween * > > _tween_map;
        
        bool _thread_enabled;
        bool _jobdoneEventsEnabled;
        bool _paused;

    };

}
#endif /* TWEENER_H */

