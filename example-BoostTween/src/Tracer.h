/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tracer.h
 * Author: frankiezafe
 *
 * Created on May 18, 2016, 5:34 PM
 */

#ifndef TRACER_H
#define TRACER_H

#include "ofMain.h"
#define TRACER_MARGIN 0.2f

template< typename TweenType >
class Tracer {

public:
    Tracer() {
        traced = nullptr;
        current = 0;
        num = 0;
        data = nullptr;
        previous = 0;
        min = 0;
        max = 1;
        x_div = 1;
        y_div = 1;
    }
    
    ~Tracer() {
        delete [] data;
    }
    
    void setName( string name ) {
        this->name = name;
    }
    
    void init( TweenType * value, unsigned int capacity, float min, float max ) {
        traced = value;
        current = 0;
        num = capacity;
        data = new float[ num ];
        for ( unsigned int i = 0; i < num; ++i ) {
           data[ i ] = 0; 
        }
        previous = (*traced);
        this->min = min;
        this->max = max;
        y_div = 1.f / ( this->max - this->min );
        x_div = 1.f / num;
    }
    
    void update( bool testchange = false ) {
        if ( !testchange || ( testchange && previous != (*traced) ) ) {
            if ( current >= num ) {
                for ( unsigned int i = 1; i < num; ++i ) {
                    data[ i - 1 ] = data[ i ];
                }
                current = num - 1;
            }
            data[ current ] = ( (*traced) - this->min ) * this->y_div;
            if ( current < num ) {
                ++current;
            }
            previous = (*traced);
        }
    }
    
    void draw( float x, float y, float width, float height ) {
        glPushMatrix();
        glTranslatef( x, y + height, 0 );
        glScalef( width, -height, 1 );
        glColor4f( 1,1,1,0.5 );
        glBegin( GL_QUADS );
            glVertex2f( 0,0 );
            glVertex2f( 0,1 );
            glVertex2f( 1,1 );
            glVertex2f( 1,0 );
        glEnd();
        glBegin( GL_LINES );
            glVertex2f( 0, TRACER_MARGIN );
            glVertex2f( 1, TRACER_MARGIN );
            glVertex2f( 0, 1 - TRACER_MARGIN );
            glVertex2f( 1, 1 - TRACER_MARGIN );
        glEnd();
        glPopMatrix();
        glPushMatrix();
        glTranslatef( x, y + height - height * TRACER_MARGIN, 0 );
        glScalef( width, -height * ( 1 - ( TRACER_MARGIN * 2 ) ), 1 );
        glColor4f( 1,0,0,1 );
        glBegin( GL_LINES );
        for ( unsigned int i = 1; i < current; ++i ) {
            glVertex2f( ( i - 1 ) * x_div, data[ i - 1 ] );
            glVertex2f( i * x_div, data[ i ] );
        }
        glEnd();
        glPopMatrix();
        glColor4f( 1,1,1,1 );
        ofDrawBitmapString( name, x + 5, y + 15 );
    }
    
    inline string getName() {
        return name;
    }
    
protected:
    string name;
    TweenType * traced;
    float previous;
    unsigned int current;
    unsigned int num;
    float * data;
    float min;
    float max;
    float x_div;
    float y_div;
    
};
#endif /* TRACER_H */

