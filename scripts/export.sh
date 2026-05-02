#!/usr/bin/env bash

pushd $(dirname $0)

export $(cat ../.env)
source $IDF_EXPORT_SCRIPT

popd
