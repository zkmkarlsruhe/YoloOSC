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

#include "ofApp.h"
#include "CLI11.hpp"

/// commandline option parser
class Commandline {

	public:

		/// constructor with required app instance
		Commandline(ofApp *app);

		/// parse commandline options
		/// returns true if program should continue or false if it should exit
		bool parse(int argc, char **argv);

		/// print parser error and return exit code
		int exit();

		ofApp *app = nullptr;              ///< required app instance
		CLI::App parser;                   ///< parser instance
		CLI::Error error = CLI::Success(); ///< parse error if program should exit
};
