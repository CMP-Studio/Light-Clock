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

#include "singleDay.h"


void singleDay::setup(string imgToLoad, int crpTop, int crpBottom){
    //isWhat = 9;
    //loader.loadFromDisk(img, imgToLoad);
    img.load(imgToLoad);
    originalHeight = img.getHeight();
    img.crop(0,crpTop, img.getWidth(), (crpBottom - crpTop) );
    //isWrapped = false;
}

