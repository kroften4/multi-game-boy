pushd $(dirname (status filename))
source export.fish
popd
set -gx _ESPIDF_SET_TARGET_LINUX true
set -gx PATH /usr/bin/ $PATH
idf.py --preview set-target linux
