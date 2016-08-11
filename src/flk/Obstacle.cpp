//this is the obstacle's sourcefile

#include "Obstacle.h"

Obstacle::Obstacle(int siz, ofVec2f pos) {
    position = pos;
    size = siz;
}


void Obstacle::draw() {
    
    //this obstacle can draw itself
    
    ofNoFill();
    ofDrawEllipse(position.x, position.y, size, size);
    
}