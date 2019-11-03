#!/bin/bash

./vendor/premake/mac/premake5 --file=premake5-Mac.lua gmake2
cd build
make 