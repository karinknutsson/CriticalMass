#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup(){
    camWidth = 1280;
    camHeight = 720;
    currTime = ofGetElapsedTimeMillis();
    
    // delay in milliseconds between each apple
    delay = 3000;

    //get back a list of devices
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(size_t i = 0; i < devices.size(); i++){
        if (devices[i].bAvailable) {
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        } else {
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(camWidth, camHeight);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0);
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew()) {
        
        // if grayImage exists, store it for next iteration
        if (grayImage.bAllocated) {
            grayImagePrev = grayImage;
        }
        
        // get pixel data from video and set in image
        image.setFromPixels(vidGrabber.getPixels());
        
        // convert to grayscale
        grayImage = image;
        
        // if previous grayImage exists, perform calculations
        if (grayImagePrev.bAllocated) {
            
            // set diff to be difference between current and previous grayImage
            diff.absDiff(grayImage, grayImagePrev);
            
            // convert to float image & amplify
            diffFloat = diff;
            diffFloat *= 5.0;
            
            // update the accumulation buffer
            if (!bufferFloat.bAllocated) {
                
                // if bufferFloat does not exist, set it equal to diffFloat
                bufferFloat = diffFloat;
            } else {
                
                // if it does exist, first dampen the bufferFloat
                bufferFloat *= 0.85;
                
                // add current difference to the buffer
                bufferFloat += diffFloat;
            }
        }
    }

    
    if (ofGetElapsedTimeMillis() > currTime + delay) {
        
        // create apples with minimum width 40 px and maximum width 160 px
        int width = ofRandom(40, 160);
        int height = width * 1.15;
        apples.push_back(Apple(width, height, ofRandom(ofGetWidth() - width), ofRandom(ofGetHeight() - height * 2)));
        
        // reset current time
        currTime = ofGetElapsedTimeMillis();
    }
    
    
    // if diffFloat exists,
    if (diffFloat.bAllocated) {
        
        // get image dimensions
        int w = grayImage.width;
        int h = grayImage.height;
    
        // get bufferFloat pixels
        float *pixels = bufferFloat.getPixelsAsFloats();
        
        // scan all pixels
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                
                // get pixel value
                float value = pixels[x + w * y];
                
                if (value >= 2 && !apples.empty()) {
                    for (int i = apples.size() - 1; i >= 0; i--) {
                        
                        // since video is mirrored, this needs to be adjusted for when comparing x position
                        int xMirrored = camWidth - x;
                        
                        // if movement is close enough to an apple, delete it
                        if ((xMirrored > apples.at(i).x && xMirrored < apples.at(i).x + apples.at(i).w) && (y > apples.at(i).y && y < apples.at(i).y + apples.at(i).w)) {
                            apples.erase(apples.begin() + i);
                        }
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw mirrored webcam input
    vidGrabber.draw(camWidth, 0, -camWidth, camHeight);

    for (auto apple = apples.begin(); apple != apples.end(); ++apple)
    {
        // draw each apple
        apple->draw();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
