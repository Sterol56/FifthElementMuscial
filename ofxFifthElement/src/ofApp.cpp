#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	endGame = 0;
	increase = 1;
	hits = 0;
	topHits = 0;

	background.loadImage("back2.jpg");
	suckGamePic.loadImage("suck2.jpg");
	winPic.loadImage("normal.png");

	panel.setup();
	panel.add(frequency.setup("BeatFrequency", .25, -1, 1));
	panel.add(filterCutoff.setup("filter Cutoff", 0.0, 0.0, .99));
	panel.add(chorusDepth.setup("chorusDepth", .01, 0, .0045));
	panel.add(chorusFrequency.setup("chorusFrequency", 70.86, 1, 500));
	panel.add(reverbRoomSize.setup("reverbRoomSize", .9, 0, 1));
	panel.add(chorusOn.setup("chorusOn", false));
	panel.add(reverbOn.setup("reverbOn", false));
	
	chorus.setEffectMix(1.0);
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
	endInfo.loadFont("verdana.ttf", 30);

	volume = 0.4;





    // This gets called once when we start our app

    // Make the app full screen
    ofSetFullscreen(false);
    
    // Turn on depth testing so rendering happens according to z-depth rather
    // than draw order.
    //ofEnableDepthTest();
    
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
	
	miss = 1;
	musicTimer = 0;
	colordelay = 50;
	fireQueue.push(299);
	fireQueue.push(434);
	fireQueue.push(585);
	fireQueue.push(785);
	fireQueue.push(938);
	fireQueue.push(1251);
//	fireQueue.push(1548);
	fireQueue.push(1770);
	fireQueue.push(1900);
	fireQueue.push(2158);
//	fireQueue.push(2422);
	fireQueue.push(2500);
	fireQueue.push(2562);
	fireQueue.push(2782);
	fireQueue.push(3494);
	fireQueue.push(3820);

	fireTimer = fireQueue.front();

	waterQueue.push(370);
	waterQueue.push(732);
//waterQueue.push(887);
	waterQueue.push(1002);
	waterQueue.push(1380);
	waterQueue.push(1466);
	waterQueue.push(1610);
	waterQueue.push(1842);
	waterQueue.push(1967);
//	waterQueue.push(2048);
	waterQueue.push(2224);
//	waterQueue.push(2600);
	waterQueue.push(2888);
	waterQueue.push(3568);
	waterQueue.push(3900);
	waterTimer = waterQueue.front();

	earthQueue.push(493);
	earthQueue.push(878);
	earthQueue.push(1093);
	earthQueue.push(1327);
	earthQueue.push(2037);
//	earthQueue.push(2071);
	earthQueue.push(2584);
	earthQueue.push(2646);
	earthQueue.push(2833);
	earthQueue.push(2924);
	earthQueue.push(3250);
	earthQueue.push(3424);
	earthQueue.push(3789);
	earthTimer = earthQueue.front();

	windQueue.push(658);
	windQueue.push(846);
	windQueue.push(1190);
	windQueue.push(1380);
	windQueue.push(1514);
//	windQueue.push(1656);
	windQueue.push(1706);
//	windQueue.push(1995);
	windQueue.push(2387);
//	windQueue.push(2544);
//	windQueue.push(2650);
	windQueue.push(2962);
	windQueue.push(2962);
	windQueue.push(3035);
	windQueue.push(3655);
	windQueue.push(3737);
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
	volocity =24;

	reso = 0;
	resoSun = 3;
	

}

//--------------------------------------------------------------
void ofApp::update(){
    // This gets called once for each graphical frame, right before draw()
    
    // Update orbit rotation angles -- larger increment == faster rotation.
    sunRotationAngle += 0.2;
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

	if (hits >= 15) {
		filter.setZero(0.9);
		sunRotationAngle += 0.2;
		colordelay = 25;
		increase = 5;
	}else if(hits >= 10) {
		filter.setZero(0.5);
		sunRotationAngle += 0.2;
		colordelay = 40;
		reso = 1;
		resoSun = 1;
		increase = 3;
	}else if(hits >= 5) {
		//beat.setFrequency(0.15);
		sunRotationAngle += 0.2;
		colordelay = 45;
		resoSun = 2;
		reso = 1;
		increase = 2;
	}
	else {
		increase = 1;
		sunRotationAngle += 0.1;
		filter.setZero(0.0);
		//beat.setFrequency(0.00999999);
		colordelay = 50;
		reso = 0;
		resoSun = 3;
	}

	if (musicTimer >= 4100) {
		if (topHits < hits) {
			topHits = hits;
		}
		shouldPlayAudio = false;
		endGame = 1;
	}
	//if (score < 0) {
	//	beat.setFrequency(0.0099);
	//}
	//else if (score > 0) {
	//	beat.setFrequency(0.01);
	//}
	//else if (score > 10) {
	//	beat.setFrequency(0.15);
	//}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	if (endGame == 0) {
		background.draw(-100, -100,-100, 1920	,1200);
		//panel.draw();
		ofSetColor(0, 255, 0);
		instructions.drawString("Press space to start/stop.  Fire for chorus. Wind for reverb", 10, 200);
		instructions.drawString("Water/Earth to volume up/down.", 10, 212);
		instructions.drawString("Fire for chorus. Wind for reverb", 10, 224);
		ofDrawBitmapString("Score: " + ofToString(score), 10, 248);

		ofDrawBitmapString("Hits: " + ofToString(hits), 10, 272);
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

		ofTranslate(ofGetWindowWidth()*0.5, ofGetWindowHeight()*0.5);

		ofPushMatrix(); // Save center state

		ofRotateZ(sunRotationAngle); // Sun's rotation about its axis

		ofPushStyle(); // Save initial style state
			// Set the color of the sun
		if (musicTimer >= fireTimer - colordelay && musicTimer <= fireTimer) {
			fireState = 1;
			ofSetColor(256, 0, 0);
		}
		else if (musicTimer >= waterTimer - colordelay && musicTimer <= waterTimer) {
			waterState = 1;

			ofSetColor(0, 0, 256);
		}
		else if (musicTimer >= earthTimer - colordelay && musicTimer <= earthTimer) {
			earthState = 1;
			ofSetColor(0, 255, 0);
		}
		else if (musicTimer >= windTimer - colordelay && musicTimer <= windTimer) {
			windState = 1;
			ofSetColor(0, 256, 255);
		}
		else {
			fireState = 0;
			waterState = 0;
			windState = 0;
			earthState = 0;

			if (musicTimer > fireTimer) {
				//if (miss = 1) {
				//	score--;
				//}
				//else {
				//	miss = 1;
				//}
				if (!fireQueue.empty()) {
					fireQueue.pop();
					if (!fireQueue.empty()) {

						fireTimer = fireQueue.front();
					}
				}
			}
			if (musicTimer > waterTimer) {
				//if (miss = 1) {
				//	score--;
				//}
				//else {
				//	miss = 1;
				//}
				if (!waterQueue.empty()) {
					waterQueue.pop();
					if (!waterQueue.empty()) {

						waterTimer = waterQueue.front();
					}
				}
			}
			if (musicTimer > earthTimer) {
				//if (miss = 1) {
				//	score--;
				//}
				//else {
				//	miss = 1;
				//}
				if (!earthQueue.empty()) {
					earthQueue.pop();
					if (!earthQueue.empty()) {

						earthTimer = earthQueue.front();
					}
				}
			}
			if (musicTimer > windTimer) {
				//if (miss = 1) {
				//	score--;
				//}
				//else {
				//	miss = 1;
				//}
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
		sun.setResolution(resoSun);
		sun.drawWireframe();

		ofPopStyle(); // Back to initial style state

		ofPopMatrix(); // Back to center state

		// Rotate our axes and then translate, for Earth orbit
		//ofRotateZ( earthOrbitAngle );
		int r = 300;
		earth.setGlobalPosition(r, 0, 0);
		water.setGlobalPosition(-r, 0, 0);
		wind.setGlobalPosition(0, r, 0);
		fire.setGlobalPosition(0, -r, 0);
		earth.setResolution(reso);
		water.setResolution(reso);
		fire.setResolution(reso);
		wind.setResolution(reso);


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

		ofSetColor(255, 255, 255);



		//case d earth
		if (shootEarth == 100) {
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
				if (miss = 1) {
					miss = 0;
				}
				if (earthState == 1) {
					score += increase;
					hits += 1;
					if (chorusOn == 1) {
						chorusOn = 0;
					}
					if (reverbOn == 0) {
						reverbOn = 1;
					}
					ofLog() << "Score up " << score << endl;
					miss = 0;
				}
				else {
					score -= increase;
					if (topHits < hits) {
						topHits = hits;
					}
					hits = 0;
					ofLog() << "Score down " << score << endl;
					if (chorusOn == 0) {
						chorusOn = 1;
					}
					if (reverbOn == 1) {
						reverbOn = 0;
					}
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
				if (miss = 1) {
					miss = 0;
				}
				if (waterState == 1) {
					if (chorusOn == 1) {
						chorusOn = 0;
					}
					if (reverbOn == 0) {
						reverbOn = 1;
					}
					score += increase;
					hits++;
					ofLog() << "Score up " << score << endl;
					miss = 0;
				}
				else {
					score -= increase;
					if (topHits < hits) {
						topHits = hits;
					}
					hits = 0;
					ofLog() << "Score down " << score << endl;
					if (chorusOn == 0) {
						chorusOn = 1;
					}
					if (reverbOn == 1) {
						reverbOn = 0;
					}
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
				if (miss = 1) {
					miss = 0;
				}
				if (fireState == 1) {
					if (chorusOn == 1) {
						chorusOn = 0;
					}
					if (reverbOn == 0) {
						reverbOn = 1;
					}
					score += increase;
					hits++;
					miss = 0;
					ofLog() << "Score up " << score << endl;
				}
				else {
					score -= increase;
					if (topHits < hits) {
						topHits = hits;
					}
					hits = 0;

					ofLog() << "Score down " << score << endl;
					if (chorusOn == 0) {
						chorusOn = 1;
					}
					if (reverbOn == 1) {
						reverbOn = 0;
					}

				}

				shootFire = 0;
				//if (chorusOn == false) {

				//	//chorusOn = true;
				//	chorusOn = true;
				//}
				//else {
				//	//chorusOn = false;
				//	chorusOn = false;
				//}
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
			if (moonWind.getY() < 120) {
				if (miss = 1) {
					miss = 0;
				}
				if (windState == 1) {
					if (chorusOn == 1) {
						chorusOn = 0;
					}
					if (reverbOn == 0) {
						reverbOn = 1;
					}

					score += increase;
					hits++;
					miss = 0;
					ofLog() << "Score up " << score << endl;
				}
				else {
					score -= increase;
					if (topHits < hits) {
						topHits = hits;
					}
					hits = 0;
					ofLog() << "Score down " << score << endl;
					if (chorusOn == 0) {
						chorusOn = 1;
					}
					if (reverbOn == 1) {
						reverbOn = 0;
					}

				}
				//if (reverbOn == false) {

				//	//chorusOn = true;
				//	reverbOn = true;
				//}
				//else {
				//	//chorusOn = false;
				//	reverbOn = false;
				//}
				shootWind = 0;
			}
		}
		else {
			windMove = 0;
			//moon.setGlobalPosition(0, 450, 0);
		}

		ofPopMatrix(); // Back to initial state at the top-left of our window.
	}
else {
	
	if (score <= 40) {
		
		suckGamePic.draw(200, 0);
		ofSetColor(256, 0, 0);
		endInfo.drawString("Game Is Over, You suck!", 280, 200);
		ofDrawBitmapString("Score: " + ofToString(score), 480, 428);

		ofDrawBitmapString("Maximum Hits: " + ofToString(topHits), 480, 452);

		ofSetColor(256, 256, 256);

	}
	else {
		//background.draw(-100, -100, 1920, 1200);
		winPic.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		ofSetColor(255, 204, 0);
		endInfo.loadFont("verdana.ttf", 20);
		endInfo.drawString("Game Is Over, You made the fifth Element and saved Earth!", 80, 150);
		endInfo.loadFont("verdana.ttf", 15);
		endInfo.drawString("Score: " + ofToString(score), 460, 188);

		endInfo.drawString("Maximum Hits: " + ofToString(topHits), 460, 212);

		ofSetColor(256, 256, 256);
	}

}



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
