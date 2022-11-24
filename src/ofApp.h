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
#pragma once

#include "ofMain.h"
#include "ofxTensorFlow2.h"
#include "ofxYolo.h"
#include "ofxOsc.h"
#include "config.h"
#include "Syphon.h"

struct ofAppSettings {
	std::string host = SEND_HOST; ///< send host address or name
	int port = SEND_PORT; ///< send port
	int device = 0; ///< desired input device id
	int rate = 30; ///< desired input framerate
	struct {
		int width = 640;
		int height = 480;
	} size; ///< desired input size
	bool syphon = false; ///< start syphon?
};

class ofApp : public ofBaseApp {

	public:

	/// \section ofApp

		ofApp() {}
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		/// load settings from a file, returns false on error
		bool loadFile(const std::string &path);

	/// \section Data

		// config settings
		ofAppSettings settings;

		// model
		ofxYolo yolo;

		// input source
		ofVideoGrabber video;
		ofImage frame; ///< cached video frame
		struct {
			bool horz = false;
			bool vert = false;
		} mirror; ///< input mirroring

		// osc
		ofxOscSender sender;

		// syphon
		Syphon syphon;

		// resources
		ofTrueTypeFont font;
};
