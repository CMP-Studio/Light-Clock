#include "ofApp.h"
/*
#include "ofxCsv.h"

//.h

ofxCsv analytics;
int currentHour;
int amountOfActivityMillis;
int startTimeOfInteraction;

// setUp

// load in analytics
analytics.loadFile(ofToDataPath("analytics.csv"));
currentHour = ofGetHours();
amountOfActivity =0;


// update

if isLatent is true but it is active now then set the interaction start time

if((!isLatent)&((ofGetElapsedTimeMillis()- timeSinceInteract) >= 5000)){
        isLatent = true;
        int timePassed = ofGetElapsedTimeMillis()- startTimeOfInteraction;
        amountOfActivity += timePassed - 5000;
    }


//to update the csv file

int row = analytics.numRows;
analytics.setString(row, 0, ofGetTimestampString());
analytics.setString(row, 1, amountOfAtivity);
analytics.saveFile(ofToDataPath("analytics.csv"));
amountOfActivity =0; \
*/
//--------------------------------------------------------------
void ofApp::setup(){
    
    
    
    
    ofHideCursor();
    analytics.load(ofToDataPath("analytics.csv"));
    //analytics.loadFile(ofToDataPath("analytics.csv"));
    currentHour = ofGetHours();
    amountOfActivity =0;


    if(ofIsGLProgrammableRenderer()) {
        brcosa.load("shader_brcosa/shadersGL3/shader");
        gaussianBlurX.load("shader_gaussianBlur/shadersGL3/shaderBlurX");
        gaussianBlurY.load("shader_gaussianBlur/shadersGL3/shaderBlurY");
    }

    //ofSetLogLevel(OF_LOG_VERBOSE);
    // ofSetFrameRate(25);
    isMuteMode = false;
    
    //ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    //ofSetVerticalSync(true);
    
    usingFlow=false;
    //rotSense.setup();
    
    //ofLog()<< "get width: " << ofGetWindowWidth();

    gui.setup();
    gui.setPosition(50, 50);
    
    // load a random image to get the size that we are working with
    ofImage testSz;
    testSz.load("/media/caroline/Storage/pano/009_2016-09-03_N/003_00-18-45.tif");
    //testSz.load("/Users/carolinerecord/Documents/of_v0.9.2_osx_release/apps/myApps/blendedImagery/bin/data/sampleImages/sample/005_2016-08-19_N/285_23-48-21.tif");
    
    int width = testSz.getWidth();
    int height = testSz.getHeight();
    
    gui.add(cropLeftRight.setup("crop left right",width/2,0,width));
    gui.add(cropTop.setup("crop top",height/2,0,height));
    gui.add(cropBottom.setup("crop bottom",height/2,0,height));
    //intervalSize
    gui.add(intervalSize.setup("interval size",60,10,600));
    
    //gui.add(reCropEveryThing.setup("Re-initialize Crop"));
    
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

    gui.add(brightness.setup("Brightness", 1, 0, 2));
    gui.add(contrast.setup("Contrast", 1, 0, 2));
    gui.add(saturation.setup("Saturation", 1, 0, 2));

    gui.add(cwTerminalVelocity.setup("cw term. vel.", 15.0, 0.0, 50.0));
    gui.add(ccwTerminalVelocity.setup("ccw term. vel.", 15.0, 0.0, 50.0));

    gui.add(volumeMoment.setup("moment vol", 1, 0, 1));
    gui.add(volumeTicking.setup("clicking vol", 1, 0, 1));



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
    filteredText.allocate( testSz.getWidth(), testSz.getHeight(), GL_RGB);

    
    day.setup( "newImagery", 0, cropTop,cropBottom,cropLeftRight,intervalSize, testSz.getWidth(), testSz.getHeight());
    
    // set up 3D sphere
    sphere.setRadius(ofGetWidth());
    sphere.setPosition( 0,0,0 );
    ofSetSphereResolution(24);
    

    ofEnableLighting();
    showGui = false;
    
    // load sounds
    left.load("sounds/left2.wav");
    left.setVolume(volumeTicking);
    right.load("sounds/right2.wav");
    right.setVolume(volumeTicking);
    moment.load("sounds/click2.mp3");
    moment.setVolume(volumeMoment);
    gong.load("sounds/gong.mp3");
    //moment.load("sounds/gongMultiple.wav");
    startCurMoment = 0;
    
    flock2.setup(testSz.getWidth(), testSz.getHeight());
    
    currentMoment.allocate( testSz.getWidth(), testSz.getHeight() );
    currentMomentMaskFirstPass.allocate(testSz.getWidth(), testSz.getHeight() );
    currentMomentMask.allocate(testSz.getWidth(), testSz.getHeight() );
    isWaitForMoment = false;
    
    // image to test full resolution
    fullRes.load("bestStitch3.png");
    isFullResTest = false;
    
    timeSinceInteract = ofGetElapsedTimeMillis();
    isLatent = false;
    
    mskPosContinuous = 0;
    imgPosContinuous = 0;
    
    comingSoonVideo.load("comingSoon.mov");
    
    startTimeRight =0;
    startTimeLeft =0;
    
    timeToWaitRight =0;
    timeToWaitLeft =0;
    ////ofLog()<<"got to the end of setup!!!";

    noiseSeedImg =0.0f;
    noiseSeedMsk =0.0f;
    ofHideCursor();

    // setup up google analytics
    ga.setShouldReportFramerates(true);
    ga.setFramerateReportInterval(600);
    ga.setEnabled(true);
    ga.setRandomizeUUID(false);
    ga.setSendToGoogleInterval(0.0);

    ga.setup("UA-84486442-1", "Light Clock","0.1");
}

void ofApp::camZoomChanged(int &camZoom){
    ofLog()<< "zoom called";
    
    cam.setFov(camZoom);
     ofHideCursor();
}

void ofApp::camZposChanged(int &camZpos){
    //ofLog()<< "camera moved back";
    ofVec3f startPos = cam.getPosition();
    cam.setPosition(startPos.x, startPos.y, camZpos);
}

void ofApp::camXposChanged(int &camXpos){
    //ofLog()<< "camera moved back";
    ofVec3f startPos = cam.getPosition();
    cam.setPosition(camXpos, startPos.y,startPos.z );
}


void ofApp::camPosChanged(int &camYpos){
    //ofLog()<< "camera zoomed";
    ofVec3f startPos = cam.getPosition();
    cam.setPosition(startPos.x,  camYpos, startPos.z );

}



void ofApp::cropTrigger(){
    
    int num1 = cropTop;
    //ofLog() << "cropTop: " << ofToString(num1);
    int num2 = cropBottom;
    //ofLog() << "cropBottom: " << ofToString(num2);
    int num3 = cropLeftRight;
    //ofLog() << "cropLeftRight: " << ofToString(num3);
    
    //ofLog()<<"ping";
    //for(int i=0; i < days.size(); i++){
    day.addCroppedImages(cropTop,cropBottom,cropLeftRight,intervalSize);
    //}
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
        ga.update();

        //ofHideCursor();
        if(currentHour != ofGetMinutes()){

           //a.sendCustomTimeMeasurement("interaction","interacation Duration", amountOfActivity);

            //to update the csv file
            ofxCsvRow row;
            row.insertString(0, ofGetTimestampString());
            row.insertInt(1, amountOfActivity);
            analytics.addRow(row);

            analytics.save(ofToDataPath("analytics.csv"));
            amountOfActivity =0;
            currentHour = ofGetMinutes();
        }

        rotSense.update();
        float mskShift = (float) 2 * rotSense.getCwVelocity();
        float imgShift = (float) 2 * rotSense.getCcwVelocity();
       
        if (mskShift > cwTerminalVelocity) {
            mskShift = cwTerminalVelocity;
        }

        if (imgShift > ccwTerminalVelocity) {
            imgShift = ccwTerminalVelocity;
        }

        day.imgPos -= imgShift;
        day.mskPos += mskShift;
        //day.mskPos -= imgShift;
        imgPosContinuous += imgShift;
        mskPosContinuous -= mskShift;
       //mskPosContinuous += imgShift;

        timeToWaitRight = int(ofMap(imgShift,0,ccwTerminalVelocity, soundFrequency, 20));
        if((ofGetElapsedTimeMillis()> (startTimeRight + timeToWaitRight)) && imgShift > 0){
            if(isLatent){
                startTimeOfInteraction = ofGetElapsedTimeMillis();
            }
            left.setVolume(ofMap(imgShift, 0, ccwTerminalVelocity, 0, volumeTicking));
            left.play();
            startTimeRight = ofGetElapsedTimeMillis();
            isLatent = false;
            timeSinceInteract = ofGetElapsedTimeMillis();
        }

        timeToWaitLeft = int(ofMap(mskShift,0,cwTerminalVelocity,soundFrequency, 20));
        if((ofGetElapsedTimeMillis()> (startTimeLeft + timeToWaitLeft)) && mskShift > 0){

                if(isLatent){
                    startTimeOfInteraction = ofGetElapsedTimeMillis();
                }
                right.setVolume(ofMap(mskShift, 0, cwTerminalVelocity, 0, volumeTicking));
                right.play();
                startTimeLeft = ofGetElapsedTimeMillis();
                isLatent = false;
                timeSinceInteract = ofGetElapsedTimeMillis();
        }
         
    
    if(day.currentMomentTrig){
        //ofLog()<< "trigger!! Timeese";
        isWaitForMoment = true;
        startCurMoment = ofGetElapsedTimeMillis();
        day.currentMomentTrig = false;
    }
    else if (isWaitForMoment & (ofGetElapsedTimeMillis() - startCurMoment >= delayTime ) ) {
        flock2.setMinSize(0);
        flock2.triggerSequenceTwo(curMomentLength);
        moment.play();
        isWaitForMoment=false;

    }
    
    if((!isLatent)&((ofGetElapsedTimeMillis()- timeSinceInteract) >= 5000)){
        cout << "NOT LATENT :)" << endl;
        isLatent = true;
        anchorImgPos = day.imgPos;
        anchorMskPos = day.mskPos;
        int timePassed = ofGetElapsedTimeMillis()- startTimeOfInteraction;
        ga.sendCustomTimeMeasurement("interaction duration", "dur",timePassed-5000);
        amountOfActivity += timePassed - 5000;
    }
    else if (isLatent){

        float noise1 =ofMap(ofNoise(1,ofGetElapsedTimef()/mskMoveSpeed),0,1,rangeMskMove->x,rangeMskMove->y);
        noise1 = ofClamp(noise1, 0, 6);
        day.mskPos += noise1;
        mskPosContinuous -= noise1;
        
        float noise2 = ofMap(ofNoise(50, ofGetElapsedTimef()/imgMoveSpeed),0,1,rangeImgMove->x,rangeImgMove->y);
        day.imgPos -= noise2;
        imgPosContinuous += noise2;


        /*
        day.mskPos = ofMap(ofNoise(1,ofGetElapsedTimef()/mskMoveSpeed),0,1,anchorMskPos + rangeMskMove->x,   anchorMskPos+ rangeMskMove->y);
        mskPosContinuous = ofMap(ofNoise(1,ofGetElapsedTimef()/mskMoveSpeed),1,0,anchorMskPos + rangeMskMove->x,   anchorMskPos + rangeMskMove->y);

        day.imgPos = ofMap(ofNoise(1,ofGetElapsedTimef()/imgMoveSpeed),0,1,anchorImgPos + rangeImgMove->x,    anchorImgPos + rangeImgMove->y);
        imgPosContinuous = ofMap(ofNoise(1,ofGetElapsedTimef()/imgMoveSpeed),1,0,anchorImgPos+ rangeImgMove->x,   anchorImgPos+ rangeImgMove->y);
        */

        /*
        noiseSeedImg += 0.0001;
        day.mskPos = ofMap(ofNoise(1,noiseSeedMsk),0,1,anchorMskPos + rangeMskMove->x,   anchorMskPos+ rangeMskMove->y);
        mskPosContinuous = ofMap(ofNoise(1,noiseSeedMsk),1,0,anchorMskPos + rangeMskMove->x,   anchorMskPos + rangeMskMove->y);

        noiseSeedMsk += 0.0001;
        day.imgPos = ofMap(ofNoise(1,noiseSeedImg),0,1,anchorImgPos + rangeImgMove->x,    anchorImgPos + rangeImgMove->y);
        imgPosContinuous = ofMap(ofNoise(1,noiseSeedImg),1,0,anchorImgPos+ rangeImgMove->x,   anchorImgPos+ rangeImgMove->y);
        */


/*
        day.mskPos = ofMap(ofNoise(1,ofGetElapsedTimef()/mskMoveSpeed),0,1,day.mskPos + rangeMskMove->x,    day.mskPos + rangeMskMove->y);
        mskPosContinuous = ofMap(ofNoise(1,ofGetElapsedTimef()/mskMoveSpeed),0,1,mskPosContinuous - rangeMskMove->x,    mskPosContinuous - rangeMskMove->y);

        day.imgPos = ofMap(ofNoise(1,ofGetElapsedTimef()/imgMoveSpeed),0,1,day.imgPos + rangeImgMove->x,    day.imgPos + rangeImgMove->y);
        imgPosContinuous = ofMap(ofNoise(1,ofGetElapsedTimef()/imgMoveSpeed),0,1,imgPosContinuous- rangeImgMove->x,   imgPosContinuous - rangeImgMove->y);
        */

    }
    
    day.update();
    flock2.update();
 
    getText.begin();
    ofPushMatrix();
    ofScale(-1,1,1);
    ofTranslate(getText.getWidth() * -1 , 0);
    ofEnableAlphaBlending();
    ofClear(0);
    ofBackground(0,0,255);
    

    
    day.draw(0,cropTop,cropLeftRight);
    
    if(flock2.isSequenceTwo){

    currentMoment.begin();
        ofClear(0);
        float drawImgPos = wrapCurrentMoment( imgPosContinuous);
        //ofLog()<< "Image position: " << imgPosContinuous;
        //ofLog()<< "Image position wrapped: " << drawImgPos;
        if (drawImgPos + currentMoment.getWidth() > currentMoment.getWidth()){
            day.manager.curMoment.image.draw( drawImgPos *-1,cropTop );
            day.manager.curMoment.image.draw( drawImgPos *-1 +  currentMoment.getWidth(),cropTop);
        }
        else{
            day.manager.curMoment.image.draw( drawImgPos *-1,cropTop);
        }
    currentMoment.end();


      currentMomentMaskFirstPass.begin();
      gaussianBlurY.begin();
      gaussianBlurY.setUniform1f("blurAmnt", 1.5); //blur amount between 0 and 10
            ofClear(0);
            float drawMskPos =  wrapCurrentMoment( mskPosContinuous);
            if (drawMskPos + currentMomentMaskFirstPass.getWidth() > currentMomentMaskFirstPass.getWidth()){
                flock2.drawIntoMe.draw( drawMskPos *-1,0, currentMomentMaskFirstPass.getWidth() , currentMomentMaskFirstPass.getHeight() );
                flock2.drawIntoMe.draw( drawMskPos *-1 +  currentMomentMaskFirstPass.getWidth() ,0,currentMomentMaskFirstPass.getWidth() , currentMomentMaskFirstPass.getHeight());
            }
            else{
                flock2.drawIntoMe.draw( drawMskPos *-1,0,currentMoment.getWidth() , currentMoment.getHeight());
            }
        gaussianBlurY.end();
        currentMomentMaskFirstPass.end();

        currentMomentMask.begin();
        gaussianBlurX.begin();
        gaussianBlurX.setUniform1f("blurAmnt", 1.5); //blur amount between 0 and 10
        ofClear(0);
        currentMomentMaskFirstPass.draw(0, 0);
        gaussianBlurX.end();
        currentMomentMask.end();

        currentMoment.getTexture().setAlphaMask(currentMomentMask.getTexture());
        currentMoment.draw(0,0);
    }


    /*
    if(isFullResTest){
        fullRes.draw(0,0);
    }
    */
    

    
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
    ofPopMatrix();
    getText.end();

    filteredText = getText;


    filteredText.begin();
   // ofClear(0);
    brcosa.begin();
    brcosa.setUniform1f("brightness", brightness);
    brcosa.setUniform1f("contrast", contrast);
    brcosa.setUniform1f("saturation", saturation);
    //ofPushMatrix();
    //ofScale(-1,1,1);
    //ofTranslate(getText.getWidth() * -1 , 0);
    getText.draw(0, 0);
    //ofPopMatrix();
    brcosa.end();

    filteredText.end();
    
    //cam.setPosition(0,0,0 );
     sphere.mapTexCoordsFromTexture( filteredText.getTexture() );
    

}


//--------------------------------------------------------------
void ofApp::draw(){



    
    
    if(!isMuteMode){
        ofBackground(0);
        cam.begin();
        getText.getTexture().bind();
        sphere.draw();
        //sphere.drawWireframe();
        getText.getTexture().unbind();
        cam.end();

    
    if(showGui){
        cam.disableMouseInput();
        gui.draw();
        ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()),40, ofGetHeight()-40,0);
    }
    else{
        cam.enableMouseInput();
    }
    if(usingFlow){
        //rotSense.draw();
    }
    } else{
        // draw video for the mute mode

        comingSoonVideo.update();
        //if(comingSoonVideo.isFrameNew()){
            //ofBackground(31,29,29);
            int wid = ofGetWindowWidth();
            int proportionalHeight = (wid/comingSoonVideo.getWidth()) * comingSoonVideo.getHeight();
            int xPos = 0;
            int yPos = (ofGetWindowHeight() - proportionalHeight)/2;
            //int yPos = 0;
            comingSoonVideo.draw(xPos, yPos, wid, proportionalHeight);
        //}
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
        //rotSense.saveSettings();
        usingFlow = !usingFlow;
    }

    else if (key == 'q'){
        day.imgPos -=10;
        imgPosContinuous +=10;
        left.play();
        left.setVolume(volumeTicking);
        isLatent = false;
        timeSinceInteract = ofGetElapsedTimeMillis();
    }

    else if(key == 'w'){
        mskPosContinuous -= 10;
        day.mskPos +=10;
        right.play();
        right.setVolume(volumeTicking);
        isLatent = false;
        timeSinceInteract = ofGetElapsedTimeMillis();
    }
    else if (key == 'h'){
        showGui = !showGui; 
        ga.sendEvent("keyboard","gui", 555);
    }
    else if(key == 'm'){
        moment.play();
        moment.setVolume(volumeMoment);
        flock2.setMinSize(0);

        flock2.triggerSequenceTwo(curMomentLength);
        moment.play();

    }
    else if(key == 'x'){
        isMuteMode = !isMuteMode;
        if (isMuteMode){
            left.setVolume(0);
            right.setVolume(0);
            gong.setVolume(0);
            moment.setVolume(0);
            comingSoonVideo.play();
            comingSoonVideo.setLoopState(OF_LOOP_NORMAL);
        }
        else{
            comingSoonVideo.stop();
            left.setVolume(volumeTicking);
            right.setVolume(volumeTicking);
            gong.setVolume(1);
            moment.setVolume(volumeMoment);
        }
    }
    else if(key =='s'){
        ofxSaveCamera(cam, "ofEasyCamSettings.xml");
        gui.saveToFile("guiSettings.xml");
    }


}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::exit(){

    day.manager.close();

    ofxSaveCamera(cam, "ofEasyCamSettings.xml");
    gui.saveToFile("guiSettings.xml");

    
    

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


