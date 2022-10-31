#!/bin/bash

cd server

clang-format -style=file -i sql/*.h
clang-format -style=file -i sql/*.cc

cd -

