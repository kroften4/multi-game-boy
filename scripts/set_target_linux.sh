#!/usr/bin/env bash

pushd $(dirname $0)
source export.sh
popd
export _ESPIDF_SET_TARGET_LINUX=true
export PATH=/usr/bin/:$PATH
idf.py --preview set-target linux
