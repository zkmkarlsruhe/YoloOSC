#! /bin/bash
#
# build YoloOSC macOS release as a notarized DMG
#
# Dan Wilcox ZKM | Hertz-Lab 2022

# stop on errors
set -e

# codesigning identity
IDENTITY="ZKM | Zentrum fuer Kunst und Medientechnologie Karlsruhe"

### main

cd $(dirname $0)/../
make dist-clean dist-clobber
make app dist-dmg mac.codesign.identity="$IDENTITY"
make dist-clean
