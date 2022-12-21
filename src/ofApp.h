#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "virus.h"
#include <vector>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        // webcam input
        ofVideoGrabber vidGrabber;
    
        // camera dimensions
        int camWidth;
        int camHeight;
        
        // current video frame
        ofxCvColorImage image;
    
        // current video frame in grayscale
        ofxCvGrayscaleImage grayImage;
    
        // previous video frame in grayscale
        ofxCvGrayscaleImage grayImagePrev;
    
        // absolute difference of current and previous frame
        ofxCvGrayscaleImage diff;
    
        // amplified difference of current and previous frame
        ofxCvFloatImage diffFloat;
    
        // buffer image: stores all differences in movement, with damping
        ofxCvFloatImage bufferFloat;
    
        // virus container
        vector<Virus> viruses;
    
        // variables to keep track of time
        int currTime;
        int delay;
    
        // sound and text variables
        ofSoundPlayer sound;
        ofSoundPlayer gameOverSound;
        ofTrueTypeFont eightBitWonder;
    
        // game variables
        bool gameStart;
        bool gameOver;
        int score;
};
