//
//  singleBoid.cpp
//  triToCirc
//
//  Created by Caroline Record on 8/2/16.
//
//

#include "singleBoid.h"
#include "ofxEasing.h"

void singleBoid::setup(){
    r = 0;
    minSize = 0;
    triSize =20;
    circSize = 160;
    numOfPoint = 32;
    triToCircleRatio =0;
    direction = 1;
    
    triIndex.push_back(0);
    triIndex.push_back(14);
    triIndex.push_back(18);
    triIndex.push_back(32);
    randomNoiseSeed = int(ofRandom(0, 700));
    
    for (int i = 0; i < numOfPoint; i++) {
        //float radians = (2 * PI * (float(i) / float(numOfPoint))) * 180/PI;
        float radians = (2 * PI * (float(i) / float(numOfPoint)));
        circle.push_back(ofVec2f(sin(radians), cos(radians)));
        float tryy = float(i)/float(numOfPoint);
        
        //circle[i] = [Math.sin(radians), Math.cos(radians)];
    }
    
    currTriIndex =0;
    for (int i = 0; i < numOfPoint; i++) {
        if(i > triIndex[currTriIndex + 1]){
            currTriIndex ++;
        }
        int indexStart = triIndex[currTriIndex];
        int indexEnd = triIndex[currTriIndex + 1];
        ofVec2f vertexStart = circle.at(indexStart % 32);
        ofVec2f vertexEnd = circle.at(indexEnd % 32);
        float ratio = float(i - indexStart)/(indexEnd - indexStart);
        float diffX = (vertexEnd.x - vertexStart.x) * ratio;
        float diffY = (vertexEnd.y - vertexStart.y) * ratio;
        ofVec2f vertex = vertexStart;
        vertex.x += diffX;
        vertex.y += diffY;
        triangle.push_back(vertex);
    }
    
    center = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
    
    isShrink = false;
    isGrow = false;

    isAppear = false;
    isDisappear = false;
    isWaitDisappear = false;
    isWaitAppear = false;
    
    dur = 3000;
    isPulse = false;
    
    isCirc = false;
    delay = int(ofRandom(0,3500));
    
    slowSpeed = .2;
    fastSpeed = 2.5; 
    
}

void singleBoid::draw(){
    //center = ofVec2f(x,y);
    ofPushStyle();
    ofFill();
    ofPushMatrix();
    ofBeginShape();
    
    ofVec2f vertex = ofVec2f(0,0);
    for( int i =0; i < numOfPoint; i++){
        ofVec2f vertexTri = triangle.at(i);
        ofVec2f vertexCircle = circle.at(i);
        
        
        if (isGrow){
            vertex.x = ofxeasing::map(triToCircleRatio, 0, 1, vertexTri.x, vertexCircle.x, ofxeasing::quad::easeOut);
            vertex.y = ofxeasing::map(triToCircleRatio, 0, 1, vertexTri.y, vertexCircle.y, ofxeasing::quad::easeOut);
        }
        else{
            vertex.x = ofxeasing::map(triToCircleRatio, 0, 1, vertexTri.x, vertexCircle.x, ofxeasing::quad::easeIn);
            vertex.y = ofxeasing::map(triToCircleRatio, 0, 1, vertexTri.y, vertexCircle.y, ofxeasing::quad::easeIn);
        }
        
        vertex.x = vertex.x * r ;
        vertex.y = vertex.y * r ;
        ofVertex(vertex.x, vertex.y);
        ofSetColor(255, 255, 255);
    }
    
    ofEndShape(true);
    ofPopMatrix();
    ofPopStyle();
    
}

void singleBoid::update(){
    
    if(isWaitAppear & ((ofGetElapsedTimeMillis()-startTime) >= delay)){
        isWaitAppear = false;
        isAppear = true;
        startTime = ofGetElapsedTimeMillis();
    }
    if(isWaitDisappear & ((ofGetElapsedTimeMillis()-startTime) >= delay)){
        isWaitDisappear = false;
        isDisappear = true;
        startTime = ofGetElapsedTimeMillis();
    }

    if (isShrink | isGrow){
        triToCircleRatio += .01 * direction;
        if (isGrow){
            r = ofMap(triToCircleRatio, 0, 1, lastR, circSize);
        }
        else{
            r = ofMap(triToCircleRatio, 0, 1, triSize,  lastR);
        }
        if((triToCircleRatio < 0)|(triToCircleRatio > 1)){
            isShrink = false;
            isGrow = false;
        }
    }
    
  /*
    if(!isWaitAppear & !isWaitDisappear & !isShrink & !isGrow & !isAppear & !isDisappear & isPulse){
        if (isCirc){
            r = ofMap(ofNoise(randomNoiseSeed,ofGetElapsedTimef()/4),0,1,sizeToGrowTo,sizeToGrowTo + 15);
        }
        else{
            r = ofMap(ofNoise(randomNoiseSeed, ofGetElapsedTimef()/4),0,1,sizeToGrowTo,sizeToGrowTo + 15);
        }
        
    }
   */

    if(isAppear){
        //r = ofxeasing::map(float(ofGetElapsedTimeMillis()), float(startTime), float(startTime + dur), 0.0f, float(sizeToGrowTo), ofxeasing::quad::easeIn);
        //vertex.x = ofxeasing::map(triToCircleRatio, 0, 1, vertexTri.x, vertexCircle.x, ofxeasing::quad::easeIn);
        maxSpeed = ofMap(ofGetElapsedTimeMillis(), startTime, startTime+dur,fastSpeed,slowSpeed);
        
        r = ofMap(ofGetElapsedTimeMillis(), startTime, startTime+dur,minSize, sizeToGrowTo);
        if ( r >= sizeToGrowTo){
            isAppear = false;
            isPulse = true;
        }
    }
    
    else if(isDisappear){
        //r = ofxeasing::map(float(ofGetElapsedTimeMillis()), float(startTime), float(startTime + dur), float(lastR), 0.0f, float(sizeToGrowTo), ofxeasing::quad::easeOut);
        r = ofMap(ofGetElapsedTimeMillis(), startTime, startTime+dur,lastR,minSize);
        maxSpeed = ofMap(ofGetElapsedTimeMillis(), startTime, startTime+dur,slowSpeed, fastSpeed);
        
        if ( r <= minSize){
            isDisappear = false;
            isPulse = false;
            r = 0; 
        }
    }
    

}


void singleBoid::toTri(){
    isShrink = true;
    direction = -1;
    isCirc = false;
    lastR = r;
    triToCircleRatio =1;
}

void singleBoid::toCirc(){
    isGrow = true;
    direction = 1;
    isCirc = true;
    lastR = r;
    triToCircleRatio= 0;
}

void singleBoid::appear(float toGrowTo){
    isWaitAppear = true;
    startTime= ofGetElapsedTimeMillis();
    sizeToGrowTo = toGrowTo;
    
}

void singleBoid::appear(){
    startTime= ofGetElapsedTimeMillis();
    sizeToGrowTo = triSize;
    isWaitAppear = true;
}

void singleBoid::disappear(){
    startTime= ofGetElapsedTimeMillis();
    lastR = r;
    isWaitDisappear = true;
}

