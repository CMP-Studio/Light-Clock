//
//  oneImage.cpp
//  FunWithFiles
//
//  Created by Caroline Record on 8/19/16.
//
//

#include "oneImage.h"



void oneImage::setup(string fPath, int cropBottom, int cropTop, bool toCrop){
    crpTp = cropTop;
    crpBttm = cropBottom; 
    filePath = fPath;
    startThread();
    isLoading = true;
    isLoaded = false;
    isCrop = true;
    isReady = false;
    isQuit = false;
}


void oneImage::reImageLoad(){


    ofLoadImage(img, filePath);
    // is it less than ten seconds. Do not recurse longer than that.
    
    bool isTimeOut = (ofGetElapsedTimeMillis() - startTime) > 2000;
    if (!img.isAllocated() & !isTimeOut ){
        sleep(100);
        img.clear(); 
        reImageLoad();
    }


}


void oneImage::threadedFunction(){
    
    startTime = ofGetElapsedTimeMillis();
    reImageLoad();
    //ofLoadImage(img, filePath);
    if (img.isAllocated()){
        if (isCrop){
            //img.getWidth(), (crpBottom - crpTop)
            img.crop(0,crpTp,img.getWidth(),(crpBttm - crpTp));
       
        }
    }
}

void oneImage::update(){
    if(!isThreadRunning() && isLoading){
        if(img.isAllocated()){
            image.setFromPixels(img);
            image.update();
            isLoading = false;
            isLoaded = true;
            isReady = true;
            img.clear();
            stopThread();
            //ofLog()<<"successful load";
            //ofLog()<< filePath;
            //ofLog()<< ofGetTimestampString();
        }
        else{
            isReady = true;
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

void oneImage::giveUpSafely(){
    //stopThread();
    //isReady=true;
}
