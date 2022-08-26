YoloOSC
=======

![screenshot](mdia/screenshot.png)

Yolo live object identification to OSC.
 
This code base has been developed by [ZKM | Hertz-Lab](https://zkm.de/en/about-the-zkm/organization/hertz-lab) as part of the project [»The Intelligent Museum«](#the-intelligent-museum). 

Please raise issues, ask questions, throw in ideas or submit code, as this repository is intended to be an open platform to collaboratively improve language identification.

Copyright (c) 2022 ZKM | Karlsruhe.   
Copyright (c) 2022 Dan Wilcox.

GNU GPL Version 3.

ofxYolo.h from [ofxTensorFlow2](https://github.com/zkmkarlsruhe/ofxTensorFlow2) example_style_transfer_arbitrary under a
BSD Simplified License: https://github.com/zkmkarlsruhe/ofxTensorFlow2  
[Jonathhhan](https://github.com/Jonathhhan) & members of the ZKM | Hertz-Lab 2022

Dependencies
------------

* [openFrameworks](https://openframeworks.cc/download/)
* openFrameworks addons:
  - ofxOSC (included with oF)
  - [ofxTensorFlow2](https://github.com/zkmkarlsruhe/ofxTensorFlow2)
  - [ofxSyphon](https://github.com/astellato/ofxSyphon) (macOS)
* [CLI11 parser](https://github.com/CLIUtils/CLI11) (included in `src`)
* pre-trained YOLO model and COCO classes txt file (download separately)

Structure
---------

* `src/`: contains the openFrameworks C++ code
* `bin/data/model`: contains the SavedModels trained with TensorFlow2
* `bin/data/classes.txt`: COCO identification class strings, one per line

Installation & Build
--------------------

Overview:

1. Download the required TF2 model and COCO classes file
2. Follow the steps in the ofxTensorFlow2 "Installation & Build" section for you platform
3. Generate the project files for this folder using the OF ProjectGenerator
4. Build for your platform

### Download Pre-Trained Model

A provide pre-trained YOLO model can be downloaded as a `model_yolo_v4.zip` file from the public shared link here:

<https://cloud.zkm.de/index.php/s/gfWEjyEr9X4gyY6>

Unzip the file and place the `model` directory and `classes.txt` file into the YoloOSC `bin/data` directory.

To make this quick, a script is provided to download and install the model (requires a Unix shell, curl, and unzip):

```shell
./scripts/download_model.sh
```

### Generating Project Files

Project files are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the openFrameworks distribution.

To (re)generate project files for an existing project:

1. Click the "Import" button in the ProjectGenerator
2. Navigate to the project's parent folder ie. "apps/myApps", select the base folder for the example project ie. "LanguageIdentifier", and click the Open button
3. Click the "Update" button

If everything went Ok, you should now be able to open the generated project and build/run the example.

### macOS

On macOS, a couple of additional manual steps are required to use ofxTensorflow2:

1. Enable C++14 (minimum) or C++17 in openFrameworks (only once, Xcode + Makefile)
2. Invoke `macos_install_libs.sh` in the Xcode project's Run Script build phases (after every project regeneration, Xcode only)

See the detailed steps in the [ofxTensorflow2 readme](https://github.com/zkmkarlsruhe/ofxTensorFlow2#macos).

For an Xcode build, open the Xcode project, select the "YoloOSC Debug" scheme, and hit "Run".

For a Makefile build, build and run on the terminal:

```shell
make ReleaseTF2
make RunRelease
```
### Linux

Build and run on the terminal:

```shell
make Release
make RunReleaseTF2
```

Usage
-----

The openFrameworks application runs the Yolo v4 object detection model using live camera input. The detected objects are sent out using OSC (Open Sound Control) messages.

### Key Commands

* `m`: toggle horz mirroring
* `n`: toggle vert mirroing

### OSC Communication

#### Sending

By default, sends to:
* address: `localhost` ie. `127.0.0.1`
* port: `7765`

Message specification:

* **/begin _num_**: detection frame begin
  - num: int, number of objects detected
* **/object _index_ _ident_ _x_ _y_ _w_ _h_ _confidence_**: detected object
  - index: int, object index, 0 to num-1
  - ident: string, identified object class, ie. "dog", "cat", "car", ...
  - x, y, w, & h: normalized bounding box 0-1 (top left corner)
  - confidence: detection confidence %, 0-1
* **/end**: detection frame end

#### Commandline Options

Additional run time settings are available via commandline options as shown via the `--help` flag output:

~~~
% bin/YoloOSC --help
yolo live object identification to osc
Usage: YoloOSC [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -a,--address TEXT           OSC send host address or name, default localhost
  -p,--port INT               OSC send port, default 7765
  -l,--list                   list input devices and exit
  -d,--dev INT                input device number, default 0
  -r,--rate INT               desired input framerate, default 30
  -s,--size TEXT              desired input size, default 640x480
  --syphon                    start streaming with Syphon (macOS only)
  -v,--verbose                verbose printing
  --version                   print version and exit
~~~

For example, to send OSC to a different address and port, use the `-a` and `-p` options:

~~~
bin/YoloOSC -a 192.168.0.101 -p 7777
~~~

#### macOS

For macOS, the application binary can be invoked from within the .app bundle to pass commandline arguments:

~~~
bin/YoloOSC.app/Contents/MacOS/YoloOSC -h
~~~

or via the system `open` command:

~~~
open bin/YoloOSC.app --device 1
~~~

_Note: `open` will launch the app without a Terminal window, so console output cannot be read._

This approach can also be wrapped up into a shell alias to be added to the account's `~/.bash_profile` or `~/.zshrc` file:

~~~
alias yoloosc="/Applications/YoloOSC.app/Contents/MacOS/YoloOSC"
~~~

Reload the shell and application can now be invoked via:

~~~
yoloosc -v --device 1
~~~

Another option is to use a wrapper script, such as the `yoloosc` script included with this repo:

~~~
./yoloosc -v --device 1
~~~

_Note: The `yoloosc` script uses the release build "YoloOSC" .app naming. If you are testing with the debug build, edit the `APP` variable name to "YoloOSCDebug"._

#### Syphon (macOS)

On macOS, the camera input frames can be received by client applications which use [Syphon](http://syphon.v002.info):

>Syphon is an open source Mac OS X technology that allows applications to share frames - full frame rate video or stills - with one another in realtime.

Syphon streaming is off by default, use the commandline `--syphon` flag to enable (see Commandline Options macOS section):

~~~
open bin/YoloOSC.app --device 1
~~~

When streaming, YoloOSC publishes the camera frame using the following naming format:
* server name: "frame"
* app name: "YoloOSC" (Release) or "YoloOSCDebug" (Debug)

For example, if receiving from YoloOSC (Release) in another openFrameworks application using a ofxSyphon client:

```c++
// ofApp.h
ofxSyphonClient client;

// ofApp.cpp
void ofApp::setup() {
    ...
    client.setup();
    client.set("frame", "YoloOSC");
}

void ofApp::draw() {
    ofSetColor(255);
    client.draw(0, 0);
}
```

Develop
-------

### Release steps

1. Update changelog
2. Update app version in Xcode project and ofApp.h define
3. Tag version commit, ala "0.3.0"
4. Push commit and tags to server:
~~~
git commit push
git commit push --tags
~~~

The Intelligent Museum
----------------------

An artistic-curatorial field of experimentation for deep learning and visitor participation

The [ZKM | Center for Art and Media](https://zkm.de/en) and the [Deutsches Museum Nuremberg](https://www.deutsches-museum.de/en/nuernberg/information/) cooperate with the goal of implementing an AI-supported exhibition. Together with researchers and international artists, new AI-based works of art will be realized during the next four years (2020-2023).  They will be embedded in the AI-supported exhibition in both houses. The Project „The Intelligent Museum” is funded by the Digital Culture Programme of the [Kulturstiftung des Bundes](https://www.kulturstiftung-des-bundes.de/en) (German Federal Cultural Foundation) and funded by the [Beauftragte der Bundesregierung für Kultur und Medien](https://www.bundesregierung.de/breg-de/bundesregierung/staatsministerin-fuer-kultur-und-medien) (Federal Government Commissioner for Culture and the Media).

As part of the project, digital curating will be critically examined using various approaches of digital art. Experimenting with new digital aesthetics and forms of expression enables new museum experiences and thus new ways of museum communication and visitor participation. The museum is transformed to a place of experience and critical exchange.

![Logo](media/Logo_ZKM_DMN_KSB.png)
