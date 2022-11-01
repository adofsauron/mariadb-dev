#!/bin/bash

cd server
mkdir -p build
cd build

rm ./* -rf

cmake3 .. 

make -j`nproc`

make install

cd ../..
