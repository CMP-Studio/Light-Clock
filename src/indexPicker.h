//
//  indexPicker.h
//  BlendImagery
//
//  Created by Caroline Record on 3/7/17.
//
//

#include "ofMain.h"

class indexPicker{
    public:
        bool setup(int sz);
        int nextIndex();
    private:
        int sizeOfArray;
        vector<int> taken;
};