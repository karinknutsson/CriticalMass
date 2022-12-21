#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup(){
    gameStart = false;
    gameOver = false;
    score = 0;
    camWidth = 1280;
    camHeight = 720;
    currTime = ofGetElapsedTimeMillis();
    
    // black background
    ofBackground(0);
    
    // delay in milliseconds between each virus
    delay = 1000;

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
    
    sound.load("arcade-sound.wav");
    gameOverSound.load("game-over.wav");
    eightBitWonder.load("8-bit-wonder.ttf", 32, true, true);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // update camera input
    vidGrabber.update();
    
    // neded for video not to have magenta tint
    ofSetColor(255);
    
    // wait to start game until camera is on
    if (!gameStart && vidGrabber.isFrameNew()) {
        gameStart = true;
    }
    
    if (gameStart && !gameOver) {
        
        // cutoff for when game is over
        if (viruses.size() > 12) {
            
            gameOver = true;
            vidGrabber.close();
            gameOverSound.play();
            
        }
        
        if (vidGrabber.isFrameNew()) {
            
            // if grayImage exists, store it for next iteration
            if (grayImage.bAllocated) {
                grayImagePrev = grayImage;
            }
            
            // get pixel data from video and set in image
            image.setFromPixels(vidGrabber.getPixels());

            // blur image
            image.blur(11);
            
            // convert to grayscale
            grayImage = image;
            
            // increase contrast
            grayImage.contrastStretch();
            
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
            
            // create viruss and add to container
            int size = ofRandom(50, 180);
            viruses.push_back(Virus(ofRandom(ofGetWidth() - size), ofRandom(ofGetHeight() - size * 2), size));
            
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
                    
                    // cutoff value for movement
                    if (value >= 1.5 && !viruses.empty()) {
                        
                        // iterate through all viruses
                        for (int i = viruses.size() - 1; i >= 0; i--) {
                            
                            // since video is mirrored, this needs to be adjusted for when comparing x position
                            int xMirrored = camWidth - x;
                            
                            // if movement is close enough to an virus, delete it
                            if ((xMirrored > viruses.at(i).x && xMirrored < viruses.at(i).x + viruses.at(i).s) && (y > viruses.at(i).y && y < viruses.at(i).y + viruses.at(i).s)) {
                                dyingViruses.push_back(viruses.at(i));
                                viruses.erase(viruses.begin() + i);
                                sound.play();
                                score += 10;
                            }
                        }
                        
                    }
                }
            }
        }
    }
    
    // decrement delay so game gets increasingly more difficult
    delay -= 1;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (gameStart && !gameOver) {
        
        // draw mirrored webcam input
        vidGrabber.draw(camWidth, 0, -camWidth, camHeight);

        // draw each living virus
        for (auto virus = viruses.begin(); virus != viruses.end(); ++virus)
        {
            virus->draw();
        }
        
        // draw each dying virus
        for (auto virus = dyingViruses.begin(); virus != dyingViruses.end(); ++virus)
        {
            virus->drawDeath();
        }
        
        // magenta color for text
        ofSetColor(255, 99, 234);
        
        // draw score
        eightBitWonder.drawString(std::to_string(score), 20, 60);
        
    } else if (gameOver) {
        
        // magenta color for text
        ofSetColor(255, 99, 234);
        
        // draw game over text
        eightBitWonder.drawString("GAME OVER\n\nSCORE " + std::to_string(score), 440, (camHeight / 2) - 48);
        
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
