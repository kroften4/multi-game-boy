#!/usr/bin/env bash

if [[ $_ESPIDF_SET_TARGET_LINUX ]]; then
    echo "Already using PATH for linux target"
else
    export _ESPIDF_SET_TARGET_LINUX=true
    export PATH=/usr/bin/:$PATH
    echo "Set up PATH for linux target."
fi
