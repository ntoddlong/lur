#!/bin/bash

mkdir build

cmake -S . -B build

pushd build && make
popd

mv build/controller .
mv build/compile_commands.json .
