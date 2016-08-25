//
//  oneImage.cpp
//  FunWithFiles
//
//  Created by Caroline Record on 8/19/16.
//
//

#include "oneImage.h"



void oneImage::setup(string fPath,int cropBottom, int cropTop){
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
        img.crop(0,0,img.getWidth(),(crpBttm - crpTp));
    }
}

void oneImage::draw(int x, int y){
    
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
            image.draw( x, y);
        }
 
}