#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    isMuteMode = false;
    
    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
    ofSetVerticalSync(true);
    
    usingFlow=true;
    //rotSense.setup();
    
    ofLog()<< "get width: " << ofGetWindowWidth();
    
   //dayOne.setup("dayOneOne.png", "day1");
   //dayTwo.setup("dayTwoTwo.png", "day2");

    /*
    for (int i=0; i< 4; i++){
        DayFade temp;
        temp.setup( "day" + ofToString(i), i);
        days.push_back(temp);
    }
     */
    
    
    
    gui.setup();
    gui.setPosition(50, 50);
    ofLog()<<"loading old settings";
   
   // cac
    
    int width = 4130;
    int height = 1984;
    
    gui.add(cropLeftRight.setup("crop left right",height/2,0,width));
    gui.add(cropTop.setup("crop top",height/2,0,height));
    gui.add(cropBottom.setup("crop bottom",height/2,0,height));
    gui.add(reCropEveryThing.setup("Re-initialize Crop"));
    
    gui.add(camZoom.setup("Cam Zoom",90,10,180));
    gui.add(camUpDown.setup("Cam Y",0,-width,width));
    gui.add(camZpos.setup("Cam Z",20,width,-width));
    gui.add(camXpos.setup("Cam X",20,width,-width));
    gui.add(maxSpinTime.setup("spin max speed",height/2,0,width));
    gui.add(soundFrequency.setup("Sound frequency",height/2,0,width));
    gui.add(noiseParameters.setup("latent state",""));
    gui.add(rangeImgMove.setup("Image movement range", ofVec2f(-1,1), ofVec2f(-6,-6),ofVec2f(6,6)));
    gui.add(imgMoveSpeed.setup("speed of image", 4 , 0,30 ));
    gui.add(rangeMskMove.setup("Mask movement range", ofVec2f(-1,1), ofVec2f(-6,-6),ofVec2f(6,6)));
    gui.add(mskMoveSpeed.setup("speed of msk", 4 , 0,30 ));
   
    //ofxLabel noiseParameters;
    //
    
    
  
    
    reCropEveryThing.addListener(this, &ofApp::cropTrigger);
    camZoom.addListener(this, &ofApp::camZoomChanged);
   
    camUpDown.addListener(this, &ofApp::camPosChanged);
    
    camZpos.addListener(this, &ofApp::camZposChanged);
    camXpos.addListener(this, &ofApp::camXposChanged);
    
    gui.loadFromFile("guiSettings.xml");
    
    cam.setFov(camZoom);
    
    ofxLoadCamera(cam, "ofEasyCamSettings.xml");
    cam.enableMouseInput();
   
    
    cropLevel = 600; 
    

    ofImage testSz;
    testSz.load("sampleImages/sample/000_2016-08-19_C/002_00-40-01.png");
    getText.allocate( testSz.getWidth(), testSz.getHeight(), GL_RGB);
    
    day.setup( "newImagery", 0, cropTop,cropBottom,cropLeftRight,testSz.getWidth(), testSz.getHeight());
    
    int num1 = cropTop;
    ofLog() << "cropTop: " << ofToString(num1);
    int num2 = cropBottom;
    ofLog() << "cropBottom: " << ofToString(num2);
    int num3 = cropLeftRight;
    ofLog() << "cropLeftRight: " << ofToString(num3);
    
    // get sample image to get size
    // eventually replace this with something smarter
    
    

    int blue = ofGetFrameNum();
    
    isEaseOut = false;
    isSpin = false;
    ellipseX = ofGetWidth()/2;
    speed = 3;
    
    //flock.load("flock.mov");
    //flock.setLoopState(OF_LOOP_NORMAL);
    //flock.play();
    
    
    //dayOne.imgScrollX = (dayOne.imgWidth-(dayOne.windowWidth+speed))-1;
    
    
    // set up 3D sphere
    sphere.setRadius(ofGetWidth());
    sphere.setPosition( 0,0,0 );
    //sphere.set
    ofSetSphereResolution(24);
    
    //texture.load("blended.jpg");
   // texture.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );
    
    ofEnableLighting();
    showGui = true;
    
    //getText.enableAlphaBlending();
    
    left.load("sounds/left.wav");
    left.setVolume(0.3f);
    right.load("sounds/right.wav");
    moment.load("sounds/click.wav");
    gong.load("sounds/gong.mp3");
    gongMultiple.load("sounds/gongMultiple.wav");
    
    curMoment.load("current.png");
    flock2.setup(curMoment.getWidth(), curMoment.getHeight() );
    
    momentAlphaShader.load("shader_alphaMsk2/shadersGL3/shader");
    
    momentAlphaShader.begin();
    momentAlphaShader.setUniform1i("imgWidth", curMoment.getWidth() );
    momentAlphaShader.end();
    
    currentMoment.allocate( curMoment.getWidth(), curMoment.getHeight() );
    currentMomentMask.allocate(curMoment.getWidth(), curMoment.getHeight() );
    
    // image to test full resolution
    fullRes.load("bestStitch3.png");
    isFullResTest = false;
    
    timeSinceInteract = ofGetElapsedTimeMillis();
    isLatent = true;
    
    
    
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
    day.addCroppedImages(cropTop,cropBottom,cropLeftRight);
    //}
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    left.setVolume(0.3f);
    
    if(day.currentMomentTrig){
        ofLog()<< "trigger!! Timeese";
        day.currentMomentTrig = false;
        flock2.setMinSize(0);
        flock2.triggerSequenceTwo();
        gong.play();
    }
    
    /*
    if(usingFlow){
        rotSense.update();
        days.at(0).imgPos += rotSense.getCwVelocity();
        days.at(0).mskPos += rotSense.getCcwVelocity();
    }
     */


    //ofLog() << "is flocking happening: " << flock2.isSequenceTwo;
    if((!isLatent)&((ofGetElapsedTimeMillis()- timeSinceInteract) >= 5000)){
        isLatent = true;
    }
    else if (isLatent){
        /*
         float output =ofMap(ofNoise(1,ofGetElapsedTimef()/mskMoveSpeed),0,1,rangeMskMove->x,rangeMskMove->y);
        day.mskPos -= output;
        
        day.imgPos += ofMap(ofNoise(50, ofGetElapsedTimef()/imgMoveSpeed),0,1,rangeImgMove->x,rangeImgMove->y);
         */
    
    }
    
    day.update();
    flock2.update();
    //day.
    //curMoment.getTexture().setAlphaMask(flock2.drawIntoMe.getTexture());
    
    /*
    flock.update();
    if(flock.isFrameNew()){
        ofLog()<<"this is happening";
        ofTexture curFrame = flock.getTexture();
      ofFbo temp;
        temp.allocate(curMoment.getWidth(),  curMoment.getHeight());
        temp.begin();
            curFrame.draw(0, 0, curMoment.getWidth(),  curMoment.getHeight());
        temp.end();
        
        
    }*/
 
    getText.begin();

    ofEnableAlphaBlending();
    ofClear(0);
    ofBackground(0,0,255);
    
    ofPushMatrix();
    
    //ofScale(-1,1,1);
    //ofTranslate(getText.getWidth() * -1 , 0);
    
    day.draw(0,cropTop,cropLeftRight);
    
    
    
    
    
    // current moment
    // this isn't wrapping right now and I need it to.
    /*
    momentAlphaShader.begin();
        momentAlphaShader.setUniformTexture("imageMask", flock2.drawIntoMe.getTexture(), 1);
        momentAlphaShader.setUniform1i("mskXPos", days.at(0).mskPos);
        momentAlphaShader.setUniform1i("imgXPos", days.at(0).imgPos);
        curMoment.draw(0 ,0);
    momentAlphaShader.end();
     */

    currentMoment.begin();
        ofClear(0);
        float drawImgPos = wrapCurrentMoment( day.imgPos);
        if (drawImgPos + currentMoment.getWidth() > currentMoment.getWidth()){
            curMoment.draw( drawImgPos *-1,0, currentMoment.getWidth() , currentMoment.getHeight() );
            curMoment.draw( drawImgPos *-1 +  currentMoment.getWidth() ,0,currentMoment.getWidth() , currentMoment.getHeight());
        }
        else{
            curMoment.draw( drawImgPos *-1,0,currentMoment.getWidth() , currentMoment.getHeight());
        }
    currentMoment.end();
    
    if(flock2.isSequenceTwo){
        currentMomentMask.begin();
            ofClear(0);
            float drawMskPos =  wrapCurrentMoment( day.mskPos);
            if (drawImgPos + currentMoment.getWidth() > currentMoment.getWidth()){
                flock2.drawIntoMe.draw( drawMskPos *-1,0, currentMoment.getWidth() , currentMoment.getHeight() );
                flock2.drawIntoMe.draw( drawMskPos *-1 +  currentMoment.getWidth() ,0,currentMoment.getWidth() , currentMoment.getHeight());
            }
            else{
                flock2.drawIntoMe.draw( drawMskPos *-1,0,currentMoment.getWidth() , currentMoment.getHeight());
            }
        currentMomentMask.end();
    
        day.manager.curMoment.image.getTexture().setAlphaMask(currentMomentMask.getTexture());
        //currentMoment.getTexture().setAlphaMask(currentMomentMask.getTexture());
        day.manager.curMoment.draw(0,0);
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
        ofSetColor(0, 0, 255);
        ofDrawEllipse(cropLeftRight, cropTop , 50,50);
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

    //ofSetColor(255,255,255,255);
    // ofEnableAlphaBlending();
    // ofDisableAlphaBlending();
    
    
   // glEnable(GL_ALPHA_TEST);
   // glAlphaFunc(GL_GREATER, 0.0f);
    //fbo.draw(0, 0, 500, 500);
   // getText.draw(-50,-500);
    //glDisable(GL_ALPHA_TEST);
    
    
    ofBackground(0);
    
    
    if(!isMuteMode){
        cam.begin();
        getText.getTexture().bind();
        sphere.draw();
        //sphere.drawWireframe();
        getText.getTexture().unbind();
        cam.end();
    
    
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()),40, ofGetHeight()-40,0);

    
    if(showGui){
        gui.draw();
    }
    if(usingFlow){
        //rotSense.draw();
    }
    } else{
        // draw video for the mute mode
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
        day.imgPos +=10;
        left.play();
        isLatent = false;
        timeSinceInteract = ofGetElapsedTimeMillis();
    }
    else if(key == 'w'){
        day.mskPos -=10;
        right.play();
        isLatent = false;
        timeSinceInteract = ofGetElapsedTimeMillis();
    }
    else if (key == 'h'){
        showGui = !showGui; 
    }
    else if (key == ' '){

        isFullResTest = !isFullResTest;
    }
    else if(key == 'v'){
        flock2.setMinSize(0);
        flock2.triggerSequence();
        moment.play();

    }
    else if(key == 'b'){
        flock2.setMinSize(0);
        flock2.triggerSequenceTwo();
        moment.play(); 
    }
    else if(key == 'n'){
        flock2.setMinSize(0);
        flock2.triggerSequenceTwo();
        gong.play();
    }
    else if(key == 'm'){
        flock2.setMinSize(0);
        flock2.triggerSequenceTwo();
        gongMultiple.play();
    }
    else if(key == ','){
        
        flock2.triggerSequenceTwo();
        flock2.setMinSize(30);
        gongMultiple.play();
    }
    else if(key == 'x'){
        isMuteMode = !isMuteMode;
        if (isMuteMode){
            left.setVolume(0);
            right.setVolume(0);
            gong.setVolume(0);
            gongMultiple.setVolume(0);
        }
        else{
            left.setVolume(1);
            right.setVolume(1);
            gong.setVolume(1);
            gongMultiple.setVolume(1);
        }
    }


}


// t- current time //
// b- start value
// c- change in value
// d- duration
float ofApp::easeOut(){
    float t = ofGetFrameNum();
    float d = 80;
    float percent = ofMap(t, begFrame, begFrame+d, 0,1);
    float r = 1.0f - ( percent - 1.0f) * (percent - 1.0f);
    float toReturn = ofMap(r, 0,1,speed,0 );
    
    if (t >= begFrame + d){
        isEaseOut = false;
    }
    
    //ofLog() << toReturn;
    return toReturn;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'w'){

        isSpin = false;
        isEaseOut = true;
        //beginningValue = dayOne.mskScrollX;
        //beginningValue = ellipseX;
        begFrame = ofGetFrameNum();
        ofLog()<< "beg frame: " << begFrame;
        
    }


    
   
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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
