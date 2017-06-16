#pragma once

#include "ofMain.h"
#include "ofxLeapMotion2.h"

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();

    
    int color[7] = {0, 35, 70, 125, 160, 190, 220};
    
    ofSoundPlayer sound[7];
    int size_circle = 0;
    float * volume;
    
    ofxLeapMotion leap;
    vector <ofxLeapMotionSimpleHand> simpleHands;
    vector <ofVec2f> fingerPos;
    vector <ofVec2f> handPos;
    ofPoint hand[4];
    ofEasyCam cam;
    
    ofVec2f pos[7];
    ofVec2f vel[7];
    
    int x_circle = 0;
    int y_circle = 0;
    
    int stage_mode = 1;
    ofMesh backMesh;
};
