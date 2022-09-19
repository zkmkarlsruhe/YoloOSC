/*
 * YoloOSC
 *
 * Copyright (c) 2022 ZKM | Hertz-Lab
 * Dan Wilcox <dan.wilcox@zkm.de>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * This code has been developed at ZKM | Hertz-Lab as part of „The Intelligent
 * Museum“ generously funded by the German Federal Cultural Foundation.
 */
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofSetWindowTitle("YoloOSC");
	ofEnableSmoothing();
	ofBackground(0);

	// model
	if(!yolo.setup("model", "classes.txt")) {
		std::exit(EXIT_FAILURE);
	}
	yolo.setNormalize(true);
	yolo.startThread();

	// camera input
	video.setDeviceID(settings.device);
	video.setDesiredFrameRate(settings.rate);
	if(!video.setup(settings.size.width, settings.size.height)) {
		std::exit(EXIT_FAILURE);
	}
	frame.allocate(video.getWidth(), video.getHeight(), OF_IMAGE_COLOR);
	if(video.getWidth() != ofGetWidth() || video.getHeight() != ofGetHeight()) {
		ofSetWindowShape(video.getWidth(), video.getHeight());
	}

	// osc
	if(!sender.setup(settings.host, settings.port)) {
		std::exit(EXIT_FAILURE);
	}

	// syphon
	syphon.setName(SYPHON_NAME);
	if(settings.syphon) {
		syphon.start();
	}

	// resources
	font.load(OF_TTF_MONO, 10);

	// print settings
	ofLogVerbose(PACKAGE) << "input size: " << video.getWidth() << "x" << video.getHeight();
	ofLogVerbose(PACKAGE) << "send host: " << sender.getSettings().host;
	ofLogVerbose(PACKAGE) << "send port: " << sender.getSettings().port;
	if(Syphon::supported() && syphon.isPublishing()) {
		ofLogVerbose(PACKAGE) << "started syphon";
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	video.update();
	if(video.isFrameNew()) {
		frame.setFromPixels(video.getPixels());
		if(mirror.horz || mirror.vert) {
			frame.mirror(mirror.vert, mirror.horz);
		}
		yolo.setInput(frame.getPixels());
		syphon.publishTexture(frame.getTexture());
	}
	if(yolo.update()) {
		auto objects = yolo.getObjects();
		ofxOscBundle bundle;
		ofxOscMessage message;
		
		message.setAddress("/begin");
		message.addInt32Arg(objects.size());
		bundle.addMessage(message);
		message.clear();

		if(!objects.empty()) {
			for(std::size_t i = 0; i < objects.size(); i++) {
				auto object = objects[i];
				message.setAddress("/object");
				message.addInt32Arg(i);
				message.addInt32Arg(object.ident.index);
				message.addStringArg(object.ident.text);
				message.addFloatArg(object.bbox.x);
				message.addFloatArg(object.bbox.y);
				message.addFloatArg(object.bbox.width);
				message.addFloatArg(object.bbox.height);
				message.addFloatArg(object.confidence);
				bundle.addMessage(message);
				message.clear();
			}
		}
		message.setAddress("/end");
		bundle.addMessage(message);
		message.clear();

		sender.sendBundle(bundle);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	float w = ofGetWidth(), h = ofGetHeight();

	// cached video input frame
	ofFill();
	ofSetColor(255);
	frame.draw(0, 0, w, h);

	// detected objects
	ofNoFill();
	auto objects = yolo.getObjects();
	for(int i = 0; i < objects.size(); i++) {
		auto object = objects[i];
		float x = object.bbox.x * w, y = object.bbox.y * h;

		// bounding box
		ofNoFill();
		ofSetColor(255, 0, 255);
		ofDrawRectangle(x, y, object.bbox.width * w, object.bbox.height * h);

		// text box
		std::string text = object.ident.text + "\n" + ofToString(object.confidence, 2);
		ofRectangle bbox = font.getStringBoundingBox(text, x-2, y-2);
		bbox.width += 4;
		bbox.height += 4;
		ofFill();
		ofSetColor(ofColor::black);
		ofDrawRectangle(bbox);

		// text
		ofSetColor(ofColor::white);
		font.drawString(text, x, y);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch(key) {
		case 'm':
			mirror.horz = !mirror.horz;
			break;
		case 'n':
			mirror.vert = !mirror.vert;
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}
