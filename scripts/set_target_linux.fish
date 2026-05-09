if set -q _ESPIDF_SET_TARGET_LINUX
    echo "Already using PATH for linux target"
else
    set -gx _ESPIDF_SET_TARGET_LINUX true
    set -gx PATH /usr/bin/ $PATH
    echo "Set up PATH for linux target."
end
