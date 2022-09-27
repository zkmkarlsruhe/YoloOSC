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
#include "Commandline.h"

Commandline::Commandline(ofApp *app) : app(app) {
	parser.description(DESCRIPTION);
}

bool Commandline::parse(int argc, char **argv) {

	// local options, the rest are ofAppSettings instance variables
	std::string size = "";
	bool list = false;
	bool verbose = false;
	bool version = false;
	std::string settings = "";

	parser.add_option("XML", settings, "XML settings file");

	parser.add_option("-a,--address", app->settings.host, "OSC send host address or name, default " + app->settings.host);
	parser.add_option("-p,--port", app->settings.port, "OSC send port, default " + ofToString(app->settings.port));
	parser.add_flag(  "-l,--list", list, "list input devices and exit");
	parser.add_option("-d,--dev", app->settings.device, "input device number, default " + ofToString(app->settings.device));
	parser.add_option("-r,--rate", app->settings.rate, "desired input framerate, default " + ofToString(app->settings.rate));
	parser.add_option("-s,--size", size, "desired input size, default " + ofToString(app->settings.size.width) + "x" + ofToString(app->settings.size.height));
	parser.add_flag("-m,--mirror", app->mirror.horz, "mirror horizontally");
	parser.add_flag("-f,--flip", app->mirror.vert, "flip vertically");
	parser.add_flag("--syphon", app->settings.syphon, "start streaming with Syphon (macOS only)");
	parser.add_flag("-v,--verbose", verbose, "verbose printing");
	parser.add_flag("--version", version, "print version and exit");

	try {
		parser.parse(argc, argv);
	}
	catch(const CLI::ParseError &e) {
		error = e;
		return false;
	}

	// verbose printing?
	ofSetLogLevel(PACKAGE, (verbose ? OF_LOG_VERBOSE : OF_LOG_NOTICE));

	// print version
	if(version) {
		std::cout << VERSION << std::endl;
		return false;
	}

	// list audio input devices
	if(list) {
		app->video.listDevices();
		return false;
	}

	// try loading settings file, commandline options then override settings
	if(settings != "") {
		if(!app->loadFile(settings)) {
			return false; // exit on error with user-supplied path
		}
		ofLogNotice(PACKAGE) << "loaded " << settings;
	}
	else {
		if(ofFile::doesFileExist(SETTINGS)) {
			if(app->loadFile(SETTINGS)) {
				ofLogNotice(PACKAGE) << "loaded " << SETTINGS;
			}
		}
	}

	// size: WxH, ie 640x480 or 1280X720
	if(size != "") {
		std::size_t found = size.find_last_of("x");
		if(found == std::string::npos) {
			found = size.find_last_of("X"); // try uppercase too
		}
		if(found != std::string::npos) {
			int w = ofToInt(size.substr(0, found));
			int h = ofToInt(size.substr(found+1));
			if(w == 0 || h == 0) {
				ofLogWarning(PACKAGE) << "ignoring invalid size: " << size;
			}
			else {
				app->settings.size.width = w;
				app->settings.size.height = h;
			}
		}
		else {
			ofLogWarning(PACKAGE) << "ignoring invalid size: " << size;
		}
	}

	// port
	if(app->settings.port <= 1024) {
		ofLogWarning(PACKAGE) << "ignoring OSC send port in system range (0-1024): " << app->settings.port;
		app->settings.port = SEND_PORT;
	}

	// host
	if(app->settings.host == "") {
		ofLogWarning(PACKAGE) << "ignoring empty send host address";
		app->settings.host = SEND_HOST;
	}

	return true;
}

int Commandline::exit() {
	return parser.exit(error);
}
