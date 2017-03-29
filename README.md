# Light-Clock
Interactive installation at the Carnegie Museum of Art for the Hillman Photography Initiative. Allows you to spin through a 360 dynamic time lapse.
![alt text](https://raw.githubusercontent.com/CMP-Studio/Light-Clock/master/clock.jpeg)
Image courtesy of Carnegie Museum of Art. Photo by Bryan Conley.

![alt text](https://raw.githubusercontent.com/CMP-Studio/Light-Clock/master/inSpace.jpg)
![alt text](https://raw.githubusercontent.com/CMP-Studio/Light-Clock/master/spinGif.gif )
# Description

The Light Clock is actually comprised of two main components:

* The curious clock itself (outside the museum), which conveys the passing of time through a continuously swooping solitary hand. This hand makes a rotation every 5 minutes and each time it gets to the top, the clock captures a 360º image of the museum plaza. It will do this 24/7 for 15 months, resulting in hundreds of thousands of images. Every one of these images is instantly sent inside the museum to…

* An interactive visualization (in the museum lobby) that remixes the captured imagery into a participatory experience for museum visitors. We’ve installed several large displays and an interaction zone, where visitors physically spin their bodies to control their point-of-view (spinning left) as time continually spins itself.

[Full process write up.](https://studio.carnegiemuseums.org/clock-9aa6da28a4e5#.gulegkgzi)
[process video.](https://vimeo.com/183861520)


# Targets

* ubuntu 16.04
* Mac os 10.11 

# Dependencies

* openFrameworks version 9.2. [Here](http://openframeworks.cc/download/) are the openFrameworks install instructions.
* ofxGui
* ofxOpenCv
* ofxEasing
* ofxCameraSaveLoad
* ofxOpticalFlowFarneback
* ofxV4L2Settings
* ofxCsv
* ofxGoogleAnalytics
* ofxSimpleHttp

# Necessary Hardware
* usb camera to sense the rotation for the optical flow.
