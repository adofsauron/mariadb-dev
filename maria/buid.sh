#!/bin/bash

cd server/build

rm ./* -rf

cmake .. -CMAKE_BUILD_TYPE=debug

make -j`nproc`

make install

cd ../..
