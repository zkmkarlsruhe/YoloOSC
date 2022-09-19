#! /bin/sh
# configure custom settings in Makefile-based projects
# 1. run this script
# 2. generate projects files with oF ProjectGenerator
# 3. make & run
# Dan Wilcox <dan.wilcox@zkm.de> 2022

# stop on error
set -e

##### variables

PLATFORM="$(uname -s)"

##### main

# change to project root dir
cd $(dirname "$0")
cd ../

# of path from config.make
OF_PATH=$(cat config.make| grep "OF_ROOT =" | grep -v "#" | awk -F" = " '{ print $2 }')

# remove ofxSyphon dependency on non-macOS systems
if [ "$PLATFORM" != "Darwin" ] ; then
	sed -i '' '/ofxSyphon$/d' addons.make
	echo "addons.make removing ofxSyphon"
fi

# Makefile: custom make targets
"$OF_PATH"/addons/ofxTensorFlow2/scripts/configure_makefile.sh .
