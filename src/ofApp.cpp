#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup(){

    // set up game to start
    gameOver = false;
    countDown = true;
    score = 0;
    startGame = false;
    withinFrame = true;
    camWidth = 1280;
    camHeight = 720;
    currentTime = ofGetElapsedTimeMillis();
    beep1played = false;
    beep2played = false;
    beep3played = false;
    longBeepPlayed = false;

    // black background
    ofBackground(0);

    // delay in milliseconds between each virus
    delay = 1000;

    // with how many milliseconds to decrement delay on each update
    decrementDelay = 0.5;

    // critical mass for viruses
    criticalMass = 12;

    // get back a list of devices
    devices = vidGrabber.listDevices();

    // set device id to first one found & initialize
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(camWidth, camHeight);
    currentDeviceId = 0;

    // load sounds and fonts
    virusKillSound.load("arcade-sound.wav");
    virusKillSound.setMultiPlay(true);
    gameOverSound.load("game-over.wav");
    shortBeep.load("short-beep.mp3");
    longBeep.load("long-beep.mp3");
    soundTrack.load("hubbard-commando.mp3");
    soundTrack.setVolume(0.5);
    soundTrack.setLoop(true);
    eightBitWonder32.load("8-bit-wonder.ttf", 32, true, true);
    eightBitWonder64.load("8-bit-wonder.ttf", 64, true, true);
}

//--------------------------------------------------------------
void ofApp::update(){

    // update camera input
    vidGrabber.update();

    // needed for video not to have magenta tint
    ofSetColor(255);

    // wait to start game until camera is on
    if (!startGame && vidGrabber.isFrameNew()) {
        startGame = true;
        countDownStartTime = ofGetElapsedTimef();
    }

    // start game after count down
    if (!countDown && !gameOver) {

        // play music
        if (!soundTrack.isPlaying()) {
          soundTrack.play();
        }

        // check if critical mass has been reached
        if (viruses.size() > criticalMass) {
            gameOver = true;
            vidGrabber.close();
            soundTrack.setPaused(true);
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

                        // check if player is within video frame
                        if (x < 10 || x > 1270 || y < 10) {
                            withinFrame = false;
                            goto endOfUpdate;
                        } else {
                            withinFrame = true;
                        }

                        // iterate through all viruses
                        for (int i = viruses.size() - 1; i >= 0; i--) {

                            // since video is mirrored, this needs to be adjusted for when comparing x position
                            int xMirrored = camWidth - x;

                            // if movement is close enough to an virus, it dies
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
            if (withinFrame && ofGetElapsedTimeMillis() > currentTime + delay) {

                // create viruses of random size and add to container
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
    delay -= decrementDelay;

    endOfUpdate:

    // update sound
    ofSoundUpdate();
}

//--------------------------------------------------------------
void ofApp::draw(){

    if (startGame) {

        if (countDown) {

            // magenta tint screen
            ofSetColor(255, 99, 234);

            // draw mirrored webcam input
            vidGrabber.draw(camWidth, 0, -camWidth, camHeight);

            // set white color for text
            ofSetColor(255);

            float elapsedTime = ofGetElapsedTimef() - countDownStartTime;

            // draw countdown & play sounds
            if (elapsedTime < 1) {
                if (!beep1played) {
                    shortBeep.play();
                    beep1played = true;
                }
                eightBitWonder64.drawString("1", (camWidth / 2) - 48, (camHeight / 2) - 16);
            } else if (elapsedTime < 2) {
                if (!beep2played) {
                    shortBeep.play();
                    beep2played = true;
                }
                eightBitWonder64.drawString("2", (camWidth / 2) - 48, (camHeight / 2) - 16);
            } else if (elapsedTime < 3) {
                if (!beep3played) {
                    shortBeep.play();
                    beep3played = true;
                }
                eightBitWonder64.drawString("3", (camWidth / 2) - 48, (camHeight / 2) - 16);
            }
            else if (elapsedTime < 5) {
                if (!longBeepPlayed) {
                    longBeep.play();
                    longBeepPlayed = true;
                }
                eightBitWonder64.drawString("GET READY", (camWidth / 2) - 360, (camHeight / 2) - 16);
            } else {
                countDown = false;
            }
        } else if (!withinFrame) {

                // magenta color tint
                ofSetColor(255, 99, 234);

                // draw mirrored webcam input
                vidGrabber.draw(camWidth, 0, -camWidth, camHeight);

                // white text
                ofSetColor(255);
                eightBitWonder32.drawString("PLEASE MOVE BACK WITHIN FRAME", 50, (camHeight / 2) - 24);
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
            eightBitWonder32.drawString(std::to_string(score), 20, camHeight - 30);
        } else {

            // magenta color for text
            ofSetColor(255, 99, 234);

            // draw game over text
            eightBitWonder32.drawString("GAME OVER\n\nSCORE " + std::to_string(score), 440, (camHeight / 2) - 48);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

  if (key == 32) {
    int nextDeviceId = currentDeviceId + 1;
    if (nextDeviceId >= devices.size()) {
      vidGrabber.setDeviceID(0);
    } else {
      vidGrabber.setDeviceID(nextDeviceId);
    }
  }

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
