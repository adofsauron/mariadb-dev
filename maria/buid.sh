#!/bin/bash

cd server/build

rm ./* -rf

cmake3 .. 

make -j`nproc`

make install

cd ../..
