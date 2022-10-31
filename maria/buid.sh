#!/bin/bash

cd server/build

rm ./* -rf

cmake .. 

make -j`nproc`

make install

cd ../..
