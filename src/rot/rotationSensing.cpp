//
//  rotationSensing.cpp
//  rotationSensing
//
//  Created by Sam Ticknor on 8/10/16.
//
//

#include "rotationSensing.h"

// http://openframeworks.cc/ofBook/chapters/threads.html

//--------------------------------------------------------------
void rotationSensing::setup() {
  
  // setup camera
#ifndef USE_USB_CAMERA
  videoFeed.load(videoSource);
  videoFeed.play();
#endif
#ifdef USE_USB_CAMERA
  videoFeed.setDesiredFrameRate(10);
  videoFeed.initGrabber(640, 480);
#endif
  
  // setup optical flow
  opticalFlow.setup(
                    videoFeed.getWidth(),
                    videoFeed.getHeight(),
                    pyramidScale,
                    pyramidLevels,
                    windowSize,
                    iterationsPerLevel,
                    expansionArea,
                    expansionSigma,
                    flowFeedback,
                    gaussianFiltering
                    );
  
  // setup interaction area
  interactionX = videoFeed.getWidth()/2;
  interactionY = videoFeed.getHeight()/2;
}

//--------------------------------------------------------------
void rotationSensing::update() {
  
  videoFeed.update();
  if(videoFeed.isFrameNew()) {
    
    // grab frame
    videoFrameColor.setFromPixels(videoFeed.getPixels());
    
    // set to gray and increase contrast
    videoFrameGrayscale = videoFrameColor;
    videoFrameGrayscale.contrastStretch();
    
    // handle background subtraction if enabled
    if (!backgroundLearned && backgroundSubtraction){
      videoBackground = videoFrameGrayscale; // Grab frame for background image
      backgroundLearned = true;
    } else if (backgroundSubtraction) {
      videoFrameGrayscale.absDiff(videoBackground);
    }
    
    // update optical flow
    opticalFlow.update(videoFrameGrayscale);
    
    // calculate velocities based on optical flow
    double count = 0.0;
    double cwFlowSum = 0.0;
    double ccwFlowSum = 0.0;
    
    for (int x = -interactionRadius; x < interactionRadius; x += samplingDensity){
      for (int y = -interactionRadius; y < interactionRadius; y += samplingDensity){
        // compute pixel position in relation to center of interaction area
        int x0 = interactionX + x;
        int y0 = interactionY + y;
        
        // get flow
        ofVec2f flow = opticalFlow.getVelAtPixel(x0, y0);
        
        // Check that the flow magnitude is larger than threshold
        if (pow((pow(flow.x, 2)+pow(flow.y, 2)), .5) > flowThreshold){
          // find the tangential unit vector of original point
          // x = cos(arctan2(y/x) + 90deg) = -y/r
          // y = sin(arctan2(y/x) + 90deg) = x/r
          double r;
          r = pow((pow(x, 2)+pow(y, 2)), .5);
          if (r != 0){
            double uvx = -y/r;
            double uvy = x/r;
            
            // now use vector projection to apply the velocities to the unit vector
            double projectedFlow = flow.x * uvx + flow.y * uvy;
            
            if(projectedFlow > 0){
              cwFlowSum += projectedFlow;
            } else {
              ccwFlowSum -= projectedFlow;
            }
            count += 1.0;
          }
        }
      }
    }
    
    // now average flow in both directions and translate to velocity
    if (count > 0.0){
      cwVelocity = max(cwVelocity, cwFlowConstant * cwFlowSum / count);
      ccwVelocity = max(ccwVelocity, ccwFlowConstant * ccwFlowSum / count);
    }
  }
  // Update velocity
  cwVelocity = max(0.0, cwVelocity - cwAcceleration);
  ccwVelocity = max(0.0, ccwVelocity - ccwAcceleration);
  
  // Update total rotation
  cwRotation += cwVelocity;
  ccwRotation += ccwVelocity;
}

//--------------------------------------------------------------
void rotationSensing::draw() {
  ofSetColor(255);
  
  // draw video feed
  videoFrameGrayscale.draw(0, 0, videoFeed.getWidth(), videoFeed.getHeight());
  
  // draw crosshairs and bounding box
  ofSetColor(255);
  // cross hairs
  ofDrawLine(0, interactionY, videoFeed.getWidth(), interactionY);
  ofDrawLine(interactionX, 0, interactionX, videoFeed.getHeight());
  // bouding box
  ofDrawLine(interactionX-interactionRadius, interactionY-interactionRadius, interactionX-interactionRadius, interactionY+interactionRadius);
  ofDrawLine(interactionX+interactionRadius, interactionY-interactionRadius, interactionX+interactionRadius, interactionY+interactionRadius);
  ofDrawLine(interactionX-interactionRadius, interactionY-interactionRadius, interactionX+interactionRadius, interactionY-interactionRadius);
  ofDrawLine(interactionX-interactionRadius, interactionY+interactionRadius, interactionX+interactionRadius, interactionY+interactionRadius);
  
  // Draw outer circle
  ofPushMatrix();
  ofTranslate(interactionX+1.5*interactionRadius, interactionY);
  ofSetColor(255, 255, 255);
  ofFill();
  ofRotate(-ccwRotation);
  ofDrawEllipse(0, -60, 5, 5);
  ofSetColor(100, 255, 100);
  ofFill();
  ofDrawEllipse(0, 0, 120, 120);
  ofPopMatrix();
  
  // Draw inner circle
  ofPushMatrix();
  ofTranslate(interactionX+1.5*interactionRadius, interactionY);
  ofSetColor(0, 0, 255);
  ofFill();
  ofRotate(cwRotation);
  ofDrawEllipse(0, -40, 5, 5);
  ofSetColor(255, 100, 100);
  ofFill();
  ofDrawEllipse(0, 0, 80, 80);
  ofPopMatrix();
}

//--------------------------------------------------------------
//* STREAM *//
double rotationSensing::getCwRotation() {
  return (int)cwRotation % 360;
}

double rotationSensing::getCcwRotation() {
  return (int)ccwRotation % 360;
}

double rotationSensing::getCwVelocity() {
  return cwVelocity;
}

double rotationSensing::getCcwVelocity() {
  return ccwVelocity;
}

//--------------------------------------------------------------
//* VIDEO *//
void rotationSensing::grabNewBackground() {
  backgroundLearned = false;
}



//--------------------------------------------------------------
//* INTERACTION CONSTANT GETTERS *//

pair<int, int> rotationSensing::getInteractionCenter(){
  return make_pair(interactionX, interactionY);
}

int rotationSensing::getInteractionRadius(){
  return interactionRadius;
}

int rotationSensing::getSamplingDensity(){
  return samplingDensity;
}

int rotationSensing::getFlowThreshold(){
  return flowThreshold;
}

double rotationSensing::getCwFlowConstant(){
  return cwFlowConstant;
}

double rotationSensing::getCcwFlowConstant(){
  return ccwFlowConstant;
}

double rotationSensing::getCwAcceleration(){
  return cwAcceleration;
}

double rotationSensing::getCcwAcceleration(){
  return ccwAcceleration;
}

//*  INTERACTION CONSTANT SETTERS *//
void rotationSensing::setInteractionCenter(int x, int y){
  interactionX = x;
  interactionY = y;
}

void rotationSensing::setInteractionRadius(int radius){
  interactionRadius = max(0, radius);
}

void rotationSensing::setSamplingDensity(int density){
  samplingDensity = max(0, density);
}

void rotationSensing::setFlowThreshold(int threshold){
  flowThreshold = max(0, threshold);
}

void rotationSensing::setCwFlowConstant(double flowConstant){
  cwFlowConstant = max(0.0, flowConstant);
}

void rotationSensing::setCcwFlowConstant(double flowConstant){
  ccwFlowConstant = max(0.0, flowConstant);
}

void rotationSensing::setCwAcceleration(double acceleration){
  cwAcceleration = max(0.0, acceleration);
}

void rotationSensing::setCcwAcceleration(double acceleration){
  ccwAcceleration = max(0.0, acceleration);
}

//--------------------------------------------------------------