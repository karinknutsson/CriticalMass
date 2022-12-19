#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "apple.h"
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
    
        ofVideoGrabber vidGrabber;
        int camWidth;
        int camHeight;
        
        ofxCvColorImage image;
        ofxCvGrayscaleImage grayImage;
        ofxCvGrayscaleImage grayImagePrev;
        ofxCvGrayscaleImage diff;
        ofxCvFloatImage diffFloat;
        ofxCvFloatImage bufferFloat;
        
        // ofImage apple;
        // Apple* apple;
    
        vector<Apple> apples;
        vector<Apple> applesToRemove;
    
        int currTime;
        int delay;
};
