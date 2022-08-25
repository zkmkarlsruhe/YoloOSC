#! /bin/sh
#
# script to download pre-trained model and coco classes .txt
#
# requires: curl, unzip
#
# Dan Wilcox ZKM | Hertz-Lab 2022

# stop on error
set -e

# ZKM NextCloud shared folder direct link
URL=https://cloud.zkm.de/index.php/s/gfWEjyEr9X4gyY6

SRC=example_models
DEST=../..

# known model filename
ZIP=model_yolo_v4.zip

##### functions

# download from a public NextCloud shared link
# $1: root folder share link URL
# $2: filename
download() {
	local path="download?path=%2F&files=$2"
	RETCODE=$(curl -LO -w "%{http_code}" $URL/$path)
	if [ "$RETCODE" != "200" ] ; then
		echo "download failed: HTTP $RETCODE"
		rm -rf $path
		return
	fi
	mkdir -p $SRC
	mv $path $SRC/$2
}

##### main

# change to script dir
cd $(dirname "$0")

download $URL $ZIP

cd "$SRC"

# example_yolo_v4
if [ -f model_yolo_v4.zip ] ; then
	unzip model_yolo_v4.zip
	rm -rf "$DEST"/bin/data/model
	mv -v model "$DEST"/bin/data
	mv -v classes.txt "$DEST"/bin/data
fi

# cleanup
cd ../
rm -rf "$SRC"
