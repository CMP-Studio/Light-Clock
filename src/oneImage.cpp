//
//  oneImage.cpp
//  FunWithFiles
//
//  Created by Caroline Record on 8/19/16.
//
//

#include "oneImage.h"



void oneImage::setup(string fPath, int cropBottom, int cropTop){
    crpTp = cropTop;
    crpBttm = cropBottom; 
    filePath = fPath;
    startThread();
    isLoading = true;
    isLoaded = false;
    
}

void oneImage::threadedFunction(){
    ofLoadImage(img, filePath);
    if (img.isAllocated()){
        //img.getWidth(), (crpBottom - crpTop)
        img.crop(0,crpTp,img.getWidth(),(crpBttm - crpTp));
    }
}

void oneImage::update(){
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

}

void oneImage::draw(int x, int y){
    
        if (isLoaded){
            image.draw( x, y);
        }
 
}