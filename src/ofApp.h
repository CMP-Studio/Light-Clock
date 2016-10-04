#pragma once

#include "ofMain.h"
#include "dayFade.h"
#include "ofxGui.h"
#include "ofxCameraSaveLoad.h"
#include "Flocking.h"
//#include "rotationSensing.h"
#include "ofxCsv.h"
//#include "oneImage.h"



class ofApp : public ofBaseApp{

	public:
    
    
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
        void cropTrigger();
    
        void camZoomChanged(int & camZoom);
    
        void camPosChanged(int & camYpos);
        void camZposChanged(int & camZpos);
        void camXposChanged(int & camXpos);
    
        void exit();
    
    float wrapCurrentMoment(float Xpos);

    ofShader gaussianBlurX;
    ofShader gaussianBlurY;
    ofFbo currentMoment;
    ofFbo currentMomentMaskFirstPass;
    ofFbo currentMomentMask;
    
    DayFade day;

    bool usingFlow;

    // map to 3D
    ofSpherePrimitive sphere;
    ofEasyCam cam;
    ofMaterial material;
    
    ofFbo getText;


    ofShader brcosa;
    ofFbo filteredText;

    
    ofxPanel gui;
    ofxIntSlider cropLeftRight;
    ofxIntSlider cropTop;
    ofxIntSlider cropBottom;
    ofxIntSlider intervalSize;
    ofxButton reCropEveryThing;
    
    ofxIntSlider camUpDown;
    ofxIntSlider camZoom;
    ofxIntSlider camZpos;
    ofxIntSlider camXpos;
    ofxIntSlider maxSpinTime;
    ofxIntSlider soundFrequency;
    
    //adjust the latent state
    ofxLabel noiseParameters;
    ofxVec2Slider rangeImgMove;
    ofxIntSlider imgMoveSpeed;
    ofxVec2Slider rangeMskMove;
    ofxIntSlider mskMoveSpeed;
    

    ofxFloatSlider brightness;
    ofxFloatSlider contrast;
    ofxFloatSlider saturation;

    ofxFloatSlider cwTerminalVelocity;
    ofxFloatSlider ccwTerminalVelocity;

    // adjust the current moment
    ofxLabel currentMomentParams;
    ofxIntSlider delayTime;
    ofxIntSlider curMomentLength;
    ofxFloatSlider volumeMoment;
    ofxFloatSlider volumeTicking;

    
    bool showGui;
    
    ofSoundPlayer left;
    ofSoundPlayer right;
    ofSoundPlayer moment;
    ofSoundPlayer gong;
    ofSoundPlayer gongMultiple;
    
    Flocking flock2;
    
    //rotationSensing rotSense;
    
    ofImage fullRes;

    bool isFullResTest;


    
    bool isLatent; 
    int timeSinceInteract;
    int anchorImgPos;
    int anchorMskPos;
    float noiseSeedImg;
    float noiseSeedMsk;

    bool isMuteMode;
    
    
    // since day.imgPos / .mskPos is no longer continuous
    // these are needed for the current moment.
    float imgPosContinuous;
    float mskPosContinuous;

    // the coming soon video
    ofVideoPlayer comingSoonVideo;
    
    
    int startCurMoment;
    bool isWaitForMoment;
    
    int startTimeRight;
    int startTimeLeft;
    
    int timeToWaitRight;
    int timeToWaitLeft;

    ofxCsv analytics;
    int currentHour;
    int amountOfActivity;
    int startTimeOfInteraction;

    
    
};
