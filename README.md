opbb10
======

Thank you for downloading Hookflash's Open Peer BlackBerry 10 SDK (QNX based) - Beta.

These are instructions for building the OpenPeer shared library for Blackberry. This assumes Mac (and probably works on Linux) but issues may come up on Windows.

## Download the NDK ##
- The IDE, compliers, etc for Blackberry can be downloaded from http://developer.blackberry.com/native/download/.
- The simulator can also be downloaded but we have had poor success with it. Far better to have a real device.

## Clone Libraries ##
Currently you need two projects "op" and "opbb10". "op" is using the 20121212-json-provisioning (temporarily) and "opbb10" is using master. Both must be located at the same directory level. Clone the repos as follows:
- git clone git@github.com:openpeer/opbb10.git
- git clone git@github.com:openpeer/op.git -b 20121212-json-provisioning

## Building Boost ##
There is a special version of boost that was built by Blackberry. It is found in the OP github repository: https://github.com/openpeer/opbb10.
Building is done from the command line. Instructions for building the library are found on the github page: https://github.com/blackberry/Boost. Search for "Build Instructions". Or, try the following (assuming Mac and the NDK is installed in Applications)

    source /Applications/bbndk/bbndk-env.sh
    cd opbb10/hookflash-libs/boost/rim-build
    ./build.sh install static

The HF libraries (core, stack, service) will be looking for this in the op/hookflash-libs/bbBoost directory, which should exist as symbolic links if you cloned the repo from github.
If they do not for any reason, you will need to create a symbolic link in a shell and linking using...
    
    ln -s opbb10/hookflash-libs/boost/ op/hookflash-libs/bbBoost

## PCRE - the regular expression library for BB ##
Boost regular expressions crash on the Blackberry. Instead of trying to fix the crash the PCRE regular expression library was added ot the opbb10 branch. This needs to be built.
Type the following commands (assuming Mac and the NDK is installed in Applications)

    source /Applications/bbndk/bbndk-env.sh
    cd opbb10/hookflash-libs/pcre/PCRE/qnx
    make

Note that the library needs to have a symbolic link too just like bbBoost added (unless cloned from github in which case it should already exist).
    
    ln -s opbb10/hookflash-libs/pcre/ op/hookflash-libs/pcre

## Adding libraries to the .BAR file ##
The .bar file is a zip file (renamed to .bar) that contains the files and directory structure of the application. Note that libraries are added via the .pro file (Cascades does things differently from native BB). But adding it to the pro does not add it to the BAR file.
Shared objects (aka libraries) are added via the bar-descriptor.xml file. Opening it in the IDE will bring up a UI. In the assets tab add the .so.

## Launching IDE ##
Go to Launch Pad -> bbndk (other) -> qde
Load the workspace: op/hookflash-core/projects/bb






License:

 Copyright (c) 2013, SMB Phone Inc.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
