#! /bin/sh
# configure custom settings in Xcode project files
# 1. generate projects files with oF ProjectGeneration
# 2. run this script
# 3. open Xcode project and build/run
# Dan Wilcox <dan.wilcox@zkm.de> 2022

# stop on error
set -e

##### variables

CONFIG=Project.xcconfig
PLIST=openFrameworks-Info.plist

PLISTBUDDY=/usr/libexec/PlistBuddy

##### main

# change to project root dir
cd $(dirname "$0")
cd ../

# of path from config.make
OF_PATH=$(cat config.make| grep "OF_ROOT =" | grep -v "#" | awk -F" = " '{ print $2 }')

# version from config.h: #define VERSION "1.2.3" -> 1.2.3
VER=$(cat src/config.h | grep "VERSION" | awk '{ gsub(/"/, "", $3); print $3 }')

# build from # of commits
BUILD=$(git rev-list --count HEAD)

# xcconfig: custom icon path & syphon define
echo "$CONFIG"
if grep -q "//YoloOSC" $CONFIG ; then
	# remove *all* lines after HEADER_SEARCH_PATHS
	# note: if the oF project templates change, this probably needs updating
	sed -i '' '/HEADER_SEARCH_PATHS/q' $CONFIG
fi
echo "" >> $CONFIG
echo "//YoloOSC" >> $CONFIG
echo 'ICON_FILE_PATH = bin/data/' >> $CONFIG
if grep -q "ofxSyphon" addons.make ; then
	echo 'OTHER_CFLAGS = $(OTHER_CFLAGS) -DYOLOOSC_USE_SYPHON' >> $CONFIG
fi

# plist: version, bundle ID, and other info
echo "$PLIST"
if $PLISTBUDDY -c 'print' $PLIST | grep -q "CFBundleShortVersionString" ; then
	$PLISTBUDDY -c "set :CFBundleShortVersionString $VER" $PLIST
else # version string is not in oF project template
	$PLISTBUDDY -c "add :CFBundleShortVersionString string $VER" $PLIST
fi
$PLISTBUDDY -c "set :CFBundleVersion $BUILD" $PLIST
$PLISTBUDDY -c 'set :CFBundleIdentifier de.zkm.${EXECUTABLE_NAME}' $PLIST
$PLISTBUDDY -c 'set :NSCameraUsageDescription "YoloOSC would like to access the camera to identify objects."' $PLIST
$PLISTBUDDY -c 'set :NSMicrophoneUsageDescription "YoloOSC would like to access the microphone."' $PLIST

# xcodeproj: libtensorflow dylib installation
"$OF_PATH"/addons/ofxTensorFlow2/scripts/configure_xcode.sh .
