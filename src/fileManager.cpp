//
//  fileManager.cpp
//  FunWithFiles
//
//  Created by Caroline Record on 8/24/16.
//
//


#include "fileManager.h"

void fileManager::setup(int numOfImgToLoad,int crpTop, int crpBot){
    
    lengthOfDeck = numOfImgToLoad;
    numMomentsToGoThrough = 5;
    amountOfskiping = 1;

    crpT = crpTop;
    crpB = crpBot;
    
    startPath = "/media/caroline/Storage/pano/";
   // startPath = "/Users/carolinerecord/Documents/of_v0.9.2_osx_release/apps/myApps/blendedImagery/bin/data/samples/sample/";
    
    momentsToTraverse = 8;
    
    setUpDays();
    //test.setup("005_2016-08-19_R/016_00-20-01.png");
    
    ofImage testSz;
    testSz.load(nextFileToLoad);

    //testQ.clear();
    //poolOfThreads.clear();
    //ofLog()<< numOfImgToLoad;
    for(int i =0; i < numOfImgToLoad; i++){
        //testQ.push_back(move(shared_ptr<oneImage>(new oneImage)));
        //ofLog()<< i;
        poolOfThreads.push_back(move(unique_ptr<oneImage>(new oneImage)));
        string temp = nextFileToLoad;
        //ofLog()<< "I am requesting it to load an image start";
        poolOfThreads.back()->setup(temp, crpT, crpB);
        //ofLog()<<"next file to load: " << nextFileToLoad;
        nextMoment();
        indexOfThreads.push_back(i);
        //sleep(5);
    }
/*
    for(int i =0; i < numOfImgToLoad; i++){
        testQ.push_back(move(shared_ptr<oneImage>(new oneImage)));
        string temp = nextFileToLoad;
        testQ.back()->setup(temp, crpT, crpB);
        //ofLog()<<"next file to load: " << nextFileToLoad;
        nextMoment();
    }
    */
    posX = 0;
    isCurrentDay = false;

    lastFileName = "";
    numOfTimesStuck = 0;

}

void fileManager::update(){
    checkNewDay();
    checkNewMoment();
    curMoment.update();
    //posX-=5;
}


bool fileManager::check(int xPos, int thresh, int interval){

    if (xPos < thresh){
        return false;
    }
    else {
        int indexOnTheMove = indexOfThreads.back();
        if(poolOfThreads.at(indexOnTheMove)->isReady){
            lastFileName = "";
            numOfTimesStuck = 0;
            indexOfThreads.pop_back();
            indexOfThreads.push_front(indexOnTheMove);
            //ofLog()<< "I am requesting it to load an image";
            poolOfThreads.at(indexOnTheMove)->setup(nextFileToLoad, crpT, crpB);
            //ofLog()<<"setup complete";
            nextMoment();
            return true;
        }
        else{
            //ofLog()<< "failing because the index on the move isn't loaded";
            string curFilePath = poolOfThreads.at(indexOnTheMove)->filePath;
            if(lastFileName == curFilePath){
                numOfTimesStuck ++;
                if (numOfTimesStuck > 5){
                    //ofLog()<< "got the mouse";
                    poolOfThreads.at(indexOnTheMove)->isQuit =true;
                }
            }
            numOfTimesStuck ++;
            lastFileName = curFilePath;
            return false;

        }
        /*
        // take out the one that just disapeared from the right

        ///testQ.pop_back();
        testQ.erase(testQ.end()-1);
        //ofLog()<< "about to pop one out:";
        //ofLog()<<"test q"<< testQ.size();
        //if(testQ.front()->isLoaded){
        //testQ.back()->destroyEverything();
        testQ.at(lengthOfDeck-1)->destroyEverything();

        //testQ.erase(testQ.end()-1);
        //ofLog()<< "just removed one";
        //ofLog()<<"after removing: "<< testQ.size();
        // insert the one to the left

        testQ.push_front(move(shared_ptr<oneImage>(new oneImage)));
        //ofLog()<< "just added one";
        testQ.front()->setup(nextFileToLoad, crpT, crpB);
        //ofLog()<<"setup complete";
        nextMoment();
        //ofLog() << "adding one to the front: " << testQ.size();
        return true;
        //}
        */
    }
    
    /*
     if (xPos < -1){
     testQ.pop_front();
     return false;
     //removing one from the left
     }
     else if (xPos < thresh){
     return false;
     }
     else {
     // take out the one that just disapeared from the right
     testQ.pop_back();
     // insert the one to the left
     
     testQ.push_front(move(unique_ptr<oneImage>(new oneImage)));
     testQ.front()->setup(nextFileToLoad, crpT, crpB);
     nextMoment();
     //ofLog() << "adding one to the front: " << testQ.size();
     return true;
     
     }
     */
    
    // if I want time to move counter clock wise
    /*
    if (xPos > thresh){
        return false;
    }
    else {
        // take out the one that just disapeared from the left
        testQ.pop_front();
        // insert the one from the rights
        testQ.push_back(move(unique_ptr<oneImage>(new oneImage)));
        testQ.back()->setup(nextFileToLoad, crpT, crpB);
        nextMoment();
        return true;
    }
     */
}

void fileManager::draw(int index, int x, int y){
    //test.draw(0,0,1000,1000);
    int indexOfImage = indexOfThreads.at(index);
    poolOfThreads.at(indexOfImage)->update();
    poolOfThreads.at(indexOfImage)->draw(x,y);

   // testQ.at(index)->update();
    //testQ.at(index)->draw(x,y);
}


void fileManager::setUpDays(){
    // day selection
    lastWeather = "";
    dayCount = 0;
    day.listDir(startPath);
    day.sort();
    numOfDays = day.size();
    vector<string> parts;
    parts = ofSplitString(day.getPath(day.size()-1), "_");
    currentDayWeather = parts.at(parts.size()-1);
    makeUnusedDaysMap();
    moment.allowExt("tif");
    moment.listDir(day.getPath(day.size()-1));
    // not necessary to sort bc I am just getting a count.
    numOfMoments = moment.size();
    indexDay =selectDay();
    moment.listDir(day.getPath(indexDay));
    moment.sort();
    indexMoment = int (ofRandom(0, moment.size() - (momentsToTraverse +1)));
    indexMoment = ofClamp(indexMoment, 0, moment.size());



    //indexMoment = int(ofRandom(0,moment.size() - (numMomentsToGoThrough*amountOfskiping +1)));

    nextFileToLoad = moment.getPath(indexMoment);
    //ofLog()<< "how big" << moment.size();
}

int fileManager::selectDay(){
   
    // defines the next day to load.
    // and defines the starting position within the moment

    //int indexOfDay;

    //have it be limited so it won't load in a empty day
    dayCount ++;
    // how many files are in this folder
    
    // cannot select the most recent day until it has a certain number of moments in it!
    bool isBigEnough = true;
    dirToCheck.listDir(day.getPath(day.size()-1));
    if (dirToCheck.size() < (numMomentsToGoThrough*amountOfskiping +3)){
        isBigEnough = false;
    }

    if((dayCount % 10==0)& (isBigEnough)){
        isCurrentDay = true;
        lastWeather = currentDayWeather;
        return day.size()-1;
    }
    else if (dayCount % 6 ==0){
        return importantDays.at(importantDaysIndex.nextIndex());
    }
    else {
        // which weather
        int whichWeather = allWeatherIndex.nextIndex();
        int theDayinThatWeather = allWeatherIndexes.at(whichWeather).nextIndex();
        return allWeather.at(whichWeather).at(theDayinThatWeather);
    }
    
        
        
        /*
        isCurrentDay = false;
        if (mapOfDays.size() <= 0){
            // fill back up with days
            //ofLog() << "went through all of them";
            makeUnusedDaysMap();
        }
        // is it full? - aka at the beginning or just filled up
        // choose a random one
        map<int, string>::iterator it = mapOfDays.begin();
        if (mapOfDays.size() == numOfDays){
            // do the - 1.1 because you want to never do the most recent day
            // as that day is managed above
            int randomDay = int(ofRandom(mapOfDays.size()-1.1));
            it = mapOfDays.begin();
            advance(it, randomDay);
            int toReturnTwo = it->first;
            lastWeather = it->second;
            mapOfDays.erase(it);
            return toReturnTwo;
        }
        
        while(it != mapOfDays.end()){
            if (lastWeather != it->second){
                lastWeather= it->second;
                int toReturn = it->first;
                mapOfDays.erase(it);
                return toReturn;
            }
            it++;
        }
        
        // there was not one available with different weather
        // so just go with a random one
        int randomDay = int(ofRandom(mapOfDays.size()-1.1));
        it = mapOfDays.begin();
        advance(it, randomDay);
        int toReturnTwo = it->first;
        lastWeather = it->second;
        mapOfDays.erase(it);
        return toReturnTwo;
    }
         */
    
}

void fileManager::makeUnusedDaysMap(){
    vector<string> theParts;
    
    // make that list of days
    // minus one bc I do not want to include the current day in this list.
    
    vector<int> cloud;
    vector<int> snow;
    vector<int> nice;
    vector<int> rain;
    
    for(int i=0; i <day.size()-1; i++){
        theParts.clear();
        theParts = ofSplitString(day.getPath(i), "_");
        string weatherCode = theParts.at(theParts.size()-1);

        if(weatherCode == "I"){
            importantDays.push_back(i);
            
        }
        else if (weatherCode == "C"){
            cloud.push_back(i);
        }
        else if (weatherCode == "R"){
            snow.push_back(i);
        }
        else if (weatherCode == "S"){
            nice.push_back(i);
        }
        else if (weatherCode == "N"){
            rain.push_back(i);
        }
        
    }
    
    importantDaysIndex.setup(importantDays.size());
    
    if(cloud.size()>0){
        allWeather.push_back(cloud);
        indexPicker temp;
        temp.setup(cloud.size());
        allWeatherIndexes.push_back(temp);
    }
    if(snow.size()>0){
        allWeather.push_back(snow);
        indexPicker temp;
        temp.setup(snow.size());
        allWeatherIndexes.push_back(temp);
    }
    if(nice.size()>0){
        allWeather.push_back(nice);
        indexPicker temp;
        temp.setup(nice.size());
        allWeatherIndexes.push_back(temp);
    }
    if(rain.size()>0){
        allWeather.push_back(rain);
        indexPicker temp;
        temp.setup(rain.size());
        allWeatherIndexes.push_back(temp);
    }
    
    allWeatherIndex.setup(allWeather.size());
    
}

void fileManager::nextMoment(){
    // there are more moments in this day
    // if it is under the amount we want to traverse or it is at the end
    if((indexMoment <= (moment.size()-(amountOfskiping+1))) & (indexMoment <= endIndexForMoments)){
        indexMoment += amountOfskiping;
        //ofLog()<< "index of moment:" << indexMoment;
        
    }
    // no more moments in this day
    // go to the next day
    else {
        //ofLog()<< "I am loading in a new day";
        //indexMoment =0;
        indexDay = selectDay();
        moment.listDir(day.getPath( indexDay ));
        // here is where I define where the starting point of this is
        //if my day is greater than 230 and is not the current day
        // then apply night avoidance rules
        // if it is the current day then do the most recent images - so you are more likely
        // to see your self
        if(isCurrentDay){
            //ofLog() << "currently displaying current day";
            // set the start day to be as close to the current day as possible.
            indexMoment = moment.size() - (numMomentsToGoThrough*amountOfskiping +1);
        }
        if((moment.size()>= 200)){
            // night avoidance rules
            int endBuffer = (numMomentsToGoThrough*amountOfskiping +1);
            int rainBuffer = endBuffer + 60;
            indexMoment = int(ofRandom(80,moment.size() - rainBuffer ));
           // indexMoment = int(ofClamp(indexMoment,0,moment.size()));
            //ofLog() << "I AM THRESHOLDING FOR THE RAIN";
        }
        else {
            indexMoment = int(ofRandom(0,moment.size() - (numMomentsToGoThrough*amountOfskiping +1)));
            indexMoment = ofClamp(indexMoment,0,moment.size());

        }
        endIndexForMoments = indexMoment +numMomentsToGoThrough*amountOfskiping;
        moment.sort();
    }
    // check if the file has a flag on it
    string nextPotentialFile = moment.getPath(indexMoment);
    vector <string> temp = ofSplitString( nextPotentialFile, "_");
    string endId = temp.at(temp.size()-1);
    //ofLog()<< "endID: " << endId;
    endId = endId[0];
    //ofLog() << endId;
    if (endId != "E"){
        nextFileToLoad = nextPotentialFile;
    }
    else {
        //ofLog()<< "do not load this file";
    }
}

void fileManager::checkNewDay(){
    dirToCheck.listDir(startPath);
    // not necessary to sort because it is only to get a count
    if(numOfDays < dirToCheck.size()){
        //ofLog() << "new day";
        setUpDays();
    }
}

void fileManager::checkNewMoment(){

    dirToCheck.listDir(day.getPath(day.size()-1));
    
    if (numOfMoments < dirToCheck.size() ){
        dirToCheck.sort();
        numOfMoments = dirToCheck.size();
        // new Image - so draw it.
        string currentMomentPath =dirToCheck.getPath(dirToCheck.size()-1);
       //ofLog() << "New moment found: " + currentMomentPath;
        curMoment.setup(currentMomentPath, crpT, crpB);
        insertMomentCheck(currentMomentPath);
    }
}


void fileManager::close(){
    for(int i =0; i < lengthOfDeck; i++){
        // close them here..
        int indexOf = indexOfThreads.at(i);
        bool ready = poolOfThreads.at(indexOf)->isLoaded;
        if (!ready){
            sleep(1000);
            close();
        }
    }
}

void fileManager::insertMomentCheck(string momentPath){
    

    /*
    // a new moment was just added
    // if the current day is the one being processed then re-initialize the list so it will include that new image
    if(indexDay == day.size()-1){
        moment.listDir(day.getPath(indexDay));
        moment.sort();
    }
    // need to check if it should be inserted into the deque or whether it will just happen the next the current day is in active circulation
    // loop through all the days currently in the deque
    
    // what is folder ID of the current day?
    //ofLog()<< momentPath;
    // 005_2016-08-19_R/013_00-20-01.png
    string dayIdCur = ofSplitString(momentPath,"_").at(0);
    vector<string> temp = ofSplitString(momentPath, "/");
    string nameOfMomentCur = temp.at(temp.size()-1);
    int momentIdCur = ofToInt(ofSplitString(nameOfMomentCur, "_").at(0));
    
    for(int i=0; i < lengthOfDeck; i++){
        string fileName = testQ.at(i)->filePath;
        string dayId = ofSplitString(fileName, "_").at(0);
        if (dayId == dayIdCur){
            vector<string> partz = ofSplitString(fileName, "/");
            string nameOfMoment = partz.at(partz.size()-1);
            int momentId = ofToInt(ofSplitString(nameOfMoment, "_").at(0));
            
            if (momentId == momentIdCur-1){
                // put it after I in the deque
                testQ.insert(testQ.begin()+i+1,move(shared_ptr<oneImage>(new oneImage)) );
                testQ.at(i+1)->setup(momentPath, crpT, crpB);
                // also take one off the end to compensate so this doesn't grow the deck beyond what it should be
                testQ.pop_back();
                
                break;
            }
            //ofLog()<< momentId;
        }
    }

     deque<unique_ptr<oneImage>>::iterator it = testQ.begin();
     while(it != testQ.end()){
     // are any of them from the current day
     string = it->
     it++;
     }*/
    
    
    // is the last day in there - if it is insert most recent image into it
}
    
