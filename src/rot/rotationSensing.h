//
//  rotationSensing.h
//  rotationSensing
//
//  Created by Sam Ticknor on 8/10/16.
//
//

#pragma once

#include "ofxOpticalFlowFarneback.h"
#include "ofxOpenCv.h"

class rotationSensing : public ofBaseApp {
  
  // #define USE_USB_CAMERA // uncomment this line to use USB camera
  
public:
  //* LIFECYCLE *//
  void setup();
  void update();
  void draw();
  
  //* STREAM *//
  double getCwRotation();
  double getCcwRotation();
  
  double getCwVelocity();
  double getCcwVelocity();
  
  //* VIDEO *//
  void grabNewBackground();
  
  //* OPTICAL FLOW GETTERS AND SETTERS *//
  double getPyramidScale();
  int getPyramidLevels();
  int getWindowSize();
  int getIterationsPerLevel();
  int getExpansionArea();
  double getExpansionSigma();
  bool getFlowFeedback();
  bool getGaussianFiltering();
  
  void setPyramidScale(double pyramidScale);
  void setPyramidLevels(int pyramidLevels);
  void setWindowSize(int windowSize);
  void setIterationsPerLevel(int iterations);
  void setExpansionArea(int expansionArea);
  void setExpansionSigma(double expansionSigma);
  void setFlowFeedback(bool feedbackOn);
  void setGaussianFiltering(bool filteringOn);
  
  //* INTERACTION CONSTANT GETTERS AND SETTERS *//
  pair<int, int> getInteractionCenter();
  int getInteractionRadius();
  int getSamplingDensity();
  int getFlowThreshold();
  double getCwFlowConstant();
  double getCcwFlowConstant();
  double getCwAcceleration();
  double getCcwAcceleration();
  
  void setInteractionCenter(int x, int y);
  void setInteractionRadius(int radius);
  void setSamplingDensity(int density);
  void setFlowThreshold(int threshold);
  void setCwFlowConstant(double flowConstant);
  void setCcwFlowConstant(double flowConstant);
  void setCwAcceleration(double acceleration);
  void setCcwAcceleration(double acceleration);
  
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
  
  //* OPTICAL FLOW *//
  ofxOpticalFlowFarneback opticalFlow;
  double pyramidScale = .35;     // 0.0 - 1.0
  double pyramidLevels = 5;      // greater than 0
  int windowSize = 10;           // greater than 0
  int iterationsPerLevel = 1;    // greater than 0
  int expansionArea = 3;         // greater than 0
  double expansionSigma = 2.25;  // 0.0 - 10.0
  bool flowFeedback = false;
  bool gaussianFiltering = false;
  
  //* INTERACTION CONSTANTS *//
  //* position of interaction area, automatically set to center in setup *//
  int interactionX;
  int interactionY;
  int interactionRadius = 150;
  
  //* how much is added on each for loop iteration *//
  int samplingDensity = 2;
  
  //* threshold for optical flow *//
  int flowThreshold = 20;
  
  //* conversion constants for changing flow velocity to rotation velocity *//
  double cwFlowConstant = .3;
  double ccwFlowConstant = .3;
  
  //* constants used for deacceleration *//
  double cwAcceleration = 0.3;
  double ccwAcceleration = 0.3;
  
};
