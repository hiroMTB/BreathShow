#!/bin/bash
sudo codesign -fs "of-cert" BreathShowApp.app
sudo codesign -dv BreathShowApp.app
zip BreathShowApp.app_r123.zip BreathShowApp.app