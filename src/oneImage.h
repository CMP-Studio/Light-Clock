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
    void setup(string fPath);
    void threadedFunction();
    void draw(int x, int y, int wid, int hght);
    
    string filePath;
    ofPixels img;
    ofImage image;
    bool isLoading;
    bool isLoaded; 
    
};