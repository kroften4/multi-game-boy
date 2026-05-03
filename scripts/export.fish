pushd $(dirname (status filename))

while read line
  if string match -r '^#' $line; or test -z "$line"; continue; end
  set item (string split -m 1 '=' $line)
  set -gx $item[1] $item[2]
end < ../.env

source $IDF_EXPORT_SCRIPT_FISH

popd
