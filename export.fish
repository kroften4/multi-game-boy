# this script should be sourced, not executed

set EXPORT_SCRIPT $argv[1]
if test -z "$argv[1]"
    set EXPORT_SCRIPT $HOME/esp/esp-idf/export.fish
end

source $EXPORT_SCRIPT

# https://github.com/espressif/clang-tidy-runner/issues/32
# this fixes everything
set -x IDF_TOOLCHAIN clang
