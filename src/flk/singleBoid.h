//
//  singleBoid.hpp
//  triToCirc
//
//  Created by Caroline Record on 8/2/16.
//
//

#include "ofMain.h"

class singleBoid{
    public:
    void setup();
    void draw();
    void update();
    void toCirc();
    void toTri();
    void appear();
    void disappear();
    
    private:
    
        float r;
        float triSize;
        float circSize;
        int numOfPoint;
        vector<ofVec2f> circle;
        ofVec2f center;
        vector<ofVec2f> triangle;
        vector<int> triIndex;
        float triToCircleRatio;
        int direction;
        int currTriIndex;
    
        bool isShrink;
        bool isGrow;
    
        bool isAppear;
        bool isDisappear;
    
        bool isPulse;
    
        int startTime;
    
        int dur;
        int lastR;
    
        bool isCirc;
    
        int randomNoiseSeed;
    
        
    
};