//
//  dayFade.h
//  greyBlend
//
//  Created by Caroline Record on 5/16/16.
//
//

#include "ofMain.h"
#include "singleDay.h"
#include "fileManager.h"

class DayFade{
    
public:
    static int imgScrollX;
    static float mskScrollX;
    
    static int mskWidth;
    static int windowWidth;
    static int imgWidth;
    static int imgHeight;
    static float percentDay;
    static float mskPos;
    static float imgPos;
    
    static int crpHeight;

    void setup( string dirName, int numDay, int crpTop, int crpBottom, int cropLeftRight,  int wid, int hght);
    void update();
    void draw(int x, int y ,int rightCropPos);
    void addCroppedImages(int crpTop, int crpBottom, int cropLeftRight);
    float wrapIt(float Xpos);
    
    string dayDirectory;
    
    bool currentMomentTrig;
    bool lastLoadedState;
    
    fileManager manager;
    
private:
    
    // reduce the number of images it is loading.
    int divNumImgs; 
    // which day it is
    // will soon be irrelevant
    int numberDay;
    // list of objects + meta data
    //vector<singleDay> singleImg;
    // make the gradient mask
    ofShader gradientMaker;
    void makeMsk(int posImg, int width);
    ofTexture gradientMask;
    // will soon be irrelevant
    int mskStartPos;
    int posMsk; 
    // slice to draw into
    ofFbo drawSliceOfImagery;
    
    
    
    int interval; 
    
    
};