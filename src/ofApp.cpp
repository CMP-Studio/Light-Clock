#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
        
    isMuteMode = false;
    
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    //ofSetVerticalSync(true);
    
    usingFlow=true;
    //rotSense.setup();
    
    ofLog()<< "get width: " << ofGetWindowWidth();

    gui.setup();
    gui.setPosition(50, 50);
    
    // load a random image to get the size that we are working with
    ofImage testSz;
    testSz.load("sampleImages/sample/007_2016-08-19_R/174_14-34-38.tif");
    int width = testSz.getWidth();
    int height = testSz.getHeight();
    
    gui.add(cropLeftRight.setup("crop left right",width/2,0,width));
    gui.add(cropTop.setup("crop top",height/2,0,height));
    gui.add(cropBottom.setup("crop bottom",height/2,0,height));
    //intervalSize
    gui.add(intervalSize.setup("interval size",60,10,600));
    
    gui.add(reCropEveryThing.setup("Re-initialize Crop"));
    
    gui.add(camZoom.setup("Cam Zoom",90,10,180));
    gui.add(camUpDown.setup("Cam Y",0,-width,width));
    gui.add(camZpos.setup("Cam Z",20,width,-width));
    gui.add(camXpos.setup("Cam X",20,width,-width));
    gui.add(maxSpinTime.setup("spin max speed",height/2,0,width));
    gui.add(soundFrequency.setup("Sound frequency",1990,0,3000));
    gui.add(noiseParameters.setup("latent state",""));
    gui.add(rangeImgMove.setup("Image movement range", ofVec2f(-1,1), ofVec2f(-6,-6),ofVec2f(6,6)));
    gui.add(imgMoveSpeed.setup("speed of image", 4 , 0,100 ));
    gui.add(rangeMskMove.setup("Mask movement range", ofVec2f(-1,1), ofVec2f(-6,-6),ofVec2f(6,6)));
    gui.add(mskMoveSpeed.setup("speed of msk", 4 , 0,100 ));
    gui.add(currentMomentParams.setup("Current Moment",""));
    gui.add(delayTime.setup("delay", 0 , 0,12000 ));
    gui.add(curMomentLength.setup("dur", 20000 , 20000,120000 ));
    
    
    reCropEveryThing.addListener(this, &ofApp::cropTrigger);
    camZoom.addListener(this, &ofApp::camZoomChanged);
   
    camUpDown.addListener(this, &ofApp::camPosChanged);
    
    camZpos.addListener(this, &ofApp::camZposChanged);
    camXpos.addListener(this, &ofApp::camXposChanged);
    
    gui.loadFromFile("guiSettings.xml");
    
    cam.setFov(camZoom);
    
    ofxLoadCamera(cam, "ofEasyCamSettings.xml");
    cam.enableMouseInput();

    getText.allocate( testSz.getWidth(), testSz.getHeight(), GL_RGB);
    
    day.setup( "newImagery", 0, cropTop,cropBottom,cropLeftRight,intervalSize, testSz.getWidth(), testSz.getHeight());
    
    // set up 3D sphere
    sphere.setRadius(ofGetWidth());
    sphere.setPosition( 0,0,0 );
    ofSetSphereResolution(24);
    

    ofEnableLighting();
    showGui = false;
    
    // load sounds
    left.load("sounds/left.wav");
    right.load("sounds/right.wav");
    moment.load("sounds/click.wav");
    gong.load("sounds/gong.mp3");
    gongMultiple.load("sounds/gongMultiple.wav");
    startCurMoment = 0;
    
    flock2.setup(testSz.getWidth(), testSz.getHeight());
    
    currentMoment.allocate( testSz.getWidth(), testSz.getHeight() );
    currentMomentMask.allocate(testSz.getWidth(), testSz.getHeight() );
    isWaitForMoment = false;
    
    // image to test full resolution
    fullRes.load("bestStitch3.png");
    isFullResTest = false;
    
    timeSinceInteract = ofGetElapsedTimeMillis();
    isLatent = true;
    
    mskPosContinuous = 0;
    imgPosContinuous = 0;
    
    comingSoonVideo.load("comingSoon.mov");
    
    startTimeRight =0;
    startTimeLeft =0;
    
    timeToWaitRight =0;
    timeToWaitLeft =0;
}

void ofApp::camZoomChanged(int &camZoom){
    ofLog()<< "zoom called";
    
    cam.setFov(camZoom);
}

void ofApp::camZposChanged(int &camZpos){
    ofLog()<< "camera moved back";
    ofVec3f startPos = cam.getPosition();
    cam.setPosition(startPos.x, startPos.y, camZpos);
}

void ofApp::camXposChanged(int &camXpos){
    ofLog()<< "camera moved back";
    ofVec3f startPos = cam.getPosition();
    cam.setPosition(camXpos, startPos.y,startPos.z );
}


void ofApp::camPosChanged(int &camYpos){
    ofLog()<< "camera zoomed";
    ofVec3f startPos = cam.getPosition();
    cam.setPosition(startPos.x,  camYpos, startPos.z );
}



void ofApp::cropTrigger(){
    
    int num1 = cropTop;
    ofLog() << "cropTop: " << ofToString(num1);
    int num2 = cropBottom;
    ofLog() << "cropBottom: " << ofToString(num2);
    int num3 = cropLeftRight;
    ofLog() << "cropLeftRight: " << ofToString(num3);
    
    ofLog()<<"ping";
    //for(int i=0; i < days.size(); i++){
    day.addCroppedImages(cropTop,cropBottom,cropLeftRight,intervalSize);
    //}
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Audio stuff - to hook up to sam's stuff
    /*
    timeToWaitRight = int(ofMap(1-rightSpin,0,1,2000-soundFrequency,1500));
    if(ofGetElapsedTimeMillis()> (startTimeRight + timeToWaitRight)){
        right.play();
        startTimeRight = ofGetElapsedTimeMillis();
        isLatent = false;
        timeSinceInteract = ofGetElapsedTimeMillis();
    }
    
    timeToWaitLeft = int(ofMap(1-leftSpin,0,1,2000-soundFrequency,1500));
    if(ofGetElapsedTimeMillis()> (startTimeLeft + timeToWaitLeft)){
            left.play();
            startTimeLeft = ofGetElapsedTimeMillis();
            isLatent = false;
            timeSinceInteract = ofGetElapsedTimeMillis();
    }
     */
    
    
    if(day.currentMomentTrig){
        ofLog()<< "trigger!! Timeese";
        isWaitForMoment = true;
        startCurMoment = ofGetElapsedTimeMillis();
        day.currentMomentTrig = false;
    }
    else if (isWaitForMoment & (ofGetElapsedTimeMillis() - startCurMoment >= delayTime ) ) {
        flock2.setMinSize(0);
        flock2.triggerSequenceTwo(curMomentLength);
        gong.play();
        isWaitForMoment=false;
    }
    
    if((!isLatent)&((ofGetElapsedTimeMillis()- timeSinceInteract) >= 5000)){
        isLatent = true;
    }
    else if (isLatent){
        
        float noise1 =ofMap(ofNoise(1,ofGetElapsedTimef()/mskMoveSpeed),0,1,rangeMskMove->x,rangeMskMove->y);
        noise1 = ofClamp(noise1, 0, 6);
        day.mskPos += noise1;
        mskPosContinuous -= noise1;
        
        float noise2 = ofMap(ofNoise(50, ofGetElapsedTimef()/imgMoveSpeed),0,1,rangeImgMove->x,rangeImgMove->y);
        day.imgPos -= noise2;
        imgPosContinuous += noise2;
    
    
    }
    
    day.update();
    flock2.update();
 
    getText.begin();

    ofEnableAlphaBlending();
    ofClear(0);
    ofBackground(0,0,255);
    
    ofPushMatrix();
    
    ofScale(-1,1,1);
    ofTranslate(getText.getWidth() * -1 , 0);
    
    day.draw(0,cropTop,cropLeftRight);
    

    if(flock2.isSequenceTwo){
        
    currentMoment.begin();
        ofClear(0);
        float drawImgPos = wrapCurrentMoment( imgPosContinuous);
        ofLog()<< "Image position: " << imgPosContinuous;
        ofLog()<< "Image position wrapped: " << drawImgPos;
        if (drawImgPos + currentMoment.getWidth() > currentMoment.getWidth()){
            day.manager.curMoment.image.draw( drawImgPos *-1,cropTop );
            day.manager.curMoment.image.draw( drawImgPos *-1 +  currentMoment.getWidth(),cropTop);
        }
        else{
            day.manager.curMoment.image.draw( drawImgPos *-1,cropTop);
        }
    currentMoment.end();
    
    
     currentMomentMask.begin();
            ofClear(0);
            float drawMskPos =  wrapCurrentMoment( mskPosContinuous);
            if (drawMskPos + currentMomentMask.getWidth() > currentMomentMask.getWidth()){
                flock2.drawIntoMe.draw( drawMskPos *-1,0, currentMomentMask.getWidth() , currentMomentMask.getHeight() );
                flock2.drawIntoMe.draw( drawMskPos *-1 +  currentMomentMask.getWidth() ,0,currentMomentMask.getWidth() , currentMomentMask.getHeight());
            }
            else{
                flock2.drawIntoMe.draw( drawMskPos *-1,0,currentMoment.getWidth() , currentMoment.getHeight());
            }
        currentMomentMask.end();
    
        currentMoment.getTexture().setAlphaMask(currentMomentMask.getTexture());
        currentMoment.draw(0,0);
    }
    
    if(isFullResTest){
        fullRes.draw(0,0);
    }
    
    ofPopMatrix();
    
    // draw circles at the crop positions
    if (showGui){
        ofSetColor(255, 0, 0);
        ofDrawEllipse(0, cropBottom, 50,50);
        ofSetColor(0, 255, 0);
        ofDrawEllipse(0, cropTop, 50,50);
        ofSetColor(0, 100, 100);
        ofDrawEllipse(cropLeftRight, cropTop , 50,50);
        ofSetColor(255);
        ofDrawRectangle(50,0,intervalSize, getText.getHeight());
        ofShowCursor();
    }
    else{
        ofHideCursor();
    }
    
    getText.end();

    
    //cam.setPosition(0,0,0 );
         sphere.mapTexCoordsFromTexture( getText.getTexture() );
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    
    
    if(!isMuteMode){
        cam.begin();
        getText.getTexture().bind();
        sphere.draw();
        //sphere.drawWireframe();
        getText.getTexture().unbind();
        cam.end();

    
    if(showGui){
        gui.draw();
        ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()),40, ofGetHeight()-40,0);
    }
    if(usingFlow){
        //rotSense.draw();
    }
    } else{
        // draw video for the mute mode
        ofBackground(31,29,29);
        comingSoonVideo.update();
        if(comingSoonVideo.isFrameNew()){
            int wid = ofGetWindowWidth();
            int proportionalHeight = (wid/comingSoonVideo.getWidth()) * comingSoonVideo.getHeight();
            int xPos = 0;
            int yPos = (ofGetWindowHeight() - proportionalHeight)/2;
            //int yPos = 0;
            comingSoonVideo.draw(xPos, yPos, wid, proportionalHeight);
        }
    }

}


float ofApp::wrapCurrentMoment(float Xpos){
    
    float wrappedMsk = Xpos;
    // If starting position is less than the beginning of the image
    if (wrappedMsk <  0 ){
        // Add as many imageWidths are needed to get back inside the image
        wrappedMsk += currentMoment.getWidth() * (abs(int( wrappedMsk / currentMoment.getWidth()))+1);
    }
    // If starting position is more than the end of the image
    else if (wrappedMsk >  currentMoment.getWidth() ){
        // Subtract as many image widths as needed to get back inside the image
        wrappedMsk -= currentMoment.getWidth() * (abs(int( wrappedMsk / currentMoment.getWidth())));
        //wrappedMsk = 0;
    }
    return wrappedMsk;
}

//-------------------------w-------------------------------------
void ofApp::keyPressed(int key){


    if (key == 'o'){
        usingFlow = !usingFlow;
    }
    else if (key == 'q'){
        day.imgPos -=10;
        imgPosContinuous +=10;
        left.play();
        isLatent = false;
        timeSinceInteract = ofGetElapsedTimeMillis();
    }
    else if(key == 'w'){
        mskPosContinuous -= 10;
        day.mskPos +=10;
        right.play();
        isLatent = false;
        timeSinceInteract = ofGetElapsedTimeMillis();
    }
    else if (key == 'h'){
        showGui = !showGui; 
    }
    else if(key == 'm'){
        flock2.setMinSize(0);
        flock2.triggerSequenceTwo(curMomentLength);
        gongMultiple.play();
    }
    else if(key == 'x'){
        isMuteMode = !isMuteMode;
        if (isMuteMode){
            left.setVolume(0);
            right.setVolume(0);
            gong.setVolume(0);
            gongMultiple.setVolume(0);
            comingSoonVideo.play();
            comingSoonVideo.setLoopState(OF_LOOP_NORMAL);
        }
        else{
            comingSoonVideo.stop();
            left.setVolume(1);
            right.setVolume(1);
            gong.setVolume(1);
            gongMultiple.setVolume(1);
        }
    }


}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::exit(){
    ofxSaveCamera(cam, "ofEasyCamSettings.xml");
    gui.saveToFile("guiSettings.xml");
    
    
    //loader.stopThread();
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}


