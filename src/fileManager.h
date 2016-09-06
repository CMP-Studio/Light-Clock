//
//  fileManager.hpp
//  FunWithFiles
//
//  Created by Caroline Record on 8/24/16.
//
//


#include "ofMain.h"
#include "oneImage.h"

class fileManager{
    public:
    
        void setup( int numOfImgToLoad, int crpTop, int crpBot);
        void update();
        void draw(int index, int x, int y);
        bool check(int xPos, int thresh, int interval);
        int crpT;
        int crpB;
        deque<shared_ptr<oneImage>> testQ;
        // the current moment
        oneImage curMoment;
        int lengthOfDeck;

    private:
    
    
    string startPath;
    
    //variables about the file path
    string nextFileToLoad;
    int indexDay;
    int indexMoment;
    int numOfDays;
    int numOfMoments;
    ofDirectory day;
    ofDirectory moment;
    string lastWeather;
    // maps of un-used days
    map<int,string> mapOfDays;
    // re-initialize list of unused days
    int dayCount;
    void makeUnusedDaysMap();
    int selectDay();
    string currentDayWeather;
    void checkNewMoment();
    void checkNewDay();
    ofDirectory dirToCheck;
    void setUpDays();
    void insertMomentCheck(string momentPath);
    void nextMoment();
    
    oneImage test;
    
    // the deque
    int imgWidth;
    int imgHeight;
    
    
    int posX; 


    
};
