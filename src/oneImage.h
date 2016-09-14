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
    
    oneImage(){
       // ofLog()<< "calling empty constructor";
    }

/*
    ~oneImage(){
        ofLog()<<"the destructor is being called";
        stopThread();
        img.clear();
        image.clear();
        //filePath.clear();

    };
    */

    bool closeThread();
    
    //ofxThreadedImageLoader loader;
    
    //ofxThreadedImageLoader* loadTwo;
    void setup(string fPath, int cropBottom, int cropTop, bool toCrop = true);
    void threadedFunction();
    void draw(int x, int y);
    void update();
    // recursive image load for images that are forming themselves.
    void reImageLoad();

    void destroyEverything();
    
    string filePath;
    ofPixels img;
    ofImage image;
    bool isLoading;
    bool isLoaded;
    
    /*
    int startDay;
    int endDay;
*/
private:
    
    bool isCrop;
    int crpBttm;
    int crpTp;
    int startTime; 
};
