//
//  indexPicker.cpp
//  BlendImagery
//
//  Created by Caroline Record on 3/7/17.
//
//

#include "indexPicker.h"


bool indexPicker::setup(int sz){
    sizeOfArray = sz;
    for (int i=0; i<sz; i++){
        taken.push_back(i);
    }
    if (sz >0){
        return true;
    }
    return false; 
}

int indexPicker::nextIndex(){
    if (taken.size() > 0){
        int random = int(ofRandom(taken.size()-1.1));
        int toReturn = taken.at(random);
        vector<int>::iterator it = taken.begin();
        advance(it, random);
        taken.erase(it);
        return toReturn;
    }
    else{
        setup(sizeOfArray);
        int random = int(ofRandom(taken.size()-1.1));
        int toReturn = taken.at(random);
        vector<int>::iterator it = taken.begin();
        advance(it, random);
        taken.erase(it);
        return toReturn;
    }
}