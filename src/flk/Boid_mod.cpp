/*
 *  Boid.cpp
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *  Updated by Takara Hokao
 *	Updated by Andrew Monks
 *
 */

#include "Boid_mod.h"



void Boid_mod::setup(){
   	vel.set(0,0);
    acc.set(0,0);
    //r = 1000;
    maxspeed = 2.5;
    maxforce = 0.3;
    customBoid.setup();
    customBoid.maxSpeed = maxspeed;
    maximumWidth = 500;
    isDoubleX = false;
    isDoubleY = false;
    cohere = .3;
    alignment = .2;
    seperation = .38;

}

Boid_mod::Boid_mod(int wid, int height) {
    
    fboWidth = wid;
    fboHeight = height;
    
    loc.set( ofRandom(1, fboWidth), fboHeight/2  + ofRandom(-100, 100));
    loc2 = loc;
    setup();
}



// Method to update location
void Boid_mod::update(vector<Boid_mod> &boids) {
    
    customBoid.update();
    
    maxspeed = customBoid.maxSpeed;
    
	flock(boids);
	
    vel += acc;   // Update velocity
    vel.x = ofClamp(vel.x, -maxspeed, maxspeed);  // Limit speed
	vel.y = ofClamp(vel.y, -maxspeed, maxspeed);  // Limit speed
    loc += vel;
    acc.set(0,0);  // Reset accelertion to 0 each cycle
	
    // this is where wrapping happens and it is not enough!
    isDoubleY = false;
    isDoubleX = false;
    loc2 = loc;
    
    // too far right, it is on the seam
    if ((loc.x + maximumWidth/2 >=  fboWidth) & ((loc.x - maximumWidth/2) <  fboWidth) ){
        loc2.x =loc.x -  fboWidth;
        isDoubleX = true;
    }
    // it has gone past the seam
    else if ((loc.x - maximumWidth/2) >=  fboWidth){
        loc.x = maximumWidth/2;
        isDoubleX = false;
    }
    // this is for when it has gone too far to the left
    else if ((loc.x-maximumWidth/2 <= 0)& (loc.x + maximumWidth/2 > 0) ){
        loc2.x =loc.x +  fboWidth;
        isDoubleX = true;
    }
    // it has gone past the seam
    else if ((loc.x + maximumWidth/2 <= 0)){
        isDoubleX = false;
        loc.x =  fboWidth - maximumWidth/2;
    }
    
    // same but for y-axis
    if ((loc.y + maximumWidth/2 >=  fboHeight) & ((loc.y - maximumWidth/2) <  fboHeight) ){
        loc2.y =loc.y -  fboHeight;
        isDoubleY = true;
    }
    else if ((loc.y - maximumWidth/2) >=  fboHeight){
        loc.y = maximumWidth/2;
        isDoubleY = false;
    }
    else if ((loc.y-maximumWidth/2 <= 0)& (loc.y + maximumWidth/2 > 0) ){
        loc2.y =loc.y +  fboHeight;
        isDoubleY = true;
    }
    else if ((loc.y + maximumWidth/2 <= 0)){
        isDoubleY = false;
        loc.y =  fboHeight - maximumWidth/2;
    }
    
    /*
	if (loc.x < -r) loc.x =  fboWidth+r;
    if (loc.y < -r) loc.y =  fboHeight+r;

    if (loc.x >  fboWidth+r) loc.x = -r;
    if (loc.y >  fboHeight+r) loc.y = -r;
     */
    
    if (isSeeking){
        seek(ofVec2f(fboWidth/2, seekHeight));
    }
    
}

void Boid_mod::seek(ofVec2f target) {
    acc += steer(target, false);
}

void Boid_mod::avoid(ofVec2f target, float fear) {
    acc -= steer(target, false) * fear;
}

void Boid_mod::pushOut(ofVec2f target, float fear) {
    acc += steer(target, false) * fear;
}

void Boid_mod::arrive(ofVec2f target) {
    acc += steer(target, true);
}

// A method that calculates a steering vector towards a target
// Takes a second argument, if true, it slows down as it approaches the target
ofVec2f Boid_mod::steer(ofVec2f target, bool slowdown) {
    ofVec2f steer;  // The steering vector
    ofVec2f desired = target - loc;  // A vector pointing from the location to the target
    
	float d = ofDist(target.x, target.y, loc.x, loc.y); // Distance from the target is the magnitude of the vector
	
	
	// If the distance is greater than 0, calc steering (otherwise return zero vector)
    if (d > 0) {
		
		desired /= d; // Normalize desired
		// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
		if ((slowdown) && (d < 700.0f)) {
			desired *= maxspeed * (d/700.0f); // This damping is somewhat arbitrary
		} else {
			desired *= maxspeed;
		}
		// Steering = Desired minus Velocity
		steer = desired - vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce); // Limit to maximum steering force
		steer.y = ofClamp(steer.y, -maxforce, maxforce); 
		
    }
    return steer;
}

void Boid_mod::draw() {
    // Draw a triangle rotated in the direction of velocity
	float angle = (float)atan2(-vel.y, vel.x);
    float theta =  -1.0*angle;
	float heading2D = ofRadToDeg(theta)+90;
    ofSetColor(255,0,0);
	
    
    if(isDoubleX | isDoubleY){
        
        ofPushStyle();
        ofFill();
        ofPushMatrix();
        
        ofTranslate(loc.x, loc.y);
        ofRotateZ(heading2D);
        ofRotateZ(180);
        customBoid.draw();
        
        ofPopMatrix();
        ofPopStyle();
        
        ofPushStyle();
        ofFill();
        ofPushMatrix();
        
        ofTranslate(loc2.x, loc2.y);
        ofRotateZ(heading2D);
        ofRotateZ(180);
        customBoid.draw();
        
        ofPopMatrix();
        ofPopStyle();
    }
    else{
        ofPushStyle();
        ofFill();
        ofPushMatrix();
        
        ofTranslate(loc.x, loc.y);
        ofRotateZ(heading2D);
        ofRotateZ(180);
        customBoid.draw();
        
        ofPopMatrix();
        ofPopStyle();
        
        
    }
    
    
    
    
}

void Boid_mod::flock(vector<Boid_mod> &boids) {
	ofVec2f sep = separate(boids);
	ofVec2f ali = align(boids);
	ofVec2f coh = cohesion(boids);
	
	// Arbitrarily weight these forces
	sep *= seperation;
	ali *= alignment;
	coh *= cohere;
	
	acc += sep + ali + coh;
}

/*
 postscript
 */
bool Boid_mod::isHit(int x, int y, int radius) {
    int r = 1;
    int range = 50;//calculation error range
    int dist =r + radius - range;
    if(pow((x-loc.x),2)+pow((y-loc.y),2) < dist * dist) {
        return true;
    }
    return false;
}

// Separation
// Method checks for nearby boids and steers away
ofVec2f Boid_mod::separate(vector<Boid_mod> &boids) {
    float desiredseparation = 50.0f;
    ofVec2f steer;
    int count = 0;
	
    // For every boid in the system, check if it's too close
    for (int i = 0 ; i < boids.size(); i++) {
		Boid_mod &other = boids[i];
		
		float d = ofDist(loc.x, loc.y, other.loc.x, other.loc.y);
		
		// If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
		if ((d > 0) && (d < desiredseparation)) {
			// Calculate vector pointing away from neighbor
			ofVec2f diff = loc - other.loc;
			diff /= d;			// normalize
			diff /= d;        // Weight by distance
			steer += diff;
			count++;            // Keep track of how many
		}
    }
    // Average -- divide by how many
    if (count > 0) {
		steer /= (float)count;
    }
	
	
    // As long as the vector is greater than 0
	//float mag = sqrt(steer.x*steer.x + steer.y*steer.y);
	
	float mag = sqrt(steer.x*steer.x + steer.y*steer.y);
    if (mag > 0) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer /= mag;
		steer *= maxspeed;
		steer -= vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce);
		steer.y = ofClamp(steer.y, -maxforce, maxforce);
    }
    return steer;
}

// Alignment
// For every nearby boid in the system, calculate the average velocity
ofVec2f Boid_mod::align(vector<Boid_mod> &boids) {
    float neighbordist = 50.0;
    ofVec2f steer;
    int count = 0;
    for (int i = 0 ; i < boids.size(); i++) {
		Boid_mod &other = boids[i];
		
		float d = ofDist(loc.x, loc.y, other.loc.x, other.loc.y);
		if ((d > 0) && (d < neighbordist)) {
			steer += (other.vel);
			count++;
		}
    }
    if (count > 0) {
		steer /= (float)count;
    }
	
    // As long as the vector is greater than 0
	float mag = sqrt(steer.x*steer.x + steer.y*steer.y);
    if (mag > 0) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer /= mag;
		steer *= maxspeed;
		steer -= vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce);
		steer.y = ofClamp(steer.y, -maxforce, maxforce);
    }
    return steer;
}

// Cohesion
// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
ofVec2f Boid_mod::cohesion(vector<Boid_mod> &boids) {
    float neighbordist = 25.0;
    ofVec2f sum;   // Start with empty vector to accumulate all locations
    int count = 0;
    for (int i = 0 ; i < boids.size(); i++) {
		Boid_mod &other = boids[i];
		float d = ofDist(loc.x, loc.y, other.loc.x, other.loc.y);
		if ((d > 0) && (d < neighbordist)) {
			sum += other.loc; // Add location
			count++;
		}
    }
    if (count > 0) {
		sum /= (float)count;
		return steer(sum, false);  // Steer towards the location
    }
    return sum;
}

void Boid_mod::toCirc(){
    customBoid.toCirc();
    maxspeed = .3;
    maxforce = 0.1;
}

void Boid_mod::toTri(){
    customBoid.toTri();
    maxspeed = 2;
    maxforce = 0.5;
    //isSeeking = true;
    //seekHeight = int(ofRandom(0, fboHeight));
    
}

void Boid_mod::disappear(){
    customBoid.disappear();
    //ease them to speed up
}

void Boid_mod::appear(float sz){
    isSeeking = false;
    customBoid.appear(sz);
    // ease them slow down
}

void Boid_mod::appear(){
    isSeeking = false;
    customBoid.appear();
}

void Boid_mod::setRatio(int ratio){
    customBoid.triToCircleRatio = ratio; 
}