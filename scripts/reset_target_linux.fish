if set -q _ESPIDF_SET_TARGET_LINUX
    set -gx PATH $PATH[2..-1]
    set -e _ESPIDF_SET_TARGET_LINUX
    echo "Restored \$PATH. Run idf.py set-target esp32"
else
    echo "Already not using linux target: _ESPIDF_SET_TARGET_LINUX not set"
end
