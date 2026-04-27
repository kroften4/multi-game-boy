:!sort -r
- 100 store data in indexed palette (or some other technique) and convert to 565 in partial buffer
- 100 provide operations like fill_rect fill_screen etc.
- 090 make the driver more configurable (display size, bgr, madctl, spi/parallel, etc.)
- 030 write a readme for patched driver
- 030 lsp auto completion system includes do not use angle braces
- 020 adjust driver init sequence timings
- 010 wire backlight to gpio and control it on init
- 010 use Kconfig instead of sdkconfig.defaults?
- 010 move config.h to sdkconfig
