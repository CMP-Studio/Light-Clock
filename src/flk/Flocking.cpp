/*
 *  Boid.h
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *  Updated by Takara Hokao
 *  Updated by Andrew Monks
 *
 */

#include "Flocking.h"

void Flocking::setup(int width, int height){
    drawIntoMe.allocate(width, height, GL_RGBA);
    
    drawIntoMe.begin();
    ofClear(255,255,255,0);
    drawIntoMe.end();
    
    // obstacle size.
    int size = 500;
    // add four buffers around the corners
    obstacles.push_back(Obstacle(size, ofVec2f(0,0)));
    obstacles.push_back(Obstacle(size, ofVec2f(width,0)));
    obstacles.push_back(Obstacle(size, ofVec2f(width,height)));
    obstacles.push_back(Obstacle(size, ofVec2f(0,height)));
    
}

void Flocking::draw() {
    
   

    
    //drawIntoMe.draw(0,0);
   
    
    
}

void Flocking::update() {

    
    if (isSequence){
        int timeSince = ofGetElapsedTimeMillis() - startTime;
        if ((sequenceStep == 3)&(timeSince >= 47000)){
            isSequence = false;
             removeAllBoids();
        }
        else if ((sequenceStep == 2)&(timeSince > 43000)){
             toDisappear();
            sequenceStep ++;
        }
        else if ((sequenceStep == 1)&(timeSince > 30000)){
             toTriangle();
            sequenceStep ++;
        }
        else if ((sequenceStep == 0)&(timeSince > 5000)){
             toCircle();
            sequenceStep ++;
        }
    }

    
    for(int i = 0; i < boids.size(); i++) {
        boids[i].update(boids);
    }
    
    //boids avoid all obstacles
    for(int i=0; i<boids.size(); i++) {
        for(int j=0; j<obstacles.size(); j++) {
            ofPoint obsPos = obstacles[j].position;
            ofPoint boidPos = boids[i].loc;
            float size = obstacles[j].size;
            float dist = ofDist(obsPos.x, obsPos.y, boidPos.x, boidPos.y);
            
            float fear = boids[i].maximumWidth/dist;   //based on obs size and distance
            if (dist< boids[i].maximumWidth) {
                if (dist < boids[i].maximumWidth/2){
                    boids[i].pushOut(ofVec2f(drawIntoMe.getWidth()/2, drawIntoMe.getHeight()/2), 1);
                }
                boids[i].pushOut(ofVec2f(drawIntoMe.getWidth()/2, drawIntoMe.getHeight()/2), fear);
            } else {
                // boids[i].avoid(obstacles[j].position, fear);  //fear between 0..1
            }
        }
    }
    drawIntoMe.begin();
    ofClear(0,0,0,255);
    for(int i = 0; i < boids.size(); i++) {
        boids[i].draw();
    }
    drawIntoMe.end();
}

void Flocking::triggerSequence(){
    isSequence = true;
    startTime = ofGetElapsedTimeMillis();
    sequenceStep = 0;
    removeAllBoids();
    
    for (int i = 0; i < startCount; i++) {
        addBoid();
    }
    
    toAppear();
}

void Flocking::addBoid() {
    boids.push_back(Boid_mod(drawIntoMe.getWidth(), drawIntoMe.getHeight()));
}


void Flocking::removeAllBoids() {
    boids.clear();

}

void Flocking::removeBoid(int x, int y, int radius) {
    int i;
    for (i=0; i<boids.size(); i++) {
        if(boids[i].isHit(x, y, radius)) {
            boids.erase(boids.begin()+i);
        }
    }
}

int Flocking::flockSize() {
    return boids.size();
}

void Flocking::toCircle(){
    for(int i = 0; i < boids.size(); i++) {
        boids[i].toCirc();
    }
}


void Flocking::toTriangle(){
    for(int i = 0; i < boids.size(); i++) {
        boids[i].toTri();
    }
}


void Flocking::toAppear(){
    for(int i = 0; i < boids.size(); i++) {
        boids[i].appear();
    }
}


void Flocking::toDisappear(){
    for(int i = 0; i < boids.size(); i++) {
        boids[i].disappear();
    }
}