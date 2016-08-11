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


#include "ofMain.h"
#include "singleBoid.h"
//#include "ofxVectorMath.h"

class Boid_mod {
public:
    
	Boid_mod(int wid, int height);
	
	void update(vector<Boid_mod> &boids);
	void draw();
	
    void seek(ofVec2f target);
    void avoid(ofVec2f target, float fear);
    void pushOut(ofVec2f target, float fear);
    void arrive(ofVec2f target);
	
    void flock(vector<Boid_mod> &boids);
    bool isHit(int x,int y, int radius);
    
    void toCirc();
    void toTri();
    
    void appear();
    void disappear();
    
    void setup();
    
	ofVec2f steer(ofVec2f target, bool slowdown);
	ofVec2f separate(vector<Boid_mod> &boids);
	ofVec2f align(vector<Boid_mod> &boids);
	ofVec2f cohesion(vector<Boid_mod> &boids);
	ofVec2f loc,vel,acc;
    
	float r;
	float maxforce;
	float maxspeed;
    
    singleBoid customBoid;
    
    int maximumWidth;
    bool isDoubleX;
    bool isDoubleY;
    ofVec2f loc2;
    
    int fboWidth;
    int fboHeight;
    
    float cohere;
    float alignment;
    float seperation;
    
    bool isSeeking;
    int seekHeight; 
    
        
};

