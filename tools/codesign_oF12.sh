#!/bin/bash
sudo codesign -fs "of-cert" BreathShowApp.app/Contents/Frameworks/libfmod.dylib
sudo codesign -fs "of-cert" BreathShowApp.app
sudo codesign -dv BreathShowApp.app
