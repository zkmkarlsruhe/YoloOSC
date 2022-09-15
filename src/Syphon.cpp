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
 *
 * Adapted from the loaf project: https://github.com/danomatika/loaf
 * Copyright (c) 2016 Dan Wilcox <danomatika@gmail.com>
 */
#include "Syphon.h"

#include "config.h"

#ifdef YOLOOSC_USE_SYPHON

#include "ofxSyphon.h"

//--------------------------------------------------------------
bool Syphon::supported() {
	return true;
}

//--------------------------------------------------------------
Syphon::Syphon() {}

//--------------------------------------------------------------
Syphon::~Syphon() {
	stop();
}

//--------------------------------------------------------------
void Syphon::start() {
	if(!server) {
		server = new ofxSyphonServer();
		server->setName(this->name);
	}
}

//--------------------------------------------------------------
void Syphon::stop() {
	if(server) {
		delete server;
		server = nullptr;
	}
}

//--------------------------------------------------------------
bool Syphon::isPublishing() {
	return (server != nullptr);
}

//--------------------------------------------------------------
void Syphon::publish() {
	if(server) {
		server->publishScreen();
	}
}

//--------------------------------------------------------------
void Syphon::publishTexture(const ofTexture & tex) {
	if(server) {
		server->publishTexture((ofTexture *)&tex);
	}
}

//--------------------------------------------------------------
void Syphon::setName(const std::string &name) {
	this->name = name;
	if(server) {
		server->setName(this->name);
	}
}

//--------------------------------------------------------------
std::string Syphon::getName() {
	if(server) {
		return server->getName();
	}
	return this->name;
}

//--------------------------------------------------------------
ofxSyphonServer *Syphon::getServer() {
	return server;
}

#else

//--------------------------------------------------------------
// dummy implementation
bool Syphon::supported() {return false;}
Syphon::Syphon() {}
Syphon::~Syphon() {}
void Syphon::start() {}
void Syphon::stop() {}
bool Syphon::isPublishing() {return false;}
void Syphon::publish() {}
void Syphon::setName(const std::string &name) {}
std::string Syphon::getName() {return this->name;}

void Syphon::publishTexture(const ofTexture & tex) {}

#endif
