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
#pragma once

#define PACKAGE	"YoloOSC"
#define	VERSION	"1.0.0"
#define DESCRIPTION "yolo live object identification to osc"

// defaults
#define SEND_HOST "localhost"
#define SEND_PORT 7765

// use Syphon on macOS, comment this if not needed
#if defined(__APPLE_CC__) && !defined(YOLOOSC_USE_SYPHON)
	#define YOLOOSC_USE_SYPHON
#endif
#define SYPHON_NAME "frame"
