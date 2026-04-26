# this script should be sourced, not executed

EXPORT_SCRIPT=$1
if [[ -z $1 ]]; then
    EXPORT_SCRIPT=$HOME/esp/esp-idf/export.sh
fi

. $EXPORT_SCRIPT

# https://github.com/espressif/clang-tidy-runner/issues/32
# this fixes everything
export IDF_TOOLCHAIN=clang 
