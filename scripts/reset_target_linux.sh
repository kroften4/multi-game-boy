#!/usr/bin/env bash

if [[ $_ESPIDF_SET_TARGET_LINUX ]]; then
    export PATH="${PATH#*[:]}"
    unset _ESP_IDF_SET_TARGET_LINUX
    echo "Restored \$PATH. Run idf.py set-target esp32"
else
    echo "Already not using linux target: _ESPIDF_SET_TARGET_LINUX not set"
fi
