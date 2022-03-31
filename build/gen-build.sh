#!/usr/bin/env bash

mkdir -p Debug
mkdir -p Release
mkdir -p RDB
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ../../
cd ../Release
cmake -DCMAKE_BUILD_TYPE=Release ../../
cd ../RDB
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ../../
cd ../..
ln -s build/Debug Debug
ln -s build/Release Release
ln -s build/RDB RDB
