#pragma once

#include "ofMain.h"
#include "dayFade.h"
#include "ofxGui.h"
#include "ofxCameraSaveLoad.h"
#include "Flocking.h"
#include "rotationSensing.h"
#include "ofxThreadedImageLoader.h"


class ofApp : public ofBaseApp{

	public:
    
        ofxThreadedImageLoader loader;
    
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
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void cropTrigger();
    
        void camZoomChanged(int & camZoom);
    
        void camPosChanged(int & camYpos);
        void camZposChanged(int & camZpos);
        void camXposChanged(int & camXpos);
    
        // t- current time
        // b- start value
        // c- change in value
        // d- duration
        float easeOut();
        void exit();
    
    float wrapCurrentMoment(float Xpos);

    ofFbo currentMoment;
    ofFbo currentMomentMask;
    
    ofImage curMoment;
    ofImage momentMsk;
    
    
    //Day dayOne;
    //Day dayTwo;
    
    vector <DayFade>days;
    
    bool isEaseOut;
    bool isSpin;
    bool usingFlow;
    float beginningValue;
    int begFrame;
    
    float ellipseX;
    int speed; 

    
    ofVideoPlayer flock;
    
    ofImage bgrnd;
    
    // map to 3D
    ofSpherePrimitive sphere;
    ofEasyCam cam;
    ofImage texture;
    ofMaterial material;
    
    ofFbo getText;
    
    int cropLevel;
    
    
    ofxPanel gui;
    ofxIntSlider cropLeftRight;
    ofxIntSlider cropTop;
    ofxIntSlider cropBottom;
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
    
    
    
    bool showGui;
    
    ofSoundPlayer left;
    ofSoundPlayer right;
    ofSoundPlayer moment;
    ofSoundPlayer gong;
    ofSoundPlayer gongMultiple;
    
    Flocking flock2;
    
    //rotationSensing rotSense;

    ofShader momentAlphaShader;
    
    ofImage fullRes;

    bool isFullResTest; 


    
    bool isLatent; 
    int timeSinceInteract;
    
    bool isMuteMode;
};
