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
float DayFade::mskPos = 0;
float DayFade::imgPos = 0;
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
    dir.allowExt("png");
    
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
    int interval = widthOfDay/(singleImg.size()/divNumImgs)+3;
    
    alphaShader.begin();
        alphaShader.setUniform1i("imgWidth", interval*2);
        alphaShader.setUniformTexture("imageMask", gradientMask, 1);
    alphaShader.end();
    
    
    drawSliceOfImagery.allocate(interval*3, imgHeight,GL_RGB);
    
    drawSliceOfImagery.begin();
        ofClear(0,0,0);
    drawSliceOfImagery.end();
    
}

void DayFade::makeMsk(int posMsk , int posImg, int width){
    
    ofLog() << "width: " << width;
    ofLog() << "div: " << singleImg.size()/divNumImgs;
    
    int interval = width/(singleImg.size()/divNumImgs)+3;
    int imgHeight = singleImg.at(0).img.getHeight();
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
    
    // now fill in the start and end positions.
    for (int i=0; i < singleImg.size()/divNumImgs; i++){
        singleImg.at(i).startDay = posMsk;
        singleImg.at(i).endDay = posMsk + interval*2;
        posMsk += interval;
    }

}


void DayFade::update(){
}




void DayFade::draw(int x, int y, int rightCropPos){
  
    // turn msk pos into its wrapped version
    int cnt =0;
    //calculate what msk Position and ImgPosition should be considering the movement of the mask

    //int imgPosTemp = imgPos;
    //imgPosTemp -= mskPos;
    //imgPosTemp = ;
    float mskPosTemp = mskPos/2;
    
    for (int i = 0; i < singleImg.size(); i++){
       if (((wrapIt(x + singleImg.at(i).startDay - mskPosTemp) < rightCropPos) & (wrapIt(x + singleImg.at(i).startDay - mskPosTemp) > 0)) | ( (wrapIt(x + singleImg.at(i).endDay- mskPosTemp)  > 0) & ( wrapIt(x + singleImg.at(i).endDay- mskPosTemp ) < rightCropPos))){
           cnt ++;
           
           // challenge is figuring out where to sample from and where to draw to
          
               drawSliceOfImagery.begin();
               ofClear(0);
               float drawImgPos = wrapIt(singleImg.at(i).startDay + imgPos - mskPosTemp);
                if (drawImgPos+drawSliceOfImagery.getWidth() > imgWidth){
                    singleImg.at(i).img.draw( drawImgPos *-1,0, imgWidth , singleImg.at(i).img.getHeight());
                    singleImg.at(i).img.draw( drawImgPos *-1 +  imgWidth ,0,imgWidth , singleImg.at(i).img.getHeight());
                }
                else{
                    singleImg.at(i).img.draw( drawImgPos *-1,0,imgWidth , singleImg.at(i).img.getHeight());
                }

           
               // ofBackground(int(ofRandom(255)), int(ofRandom(255)), int(ofRandom(255)));
               drawSliceOfImagery.end();
        
           drawSliceOfImagery.getTexture().setAlphaMask(gradientMask);
           drawSliceOfImagery.draw(wrapIt(x + singleImg.at(i).startDay - mskPosTemp) ,y);
           //gradientMask.draw(x + singleImg.at(i).startDay,y);
           //can I crop the image and the mask here before applying the shader to it
           // the wrapping will be tricky
           // idk if I even need my special image now
           /*
           alphaShader.begin();
                alphaShader.setUniformTexture("imageMask", singleImg.at(i).msk, 1);
                alphaShader.setUniform1i("mskXPos", mskPos);
                alphaShader.setUniform1i("imgXPos", imgPos);
                singleImg.at(i).img.draw(x,y, imgWidth , singleImg.at(i).img.getHeight() );
           alphaShader.end();
            */
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
// Thit keeps xPos within imageWidth
float DayFade::wrapIt(float Xpos){
    
    float wrappedMsk = Xpos - mskPos;
    
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