#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight()); 
    
    ofSetVerticalSync(true);
    
    
    
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
    int width = 2700;
    int height = 1429;
    
    gui.add(cropLeftRight.setup("crop left right",height/2,0,width));
    gui.add(cropTop.setup("crop top",height/2,0,height));
    gui.add(cropBottom.setup("crop bottom",height/2,0,height));
    gui.add(reCropEveryThing.setup("Re-initialize Crop"));
    
    gui.add(camUpDown.setup("Cam Up Down",0,-width,width));
    gui.add(camZoom.setup("Cam Zoom",90,10,180));
    gui.add(maxSpinTime.setup("spin max speed",height/2,0,width));
    gui.add(soundFrequency.setup("Sound frequency",height/2,0,width));
    
    
    reCropEveryThing.addListener(this, &ofApp::cropTrigger);
    camUpDown.addListener(this, &ofApp::camPosChanged);
    camZoom.addListener(this, &ofApp::camZoomChanged);
    
    gui.loadFromFile("guiSettings.xml");
    
    cam.setFov(camZoom);
    cam.setPosition(0,camUpDown,0);
    
    ofxLoadCamera(cam, "ofEasyCamSettings");
    
    
    cropLevel = 600; 
    
    DayFade temp;
    //string dirName, int numDay, int crpTop, int crpBottom, int cropLeftRight

    temp.setup( "sampleImages", 0, cropTop,cropBottom,cropLeftRight);
    days.push_back(temp);
    
    int num1 = cropTop;
    ofLog() << "cropTop: " << ofToString(num1);
    int num2 = cropBottom;
    ofLog() << "cropBottom: " << ofToString(num2);
    int num3 = cropLeftRight;
    ofLog() << "cropLeftRight: " << ofToString(num3);
    
    getText.allocate( days.at(0).imgWidth, days.at(0).originalImgHeight, GL_RGB);

    int blue = ofGetFrameNum();
    
    isEaseOut = false;
    isSpin = false;
    ellipseX = ofGetWidth()/2;
    speed = 3;
    
    flock.load("flock.mov");
    flock.setLoopState(OF_LOOP_NORMAL);
    flock.play();
    
    curMoment.load("currentMoment.jpg");
    
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
    right.load("sounds/right.wav");
    moment.load("sounds/click.wav");
    
}

void ofApp::camZoomChanged(int &camZoom){
    ofLog()<< "zoom called";
    cam.setFov(camZoom);
    
}

void ofApp::camPosChanged(int &camYpos){
    ofLog()<< "camera zoomed";
    cam.setPosition(0, camYpos, 0);
}

void ofApp::cropTrigger(){
    
    int num1 = cropTop;
    ofLog() << "cropTop: " << ofToString(num1);
    int num2 = cropBottom;
    ofLog() << "cropBottom: " << ofToString(num2);
    int num3 = cropLeftRight;
    ofLog() << "cropLeftRight: " << ofToString(num3);
    
    ofLog()<<"ping";
    for(int i=0; i < days.size(); i++){
        days.at(i).addCroppedImages(cropTop,cropBottom,cropLeftRight);
    }
    
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    for(int i=0; i< days.size(); i++){
        days.at(i).update();
    }
    
    
    flock.update();
    if(flock.isFrameNew()){
        ofLog()<<"this is happening";
        ofTexture curFrame = flock.getTexture();
        ofFbo temp;
        temp.allocate(curMoment.getWidth(),  curMoment.getHeight());
        temp.begin();
            curFrame.draw(0, 0, curMoment.getWidth(),  curMoment.getHeight());
        temp.end();
        curMoment.getTexture().setAlphaMask(temp.getTexture());
    }
    
    getText.begin();

    ofEnableAlphaBlending();
    ofClear(0);
    ofBackground(0,0,255);
    for(int i=0; i< days.size(); i++){
        days.at(i).draw(0,cropTop,cropLeftRight);
    }
    
    

    curMoment.draw(days.at(0).imgPos * -1 ,0);

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
    cam.begin();
    //sphere.draw();
    getText.getTexture().bind();
    sphere.draw();
    //sphere.drawWireframe();
    getText.getTexture().unbind();
    cam.end();

 
 /*
    ofClear(0);
    ofBackground(0,0,255);
    for(int i=0; i< days.size(); i++){
        days.at(i).draw(0,0,cropLeftRight);
    }
  */
   
    
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()),40, ofGetHeight()-40,0);
    
    if(showGui){
        gui.draw();
    }

}

//-------------------------w-------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'w'){
        isSpin = true;
    }
    
    else if (key == 'e'){
        
        days.at(0).imgPos +=10;
        left.play();
    }
    else if(key == 'r'){
        days.at(0).imgPos -=10;
    }
    else if (key == 'd'){
        days.at(0).mskPos +=10;
    }
    else if(key == 'f'){
        days.at(0).mskPos -=10;
        right.play();
    }
    else if (key == 'h'){
        showGui = !showGui; 
    }
    else if (key == ' '){
        moment.play();
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