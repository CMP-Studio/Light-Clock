


#convert each image into its equirectangular version
#nona is part of hugin http://hugin.sourceforge.net/
/Applications/hugin/HuginTools/nona -o out template.pto start_R.tif start_F.tif

#stitch them together
#/Applications/hugin/HuginTools/enblend -o /Users/carolinerecord/Desktop/accessThis/testCompare2.tif out0000.tif out0001.tif
/Applications/hugin/HuginTools/enblend -o /Users/carolinerecord/Documents/of_v0.9.2_osx_release/apps/myApps/blendedImagery/bin/data/sampleImages/sample/005_2016-08-19_R/64_00-20-01.tif out0000.tif out0001.tif



# delete the intermediary files 
#rm out0000.tif out0001.tif
