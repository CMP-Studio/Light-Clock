//
//  oneImage.hpp
//  FunWithFiles
//
//  Created by Caroline Record on 8/19/16.
//
//

#include "ofMain.h"

class oneImage: public ofThread{
public:
    
    //oneImage();
    //virtual ~oneImage(){ };

    
    //ofxThreadedImageLoader loader;
    
    //ofxThreadedImageLoader* loadTwo;
    void setup(string fPath, int cropBottom, int cropTop, bool toCrop = true);
    void threadedFunction();
    void draw(int x, int y);
    void update();
    // recursive image load for images that are forming themselves.
    void reImageLoad();
    void giveUpSafely();

    string filePath;
    ofPixels img;
    ofImage image;
    bool isLoading;
    bool isLoaded;
    
    int startDay;
    int endDay;

    // use instead of isLoaded because of dud images
    bool isReady;
    bool isQuit;

private:
    
    bool isCrop;
    int crpBttm;
    int crpTp;
    int startTime; 
};
