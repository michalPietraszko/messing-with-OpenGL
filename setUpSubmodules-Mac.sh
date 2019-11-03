#!/bin/bash

 git submodule init
 git submodule update --recursive --progress
 cd vendor/GLFW
 cmake -DBUILD_SHARED_LIBS=ON .
 make