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

  ofLog()<< "testPing";
  // setup camera
#ifndef USE_USB_CAMERA
  videoFeed.load(videoSource);
  videoFeed.play();
#endif
#ifdef USE_USB_CAMERA
  ofLog()<< "PING I AM IN";
  videoFeed.setVerbose(true);
  videoFeed.listDevices();
  //videoFeed.setDeviceID();
  videoFeed.setDesiredFrameRate(10);
  videoFeed.initGrabber(640, 480);

#endif
  
  // setup gui
  gui.setup();
  gui.setPosition(50, 400);

  // setup camera settings and related gui
  overheadCameraSettingsGui.setup("panel","settings.xml",0,0);
  overheadCameraSettings.setup("/dev/video0");
  overheadCameraSettingsGui.add(overheadCameraSettings.parameters);
  overheadCameraSettingsGui.setPosition(300, 400);

  // interaction constants
  gui.add(samplingDensity.setup("Sampling density", 2, 1, 10));
  gui.add(flowThreshold.setup("Flow threshold", 15, 0, 30));
  gui.add(cwFlowConstant.setup("CW velocity", 0.7, 0.0, 2.0));
  gui.add(cwAcceleration.setup("CW de-acceleration", 0.15, 0.0, 2.0));
  gui.add(ccwFlowConstant.setup("CCW velocity", 0.5, 0.0, 2.0));
  gui.add(ccwAcceleration.setup("CCW de-acceleration", 0.18, 0.0, 2.0));

  gui.add(interactionX.setup("x", 400, 0, 600));
  gui.add(interactionY.setup("y", 250, 0, 600));

  // optical flow constants
  gui.add(pyramidScale.setup("Pyramid scale", 0.35, 0.0, .99));
  pyramidScale.addListener(this, &rotationSensing::setPyramidScale);

  gui.add(pyramidLevels.setup("Pyramid levels", 5, 1, 10));
  pyramidLevels.addListener(this, &rotationSensing::setPyramidLevels);
  
  gui.add(windowSize.setup("Window size", 10, 1, 10));
  windowSize.addListener(this, &rotationSensing::setWindowSize);
  
  gui.add(iterationsPerLevel.setup("Iterations per level", 1, 1, 10));
  iterationsPerLevel.addListener(this, &rotationSensing::setIterationsPerLevel);
  
  gui.add(expansionArea.setup("Expansion area", 3, 1, 10));
  expansionArea.addListener(this, &rotationSensing::setExpansionArea);
  
  gui.add(expansionSigma.setup("Expansion sigma", 2.25, 0.0, 10.0));
  expansionSigma.addListener(this, &rotationSensing::setExpansionSigma);
  
  gui.loadFromFile("guiSettings.xml");
  
  // setup optical flow
  opticalFlow.setup(
    2*interactionRadius,
    2*interactionRadius,
    pyramidScale,
    pyramidLevels,
    windowSize,
    iterationsPerLevel,
    expansionArea,
    expansionSigma,
    false,
    false
  );

 // interactionX = 400;
//  interactionY = 250;

}

//--------------------------------------------------------------
void rotationSensing::update() {
  videoFeed.update();
  if(videoFeed.isFrameNew()) {
    
    ofPixels feed = videoFeed.getPixels();
    feed.crop(interactionX-interactionRadius,
              interactionY-interactionRadius,
              2*interactionRadius,
              2*interactionRadius);
    feed.rotate90(1);
    
    // grab frame
    videoFrameColor.setFromPixels(feed);
    
    // set to gray and increase contrast
    videoFrameGrayscale = videoFrameColor;
    //videoFrameGrayscale.contrastStretch();
    
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
    double cwFlowCount= 0.0;
    double ccwFlowCount = 0.0;
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
              ccwFlowSum += abs(projectedFlow);
              ccwFlowCount += 1.0;
            } else if (projectedFlow < 0){
              cwFlowSum += abs(projectedFlow);
              cwFlowCount += 1.0;
            }
            count += 1.0;
          }
        }
      }
    }
    
    // now average flow in both directions and translate to velocity
    if (count > 0.0){
      if ((cwFlowCount / count) > 0.33) {
        cwVelocity = max(cwVelocity, cwFlowConstant * cwFlowSum / count);
      }
      if ((ccwFlowCount / count) > 0.33) {
        ccwVelocity = max(ccwVelocity, ccwFlowConstant * ccwFlowSum / count);
      }
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
  ofPushMatrix();
  ofTranslate(0, 400);
  ofSetColor(255);
  
  // draw video feed
  videoFrameGrayscale.draw(interactionX-interactionRadius, interactionY-interactionRadius, videoFrameGrayscale.getWidth(), videoFrameGrayscale.getHeight());
  
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

  ofSetColor(255);
  ofPopMatrix();
  
  // Guis
  gui.draw();
  overheadCameraSettingsGui.draw();

}

void rotationSensing::saveSettings(){
  gui.saveToFile("guiSettings.xml");
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
//*  OPTICAL FLOW SETTERS *//
void rotationSensing::setPyramidScale(float &scale){
  opticalFlow.setPyramidScale((double)scale);
}

void rotationSensing::setPyramidLevels(int &levels){
  opticalFlow.setPyramidLevels(levels);
}

void rotationSensing::setWindowSize(int &size){
  opticalFlow.setWindowSize(size);
}

void rotationSensing::setIterationsPerLevel(int &iterations){
  opticalFlow.setIterationsPerLevel(iterations);
}

void rotationSensing::setExpansionArea(int &area){
  opticalFlow.setExpansionArea(area);
}

void rotationSensing::setExpansionSigma(float &sigma){
  opticalFlow.setExpansionSigma((double)sigma);
}

//--------------------------------------------------------------
