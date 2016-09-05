/*
 *  Boid.h
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *  Updated by Takara Hokao
 *	Updated by Andrew Monks
 *
 */

#pragma once

#include "ofMain.h"
#include "Boid_mod.h"
#include "Obstacle.h"

class Flocking {
public:
	void update();
    void setup(int width, int height);
	void draw();
    void triggerSequence();
    void triggerSequenceTwo(int duration);
    ofFbo drawIntoMe;
    void setMinSize(int sz);
    bool isSequenceTwo;
    
private:
	void addBoid();
	void addBoid(int x, int y);
    void removeBoid(int x, int y, int radius);
    void removeAllBoids();
    int flockSize();
    void toCircle();
    void toTriangle();
    void toAppear();
    void toDisappear();
    
	vector<Boid_mod> boids;
    int startCount = 200;
    
    vector<Obstacle> obstacles;
    
    int startTime;
    bool isSequence;
    
    int sequenceStep;
    int lengthOfMoment;
    
    
};
