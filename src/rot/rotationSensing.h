//
//  rotationSensing.h
//  rotationSensing
//
//  Created by Sam Ticknor on 8/10/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOpticalFlowFarneback.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"

class rotationSensing : public ofBaseApp {
  
  // #define USE_USB_CAMERA // uncomment this line to use USB camera
  
public:
  //* LIFECYCLE *//
  void setup();
  void update();
  void draw();
  void saveSettings();
  
  //* STREAM *//
  double getCwRotation();
  double getCcwRotation();
  
  double getCwVelocity();
  double getCcwVelocity();
  
  //* VIDEO *//
  void grabNewBackground();
  
  //* GUI + SETTINGS *//
  ofxPanel gui;
  // interaction constants
  ofxIntSlider samplingDensity;
  ofxIntSlider flowThreshold;
  ofxFloatSlider cwFlowConstant;
  ofxFloatSlider ccwFlowConstant;
  ofxFloatSlider cwAcceleration;
  ofxFloatSlider ccwAcceleration;
  // optical flow constants
  ofxFloatSlider pyramidScale;
  ofxIntSlider pyramidLevels;
  ofxIntSlider windowSize;
  ofxIntSlider iterationsPerLevel;
  ofxIntSlider expansionArea;
  ofxFloatSlider expansionSigma;
  ofxButton flowFeedback;
  ofxButton gaussianFiltering;
  
  // Set center
  void setInteractionCenter(int x, int y);
  
private:
  //* STREAM *//
  double cwRotation = 0.0;
  double ccwRotation = 0.0;
  
  double cwVelocity = 0.0;
  double ccwVelocity = 0.0;
  
  //* VIDEO *//
  string videoSource = "video0004.mov"; // If not using USB cam, name of file
  bool backgroundSubtraction = false; // set true for background subtraction
  bool backgroundLearned = false;
  
  #ifndef USE_USB_CAMERA
    ofVideoPlayer videoFeed;
  #endif
  #ifdef USE_USB_CAMERA
    ofVideoGrabber videoFeed;
  #endif
  
  ofxCvColorImage videoFrameColor;
  ofxCvGrayscaleImage videoFrameGrayscale;
  ofxCvGrayscaleImage videoBackground;
  
  //* FLOW *//
  ofxOpticalFlowFarneback opticalFlow;

  //* GUI + SETTINGS *//
  void setPyramidScale(float &scale);
  void setPyramidLevels(int &levels);
  void setWindowSize(int &size);
  void setIterationsPerLevel(int &iterations);
  void setExpansionArea(int &area);
  void setExpansionSigma(float &sigma);
  
  int interactionX;
  int interactionY;
  int interactionRadius = 150;  
};
