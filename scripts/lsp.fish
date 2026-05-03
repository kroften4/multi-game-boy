#!/usr/bin/env bash

# Generate the compilation databases for clangd LSP


pushd $(dirname (status filename))
echo "###### SOURCING VENV ######"
source export.fish

cd ../
echo "###### BUILDING MAIN ######"
IDF_TOOLCHAIN=clang idf.py -B build.clang build

echo "###### BUILDING TEST ######"
cd test
IDF_TOOLCHAIN=clang idf.py -B build.clang build

popd
