#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup(){
    gameStart = false;
    gameOver = false;
    countDown = true;
    score = 0;
    camWidth = 1280;
    camHeight = 720;
    currentTime = ofGetElapsedTimeMillis();
    
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
    
    virusKillSound.load("arcade-sound.wav");
    virusKillSound.setMultiPlay(true);
    gameOverSound.load("game-over.wav");
    eightBitWonder.load("8-bit-wonder.ttf", 32, true, true);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // update sound
    ofSoundUpdate();
    
    // update camera input
    vidGrabber.update();
    
    // neded for video not to have magenta tint
    ofSetColor(255);
    
    // wait to start game until camera is on
    if (!gameStart && vidGrabber.isFrameNew()) {
        gameStart = true;
    }
    
    if (!countDown && gameStart && !gameOver) {
        
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
                                virusKillSound.play();
                                score += 10;
                            }

                        }
                        
                    }
                }
            }
            
            // add viruses over time
            if (ofGetElapsedTimeMillis() > currentTime + delay) {
                
                // create viruses and add to container
                int size = ofRandom(50, 180);
                viruses.push_back(Virus(ofRandom(ofGetWidth() - size), ofRandom(ofGetHeight() - size * 2), size));
                
                // reset current time
                currentTime = ofGetElapsedTimeMillis();
            }
        }
        
    }
    
    // iterate through dying viruses
    for (int i = dyingViruses.size() - 1; i >= 0; i--) {
        
        // if dying animation is done, delete it
        if (dyingViruses.at(i).resizeFactor < 0) {
            dyingViruses.erase(dyingViruses.begin() + i);
        }
        
    }
    
    // decrement delay so game gets increasingly more difficult with time
    delay -= 1;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (countDown) {
        
        // TODO: countdown text & sound
        
        countDown = false;
        
    } else if (!gameOver) {
        
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
        
        // draw score
        eightBitWonder.drawString(std::to_string(score), 20, camHeight - 30);
        
    } else {
        
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
