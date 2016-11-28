#pragma once

#include "ofMain.h"
#include "ofxStk.h"
#include "ofxGui.h"

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



    // OpenFrameworks has many handy graphical primitive objects
    ofSpherePrimitive sun;
    ofSpherePrimitive earth;
	ofSpherePrimitive fire;
	ofSpherePrimitive wind;
	ofSpherePrimitive water;
	ofSpherePrimitive love;

	ofSpherePrimitive moon;
    ofSpherePrimitive moonFire;
	ofSpherePrimitive moonEarth;
	ofSpherePrimitive moonWater;
	ofSpherePrimitive moonWind;

	ofPath path;
    
    float sunRotationAngle;
    float earthRotationAngle;
	queue<int> fireQueue;
	queue<int> waterQueue;
	queue<int> earthQueue;
	queue<int> windQueue;
	int musicTimer;
	int fireTimer;
	int waterTimer;
	int earthTimer;
	int windTimer;
	int colordelay;

	int fireState;
	int waterState;
	int windState;
	int earthState;
	
	int score;
	int miss;



	int shoot;
	int shootEarth;
	int shootFire;
	int shootWater;
	int shootWind;
	int myCircleX;
	int myCircleY;
	int volocity;
	float fireMove;
	float waterMove;
	float earthMove;
	float windMove;
    float earthOrbitAngle;
    float moonOrbitAngle;
	float volume;

	void audioOut(float *output, int bufferSize, int nChannels);
	void chorusParametersChanged(float &value);
	void reverbParametersChanged(float &value);
	void frequencyChanged(float &value);
	void filterCutoffChanged(float &value);

	ofxPanel panel;
    ofxFloatSlider frequency;
    ofxFloatSlider chorusDepth;
    ofxFloatSlider chorusFrequency;
    ofxFloatSlider reverbRoomSize;
    ofxFloatSlider filterCutoff;
    ofxToggle reverbOn;
    ofxToggle chorusOn;
    
    stk::FileLoop beat;
    stk::FreeVerb reverb;
    stk::Chorus chorus;
    stk::OneZero filter;
    
    ofTrueTypeFont instructions;
    bool shouldPlayAudio;
};
