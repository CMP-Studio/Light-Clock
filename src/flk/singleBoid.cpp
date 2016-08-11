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
    r = 20;
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
    
    dur = 3000;
    isPulse = false;
    
    isCirc = false;
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
    if(!isShrink & !isGrow & !isAppear & !isDisappear & isPulse){
        if (isCirc){
            r = ofMap(ofNoise(randomNoiseSeed,ofGetElapsedTimef()),0,1,circSize,circSize + 15);
        }
        else{
            r = ofMap(ofNoise(randomNoiseSeed, ofGetElapsedTimef()),0,1,triSize,triSize + 15);
        }
        
    }*/ 

    if(isAppear){
        r = ofMap(ofGetElapsedTimeMillis(), startTime, startTime + dur, 0, triSize);
        if ( r >= triSize){
            isAppear = false;
            isPulse = true;
        }
    }
    
    else if(isDisappear){
        r = ofMap(ofGetElapsedTimeMillis(), startTime, startTime + dur, lastR, 0);
        if ( r <= 0){
            isDisappear = false;
            isPulse = false; 
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

void singleBoid::appear(){
    isAppear = true;
    startTime= ofGetElapsedTimeMillis();
}

void singleBoid::disappear(){
    isDisappear = true;
    startTime= ofGetElapsedTimeMillis();
    lastR = r;
}