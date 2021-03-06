# ofxBoostTween #

This addon allows you to animate any kind of object in openframeworks. Based on [Tween](https://bitbucket.org/frankiezafe/tween)

It supports all types (int, bool, float, etc.) and all structures and objects where '+', '-', '*' and '=' operators are overloaded (ofVec2f for instance).

*   Quaternions and Matrices tweens, see example-BoostTween/src/demoAdvanced
*   Callback, see example-BoostTween/src/demoCallback

See videos for demos:

*   basic features: https://vimeo.com/167244047
*   ofQuaternion and ofMatrix4x4: https://vimeo.com/168015899

Have fun!

## features ##

*   repeat count or forever
*   palindrome mode
*   event system
*   20 types of interpolation
*   easy tweens accumulation on the same pointer
*   handle boolean, string, and callbacks
*   time speed control (slow motion or fast forward) - negative time is not handled
*   asynchronous or synchronous
*   reusable configurations of tweens
*   destruction of tweens while running

## dependencies ##

Tweener package is fully independant from openframeworks and rely on boost library for threading and callbacks management.

### installation on linux/debian ###

    $ sudo apt-get install libboost1.54-dev // or higher

## running the examples ##

Once boost is installed, copy the 'example-BoostTween' in your app folder.

In terminal:

    $ make
    $ cd bin
    $ ./example-BoostTween

To launch the demos, specify with param '-d', for instance:

    $ ./example-BoostTween -d float

For the float demo, or:

    $ ./example-BoostTween -d rotation

(you got it, i guess...)
