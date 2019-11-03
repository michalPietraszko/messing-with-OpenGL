#!/bin/bash

./premake5 --file=premake5-Mac.lua gmake2
cd build
make 