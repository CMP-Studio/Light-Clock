//
//  oneImage.cpp
//  FunWithFiles
//
//  Created by Caroline Record on 8/19/16.
//
//

#include "oneImage.h"



void oneImage::setup(string fPath){
    filePath = fPath;
    startThread();
    isLoading = true;
    isLoaded = false;
    
}

void oneImage::threadedFunction(){
    ofLoadImage(img, filePath);
    if (img.isAllocated()){
        img.crop(0,0,img.getWidth(),img.getHeight()/2);
    }
}

void oneImage::draw(int x, int y,int wid, int hght){
    
        if(!isThreadRunning() && isLoading){
            if(img.isAllocated()){
                image.setFromPixels(img);
                image.update();
                isLoading = false;
                isLoaded = true;
                img.clear();
                stopThread();
            }
        }
        if (isLoaded){
            image.draw( x, y, wid, hght);
        }
 
}