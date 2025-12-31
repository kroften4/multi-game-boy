# this script should be sourced, not executed

. $HOME/esp/esp-idf/export.fish

# https://github.com/espressif/clang-tidy-runner/issues/32
# this fixes everything
set IDF_TOOLCHAIN clang
