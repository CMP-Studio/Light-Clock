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
int DayFade::originalImgHeight = 0;
float DayFade::percentDay = 1;
int DayFade::mskPos = 0;
int DayFade::imgPos = 0;
//static int crpHeight;
int DayFade::crpHeight = 0;

void DayFade::setup( string dirName, int numDay, int crpTop, int crpBottom, int cropLeftRight){
#ifdef TARGET_OPENGLES
    gradientMaker.load("shader_gradient/shadersES2/shader");
    alphaShader.load("shader_alphaMsk2/shadersES2");
#else
    if(ofIsGLProgrammableRenderer()){
        gradientMaker.load("shader_gradient/shadersGL3/shader");
        alphaShader.load("shader_alphaMsk2/shadersGL3/shader");
    }
    else{
        gradientMaker.load("shader_gradient/shadersGL2/shader");
        alphaShader.load("shader_alphaMsk2/shadersGL2/shader");
    }
#endif
    
    dayDirectory = dirName;
    numberDay = numDay;
    windowWidth = ofGetWidth();
    addCroppedImages (crpTop,crpBottom, cropLeftRight);
    
}

void DayFade::addCroppedImages( int crpTop, int crpBottom, int cropLeftRight){
    
    singleImg.clear();
    
    ofDirectory dir(dayDirectory);
    dir.allowExt("jpg");
    
    dir.listDir();
    dir.sort();
    for(int i=0; i<dir.size(); i++){
        singleDay tmp;
        tmp.setup( dir.getPath(i), crpTop, crpBottom );
        ofLog()<<dir.getPath(i);
        singleImg.push_back(tmp);
    }
    
    cropRight = cropLeftRight;
    
    
    originalImgHeight = singleImg.at(0).originalHeight;
    imgHeight = singleImg.at(0).img.getHeight();
    imgWidth = singleImg.at(0).img.getWidth();
    
    int widthOfDay = imgWidth * percentDay;
    //mskStartPos = widthOfDay * numberDay;
    mskStartPos = 0;
    
    // to reduce the number of images
    divNumImgs = 1;
    
    makeMsk(mskStartPos, 0, widthOfDay);
    alphaShader.begin();
    alphaShader.setUniform1i("imgWidth", imgWidth);
    alphaShader.end();
}

void DayFade::makeMsk(int posMsk , int posImg, int width){
    
    ofLog() << "width: " << width;
    ofLog() << "div: " << singleImg.size()/divNumImgs;
    
    int interval = width/(singleImg.size()/divNumImgs)+3;
    int imgHeight = singleImg.at(0).img.getHeight();
    int offset = -imgWidth;
    gradientMaker.begin();
         gradientMaker.setUniform1i("width",imgWidth);
    gradientMaker.end();
    
    for (int i=0; i < singleImg.size()/divNumImgs; i++){
        //singleImg.at(i).isWrapped = false;
        ofFbo mask;
        mask.allocate(imgWidth, imgHeight);
        mask.begin();
            ofClear(0, 0, 0, 0);
            gradientMaker.begin();
                gradientMaker.setUniform1i("begFadeStart",posMsk);
                singleImg.at(i).startDay = posMsk;
                gradientMaker.setUniform1i("begFadeEnd", posMsk + interval);
                gradientMaker.setUniform1i("endFadeStart",posMsk + interval*2 );
                gradientMaker.setUniform1i("endFadeEnd",posMsk + interval*3);
                singleImg.at(i).endDay = posMsk + interval*3;
                ofDrawRectangle(0, 0,imgWidth,imgHeight);
            gradientMaker.end();
    
        /*
        if (posMsk + interval*3 > imgWidth ){
            singleImg.at(i).isWrapped = true;
            gradientMaker.begin();
            gradientMaker.setUniform1i("begFadeStart",offset + posMsk);
            gradientMaker.setUniform1i("begFadeEnd", offset + posMsk + interval );
            gradientMaker.setUniform1i("endFadeStart",offset +posMsk + interval*2 );
            gradientMaker.setUniform1i("endFadeEnd",offset +posMsk + interval*3);
            ofDrawRectangle(0, 0,imgWidth,imgHeight);
            gradientMaker.end();
        }
         */
             // only wrap it if it is needed.
            // draw a second one with an offset to take care of the wrap.
        /*
            gradientMaker.begin();
                gradientMaker.setUniform1i("begFadeStart",offset + posMsk);
                gradientMaker.setUniform1i("begFadeEnd", offset + posMsk + interval );
                gradientMaker.setUniform1i("endFadeStart",offset +posMsk + interval*2 );
                gradientMaker.setUniform1i("endFadeEnd",offset +posMsk + interval*3);
                ofDrawRectangle(0, 0,imgWidth,imgHeight);
            gradientMaker.end();
          */
       
        mask.end();
        posMsk += interval;
        ofLog()<< posMsk;
        ofTexture temp = mask.getTexture();
        singleImg.at(i).msk= temp;
       
        
    }
}


void DayFade::update(){
}




void DayFade::draw(int x, int y, int rightCropPos){
  
    // turn msk pos into its wrapped version
    int cnt =0;
   
    for (int i = 0; i < singleImg.size()/divNumImgs; i++){
       if (((wrapIt(singleImg.at(i).startDay) < rightCropPos )& (wrapIt(singleImg.at(i).startDay) > 0)) | ((wrapIt(singleImg.at(i).endDay)  > 0 )&(wrapIt(singleImg.at(i).endDay)  < rightCropPos))){
           cnt ++;
           alphaShader.begin();
                alphaShader.setUniformTexture("imageMask", singleImg.at(i).msk, 1);
                alphaShader.setUniform1i("mskXPos", mskPos);
                alphaShader.setUniform1i("imgXPos", imgPos);
                singleImg.at(i).img.draw(x,y, imgWidth , singleImg.at(i).img.getHeight() );
           alphaShader.end();
       }
    }
    
    /*
    for (int i = 0; i < singleImg.size(); i++){
        ofSetColor(255, 0, 0);
        ofDrawEllipse(wrapIt(singleImg.at(i).startDay), ofGetHeight()/2, 10, 10);
        //ofSetColor(0, 255, 0);
        //ofDrawEllipse(wrapIt(singleImg.at(i).endDay), ofGetHeight()/2, 10, 10);
    }
     */
    
    //ofLog()<< ofToString(cnt);


    
    /*
    ofTexture oneMsk = *allDayImage.at(0).getTexture().getAlphaMask();
    allDayImage.at(0).update();
    allDayImage.at(0).draw(0,0);
    oneMsk.draw(0, 0);
   */
  //theMasks.at(0).draw(0,0);
    
    //allDayImage.at(0).draw(0,0);
   // allDayImage.at(1).draw(0,0);
   // allDayImage.at(2).draw(0,0);
    
    
}

int DayFade::wrapIt(int Xpos){
    int wrappedMsk =mskPos;
    wrappedMsk *= -1;
    wrappedMsk += Xpos;
    
    if (wrappedMsk <  0 ){
        wrappedMsk += imgWidth * (abs(int( wrappedMsk / imgWidth))+1);
    }
    else if (wrappedMsk >  imgWidth ){
        wrappedMsk -= imgWidth * (abs(int( wrappedMsk / imgWidth)));
    }
    return wrappedMsk;
}