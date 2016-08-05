//
//  dayFade.h
//  greyBlend
//
//  Created by Caroline Record on 5/16/16.
//
//

#include "ofMain.h"
#include "singleDay.h"

class DayFade{
    
public:
    static int imgScrollX;
    static float mskScrollX;
    
    static int mskWidth;
    static int windowWidth;
    static int imgWidth;
    static int imgHeight;
    static int originalImgHeight;
    
    static float percentDay;
    
    static int mskPos;
    static int imgPos;
    
    static int crpHeight;

    
    
    void setup( string dirName, int numDay, int crpTop, int crpBottom, int cropLeftRight );
    void update();
    void draw(int x, int y ,int rightCropPos);
    
    void addCroppedImages(int crpTop, int crpBottom, int cropLeftRight);
    
    int wrapIt(int Xpos);
    
    string dayDirectory;
    int cropRight; 
    
    
    
    
private:
    
    int divNumImgs; 
    
    int numberDay;
    
    ofImage msk;
    
    ofPixels mskPix;

    vector<singleDay> singleImg;
    
    ofImage rsltImg;
    
    int height;
    int width;
    
    ofShader gradientMaker;
    void makeMsk(int posMsk , int posImg, int width);
    
    int mskStartPos;

    ofVideoPlayer flk;
    
    ofShader alphaShader;
    
};