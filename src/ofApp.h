#pragma once

#include "ofMain.h"
#include "dayFade.h"
#include "ofxGui.h"
#include "ofxCameraSaveLoad.h"

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
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void cropTrigger();
        void camPosChanged(int & camYpos);
        void camZoomChanged(int & camZoom);
        // t- current time
        // b- start value
        // c- change in value
        // d- duration
        float easeOut();
        void exit();

    ofFbo currentMoment;
    ofImage curMoment;
    ofImage momentMsk;
    
    
    //Day dayOne;
    //Day dayTwo;
    
    vector <DayFade>days;
    
    bool isEaseOut;
    bool isSpin;
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
    ofxIntSlider maxSpinTime;
    ofxIntSlider soundFrequency;
    
    bool showGui;
    
    ofSoundPlayer left;
    ofSoundPlayer right;
    ofSoundPlayer moment;
    
    
    
    
};
