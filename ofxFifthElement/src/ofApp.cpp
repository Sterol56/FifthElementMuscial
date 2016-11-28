#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	panel.add(frequency.setup("BeatFrequency", .25, -1, 1));
	panel.add(filterCutoff.setup("filter Cutoff", 0.0, 0.0, .99));
	panel.add(chorusDepth.setup("chorusDepth", .01, 0, .1));
	panel.add(chorusFrequency.setup("chorusFrequency", 500, 1, 500));
	panel.add(reverbRoomSize.setup("reverbRoomSize", .9, 0, 1));
	panel.add(chorusOn.setup("chorusOn", false));
	panel.add(reverbOn.setup("reverbOn", false));
	
	chorus.setEffectMix(0.5);
	reverb.setEffectMix(.5);

	frequency.addListener(this, &ofApp::frequencyChanged);
	chorusDepth.addListener(this, &ofApp::chorusParametersChanged);
	chorusFrequency.addListener(this, &ofApp::chorusParametersChanged);
	reverbRoomSize.addListener(this, &ofApp::reverbParametersChanged);
	filterCutoff.addListener(this, &ofApp::filterCutoffChanged);

	chorus.setModDepth(chorusDepth);
	chorus.setModFrequency(chorusFrequency);
	reverb.setRoomSize(reverbRoomSize);
	beat.setFrequency(frequency);
	filter.setZero(filterCutoff);


	beat.openFile(ofToDataPath("FE.wav", true));
	stk::Stk::setSampleRate(44100.0);
	ofSoundStreamSetup(2, 0);
	instructions.loadFont("verdana.ttf", 10);

	volume = 0.4;





    // This gets called once when we start our app

    // Make the app full screen
    ofSetFullscreen(false);
    
    // Turn on depth testing so rendering happens according to z-depth rather
    // than draw order.
    ofEnableDepthTest();
    
    // Black background
    ofBackground(0);
    
    // Set the size of our sphere "planets"
    sun.setRadius( 120.0 );
    earth.setRadius(60.0);
	fire.setRadius(60.0);
	wind.setRadius(60.0);
	water.setRadius(60.0);

	moonWind.setRadius( 12.0 );
	moonWater.setRadius(12.0);
	moonFire.setRadius(12.0);
	moonEarth.setRadius(12.0);
	moon.setRadius(12.0);
    
    // Initialize orbit rotation angles to zero
    sunRotationAngle = 0;

	musicTimer = 0;
	colordelay = 70;
	fireQueue.push(500);
	fireQueue.push(600);
	fireTimer = fireQueue.front();

	waterQueue.push(700);
	waterQueue.push(800);
	waterTimer = waterQueue.front();

	earthQueue.push(900);
	earthQueue.push(1000);
	earthTimer = earthQueue.front();

	windQueue.push(1100);
	windQueue.push(1200);
	windTimer = windQueue.front();

	fireState = 0;
	waterState = 0;
	windState = 0;
	earthState = 0;

	score = 0;


    earthOrbitAngle = 0;
    moonOrbitAngle = 0;
	fireMove = 0;
	waterMove = 0;
	windMove = 0;
	earthMove = 0;
	shoot = 0;
	shootWind = 0;
	shootWater = 0;
	shootFire = 0;
	shootEarth = 0;
	volocity =12;
	

}

//--------------------------------------------------------------
void ofApp::update(){
    // This gets called once for each graphical frame, right before draw()
    
    // Update orbit rotation angles -- larger increment == faster rotation.
    sunRotationAngle += 0.1;
    //earthRotationAngle += 2.0;
    //earthOrbitAngle += 0.5;
    //moonOrbitAngle += 2.5;

	fireMove +=volocity;
	waterMove += volocity;
	windMove += volocity;
	earthMove += volocity;
	if (shouldPlayAudio) {
		musicTimer++;
	}

    
}

//--------------------------------------------------------------
void ofApp::draw(){
	//panel.draw();
	instructions.drawString("Press space to start/stop.  Fire for chorus. Wind for reverb", 10, 200);
	instructions.drawString("Water/Earth to volume up/down.", 10, 212);
	instructions.drawString("Fire for chorus. Wind for reverb", 10, 224);
	ofDrawBitmapString("score: " + ofToString(score), 10, 236);
	//sun.setGlobalPosition(ofGetWindowWidth()*0.5, ofGetWindowHeight()*0.5,0);
	//ofPushMatrix();
	//	ofRotateZ(sunRotationAngle);
	//	ofPushStyle();
	//		ofSetColor(200, 150, 0);
	//		sun.drawWireframe();
	//	ofPopStyle();
	//ofPopMatrix();

    // This gets called once for each graphical frame, right after update()
    // This is where you draw all the graphics
    
    ofPushMatrix(); // Save our state for later

        // Since our coordinates start in the top left, translate to the center.
		
        ofTranslate( ofGetWindowWidth()*0.5, ofGetWindowHeight()*0.5 );
        
        ofPushMatrix(); // Save center state
        
            ofRotateZ( sunRotationAngle ); // Sun's rotation about its axis
            
            ofPushStyle(); // Save initial style state
                // Set the color of the sun
				if (musicTimer >= fireTimer - colordelay && musicTimer <= fireTimer) {
					fireState = 1;
					ofSetColor(256, 0, 0);
				}
				else if (musicTimer >= waterTimer - colordelay && musicTimer <= waterTimer) {
					waterState = 1;

					ofSetColor(0, 0, 256);
				}else if (musicTimer >=earthTimer - colordelay && musicTimer <= earthTimer) {
					earthState = 1;
					ofSetColor(0, 255, 0);
				}else if (musicTimer >= windTimer - colordelay && musicTimer <= windTimer) {
					windState = 1;
					ofSetColor(0, 256, 255);
				}else  {
					fireState = 0;
					waterState = 0;
					windState = 0;
					earthState = 0;

					if (musicTimer > fireTimer) {
						if (!fireQueue.empty()) {
							fireQueue.pop();
							if (!fireQueue.empty()) {

								fireTimer = fireQueue.front();
							}
						}
					}
					if (musicTimer > waterTimer) {
						if (!waterQueue.empty()) {
							waterQueue.pop();
							if (!waterQueue.empty()) {

								waterTimer = waterQueue.front();
							}
						}
					}
					if (musicTimer > earthTimer) {
						if (!earthQueue.empty()) {
							earthQueue.pop();
							if (!earthQueue.empty()) {

								earthTimer = earthQueue.front();
							}
						}
					}
					if (musicTimer > windTimer) {
						if (!windQueue.empty()) {
							windQueue.pop();
							if (!windQueue.empty()) {

								windTimer = windQueue.front();
							}
						}
					}
					ofSetColor(200, 150, 0);
				}

                // Draw the sun (wireframe so we can see rotation)
                sun.drawWireframe();

            ofPopStyle(); // Back to initial style state
        
        ofPopMatrix(); // Back to center state
    
        // Rotate our axes and then translate, for Earth orbit
        //ofRotateZ( earthOrbitAngle );
		int r = 300;
		earth.setGlobalPosition( r, 0, 0);
		water.setGlobalPosition(-r, 0, 0);
		wind.setGlobalPosition(0, r, 0);
		fire.setGlobalPosition(0, -r, 0);
    

					
                // Set the color of the fire
                ofSetColor(256, 0, 0);
                // Draw the fire (also wireframe so we can see rotation)
                fire.drawWireframe();
				// Set the color of the wind
				ofSetColor(0, 256, 255);
				// Draw the wind (also wireframe so we can see rotation)
				wind.drawWireframe();

				// Set the color of the water
				ofSetColor(0, 0, 256);
				// Draw the water (also wireframe so we can see rotation)
				water.drawWireframe();

				// Set the color of the Earth
				ofSetColor(0, 255, 0);
				// Draw the Earth (also wireframe so we can see rotation)
				earth.drawWireframe();



		//case d earth
		if (shootEarth== 100) {
			// Recall that our axes are still centered about the Earth.
			// Now do another rotate and translate, thus we rotate the moon about
			// the Earth.
			ofPushMatrix();
				//ofRotateY(moonOrbitAngle);
				 //ofRotateX( moonOrbitAngle ); // also rotate about the X direction?
				moonEarth.setGlobalPosition(+r - earthMove, 0, 0);
				

				ofPushStyle();  // Save initial style state
					// Set the Moon color
				ofSetColor(0, 256, 0);
				// Draw the moon
				moonEarth.drawWireframe();
				ofPopStyle(); // Back to initial style state
			ofPopMatrix();

			if (ofDist(moonEarth.getX(), moonEarth.getY(), sun.getX(), sun.getY()) < 132) {
				if (earthState == 1) {
					score += 1;
					ofLog() << "Score up " << score << endl;
				}
				else {
					score -= 1;
					ofLog() << "Score down " << score << endl;
				}
				shootEarth = 0;
				//if (volume >=0 ) {
				//	volume -= 0.2;
				//}
			}

		}
		else {
			earthMove = 0;
			//moon.setGlobalPosition(+450, 0, 0);
		}

		//case a water
		if (shootWater == 97) {
			// Recall that our axes are still centered about the water.
			ofPushMatrix();
			moonWater.setGlobalPosition(-r + waterMove, 0, 0);


			ofPushStyle();  // Save initial style state
							// Set the Moon color
			ofSetColor(0, 0, 256);
			// Draw the moon
			moonWater.drawWireframe();
			ofPopStyle(); // Back to initial style state
			ofPopMatrix();

			//collision
			if (ofDist(moonWater.getX(), moonWater.getY(), sun.getX(), sun.getY()) < 132) {
				if (waterState == 1) {
					score += 1;
					ofLog() << "Score up " << score << endl;
				}
				else {
					score -= 1;
					ofLog() << "Score down " << score << endl;
				}

				shootWater = 0;
				//if (volume <= 1) {
				//	volume += 0.2;
				//}
			}




		}
		else {
			waterMove = 0;
			//moon.setGlobalPosition(-450, 0, 0);
		}

		//case w fire
		if (shootFire == 119) {

			ofPushMatrix();
			moonFire.setGlobalPosition(0, -r + fireMove, 0);
			ofPushStyle();  // Save initial style state
							// Set the Moon color
			ofSetColor(256, 0, 0);
			// Draw the moon
			moonFire.drawWireframe();
			ofPopStyle(); // Back to initial style state
			ofPopMatrix();


			if (ofDist(moonFire.getX(), moonFire.getY(), sun.getX(), sun.getY()) < 132) {
				if (fireState == 1) {
					score += 1;
					ofLog() << "Score up " << score << endl;
				}
				else {
					score -= 1;
					ofLog() << "Score down " << score << endl;
				}

				shootFire = 0;
				if (chorusOn == false) {

					//chorusOn = true;
					chorusOn = true;
				}
				else {
					//chorusOn = false;
					chorusOn = false;
				}
			}

		}
		else {
			fireMove = 0;
			//moon.setGlobalPosition(0, -450, 0);
		}

		//case s wind
		if (shootWind == 115) {
			// Recall that our axes are still centered about the Earth.
			// Now do another rotate and translate, thus we rotate the moon about
			// the Earth.
			ofPushMatrix();

			moonWind.setGlobalPosition(0, +r - windMove, 0);


			ofPushStyle();  // Save initial style state
							// Set the Moon color
			ofSetColor(0, 256, 256);
			// Draw the moon
			moonWind.drawWireframe();
			ofPopStyle(); // Back to initial style state
			ofPopMatrix();
			if (moonWind.getY()< 120) {
				if (windState == 1) {
					score += 1;
					ofLog() << "Score up " << score << endl;
				}
				else {
					score -= 1;
					ofLog() << "Score down " << score << endl;
				}
				if (reverbOn == false) {

					//chorusOn = true;
					reverbOn = true;
				}
				else {
					//chorusOn = false;
					reverbOn = false;
				}
				shootWind = 0;
			}
		}
		else {
			windMove = 0;
			//moon.setGlobalPosition(0, 450, 0);
		}
    
    ofPopMatrix(); // Back to initial state at the top-left of our window.



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofLog() << "key " << key << endl;
	//case w
	switch (key) {
		case 100: shootEarth = 100;
			break;
		case 97: shootWater = 97;
			break;
		case 119: shootFire = 119;
			break;
		case 115:shootWind = 115;
			break;
	}
	if (key == ' ') {
		shouldPlayAudio = !shouldPlayAudio;
		ofLog() << "timer " << musicTimer << endl;
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

void ofApp::audioOut(float *output, int bufferSize, int nChannels) {
	if (shouldPlayAudio) {
		stk::StkFrames frames(bufferSize, 2);
		beat.tick(frames);

		// the beat is a 2 channel file , however most effects only work on on channel
		// so we will just use the left channel. If you want to apply effects on the second channel you need to
		//create two sets of effects one for the left channel and one for the right channel
		stk::StkFrames leftChannel(bufferSize, 1);
		// copy the left Channel of 'frames' into `leftChannel`
		frames.getChannel(0, leftChannel, 0);

		// applys a filter and writes back into leftChannel
		filter.tick(leftChannel);

		if (chorusOn) {
			// chrous takes a mono frame and outputs a stereo frame
			// we will only use the left channel of the chorus output
			stk::StkFrames chorusOut(bufferSize, 2);
			chorus.tick(leftChannel, chorusOut);
			chorusOut.getChannel(0, leftChannel, 0);
		}
		if (reverbOn) {
			// reverb also takes a mono frame and outputs a stereo frame
			// we will only use the left channel of the reverb output
			stk::StkFrames reverbOut(bufferSize, 2);
			reverb.tick(leftChannel, reverbOut, 0, 0);
			reverbOut.getChannel(0, leftChannel, 0);
		}
		for (int i = 0; i < bufferSize; i++) {
			output[2 * i] = leftChannel(i, 0) *volume;
			output[2 * i + 1] = leftChannel(i, 0) *volume;
		}
	}
}

void ofApp::chorusParametersChanged(float &value) {
	chorus.setModDepth(chorusDepth);
	chorus.setModFrequency(chorusFrequency);
}

void ofApp::reverbParametersChanged(float &value) {
	reverb.setRoomSize(value);
}

void ofApp::frequencyChanged(float &value) {
	beat.setFrequency(value);
}

void ofApp::filterCutoffChanged(float &value) {
	filter.setZero(value);
}
