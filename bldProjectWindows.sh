#!/bin/bash

vendor/premake/windows/premake5 --file=premake5-Windows.lua vs2019
read -p "Press any key to continue... " -n1 -s