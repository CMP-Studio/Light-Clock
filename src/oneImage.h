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
    void setup(string fPath, int cropBottom, int cropTop);
    void threadedFunction();
    void draw(int x, int y);
    void update(); 
    
    string filePath;
    ofPixels img;
    ofImage image;
    bool isLoading;
    bool isLoaded;
    
    int startDay;
    int endDay;
private:
    int crpBttm;
    int crpTp;
    
};