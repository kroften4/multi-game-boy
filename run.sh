#!/usr/bin/env bash

case $1 in
    monitor)
	    pio run --target upload && pio device monitor
        ;;
    upload)
	    pio run --target upload
        ;;
    lsp)
	    pio run -t clean && pio run -t compiledb
        ;;
    *)
	    pio run
        ;;
esac
