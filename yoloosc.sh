#! /bin/sh
#
# app run wrapper script
#
# Copyright (c) 2022 ZKM | Hertz-Lab
# Dan Wilcox <dan.wilcox@zkm.de>
#
# BSD Simplified License.
# For information on usage and redistribution, and for a DISCLAIMER OF ALL
# WARRANTIES, see the file, "LICENSE.txt," in this distribution.
#
# This code has been developed at ZKM | Hertz-Lab as part of „The Intelligent
# Museum“ generously funded by the German Federal Cultural Foundation.

# if using the debug build, change APP to YoloOSCDebug
APP=YoloOSC
DIR="$(dirname $0)/bin"
EXEC=${DIR}/${APP}

# platform specifics
case "$(uname -s)" in
	Linux*)  ;;
	Darwin*)
		# invoke executable inside .app bundle
		EXEC="$DIR"/${APP}.app/Contents/MacOS/${APP}
		;;
	CYGWIN*) ;;
	MINGW*)  ;;
	*)       ;;
esac

# TODO: add lib paths for Linux?
# go
$EXEC $@
