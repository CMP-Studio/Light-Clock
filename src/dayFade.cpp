//
//  dayFade.cpp
//  greyBlend
//
//  Created by Caroline Record on 5/16/16.
//
//

//
//  Day.cpp
//  greyBlend
//
//  Created by Caroline Record on 5/16/16.
//
//

#include "dayFade.h"

int DayFade::imgScrollX =0;
float DayFade::mskScrollX =0;
int DayFade::mskWidth = 0;
int DayFade::windowWidth = 0;
int DayFade::imgWidth = 0;
int DayFade::imgHeight = 0;
float DayFade::percentDay = 1;
float DayFade::mskPos = 0;
float DayFade::imgPos = 0;
int DayFade::crpHeight = 0;

void DayFade::setup( string dirName, int numDay, int crpTop, int crpBottom, int cropLeftRight,  int wid, int hght){
    imgWidth = wid;
    imgHeight = hght;
    gradientMaker.load("shader_gradient/shadersGL3/shader");
    dayDirectory = dirName;
    numberDay = numDay;
    windowWidth = ofGetWidth();
    addCroppedImages (crpTop,crpBottom, cropLeftRight);
    currentMomentTrig = false;
    lastLoadedState = false;

}

void DayFade::addCroppedImages( int crpTop, int crpBottom, int cropLeftRight){
    
    int sz = 20;
    manager.setup(sz, crpBottom,crpTop);
    
    //might be irrelevant eventually
    int widthOfDay = imgWidth * percentDay;
    //mskStartPos = widthOfDay * numberDay;
    mskStartPos = 0;
    
    // to reduce the number of images
    divNumImgs = 1;
    
    // how thin the slices are is decided here
    interval = widthOfDay/(sz/divNumImgs)+3;
    
    // make single gradient to draw over all others
    posMsk = mskStartPos;
    makeMsk( 0, widthOfDay);

    // where I will draw the imagery into
    drawSliceOfImagery.allocate(interval*3, imgHeight,GL_RGB);
    drawSliceOfImagery.begin();
        ofClear(0,0,0);
    drawSliceOfImagery.end();
    
}

void DayFade::makeMsk(int posImg, int width){
    
    int offset = -imgWidth;
    // make just one gradient per day.
    
    ofFbo msk;
    msk.allocate(interval*3, imgHeight);
    msk.begin();
        ofClear(0, 0, 0, 0);
        gradientMaker.begin();
            gradientMaker.setUniform1i("width",interval);
            gradientMaker.setUniform1i("begFadeStart",0);
            gradientMaker.setUniform1i("begFadeEnd", interval);
            gradientMaker.setUniform1i("endFadeStart",interval*2 );
            gradientMaker.setUniform1i("endFadeEnd", interval*3);
            ofDrawRectangle(0, 0,interval*3,imgHeight);
        gradientMaker.end();
        //ofBackground(255,255);
    msk.end();
    gradientMask = msk.getTexture();
    
    // now fill in the start and end positions for each image
    for (int i=0; i < manager.testQ.size()/divNumImgs; i++){
        manager.testQ.at(i)->startDay = posMsk;
        manager.testQ.at(i)->endDay = posMsk + interval*2;
        posMsk += interval;
    }

}


void DayFade::update(){
    manager.update();
    if (!lastLoadedState & manager.curMoment.isLoaded){
        currentMomentTrig = true;
    }
    lastLoadedState =  manager.curMoment.isLoaded;
    
}




void DayFade::draw(int x, int y, int rightCropPos ){

    
    // go through each image there is to draw
    for (int i = 0; i < manager.testQ.size(); i++){
        
        int mskPosNew = (interval)*i + mskPos;
        bool addingNew =  manager.check(mskPosNew,-1);
        
        if(addingNew){
            mskPos += interval;
            mskPosNew = (interval)*i + mskPos;
        }   
               // draw the imagery into a fbo that is just big enough for the slice
            drawSliceOfImagery.begin();
               ofClear(0);

                // position the image within the slice
                float XpositionToDraw  = -mskPosNew + imgPos;
                // wrap  imgPos so it doesn't go beyond the bounds of the image width
                if (XpositionToDraw < -imgWidth){
                    imgPos += imgWidth;
                    XpositionToDraw  = -mskPosNew + imgPos;
                }
                else if (XpositionToDraw > imgWidth) {
                    imgPos -= imgWidth;
                    XpositionToDraw  = -mskPosNew + imgPos;
                }
        
                // draw the image twice if need this is needed to create the wrap
        
                // the left edge of the image is showing
                if (XpositionToDraw > 0){
                    // draw it in its regular position.
                    manager.draw(i,XpositionToDraw,0);
                    // draw another one behind it
                    manager.draw(i,XpositionToDraw - imgWidth,0);
                }
                // the right edge of the image is showing
                else if (XpositionToDraw + imgWidth < drawSliceOfImagery.getWidth() ){
                    manager.draw(i,XpositionToDraw,0);
                    // draw another one behind it
                    manager.draw(i,XpositionToDraw + imgWidth,0);
                }
        
                // just one will do it
                else{
                    manager.draw(i,XpositionToDraw,0);
                }
        drawSliceOfImagery.end();
        
        drawSliceOfImagery.getTexture().setAlphaMask(gradientMask);
        
        drawSliceOfImagery.draw(mskPosNew ,y);

           
       }
    //}
}



// This keeps xPos within imageWidth
float DayFade::wrapIt(float Xpos){
    float wrappedMsk = Xpos - mskPos;
    ofLog() << imgWidth;
    // If starting position is less than the beginning of the image
    if (wrappedMsk <  0 ){
        // Add as many imageWidths are needed to get back inside the image
        wrappedMsk += imgWidth * (abs(int( wrappedMsk / imgWidth))+1);
    }
    // If starting position is more than the end of the image
    else if (wrappedMsk >  imgWidth ){
        // Subtract as many image widths as needed to get back inside the image
        wrappedMsk -= imgWidth * (abs(int( wrappedMsk / imgWidth)));
        //wrappedMsk = 0;
    }
    return wrappedMsk;
}

