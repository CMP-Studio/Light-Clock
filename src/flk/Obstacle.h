#pragma once

#include "ofMain.h"

class Obstacle {
public:
    
    Obstacle(int siz, ofVec2f pos); //constructor (has no type)
    void draw();

    ofVec2f position;
    int size;
    
};