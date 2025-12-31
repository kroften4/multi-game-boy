# MultiGameBoy (title WIP)
DIY Multiplayer Game Pad

[Design doc](https://typst.app/project/wWPyvWHVRvhnejkS593nv4)

# Developing

## Install
1. Install ESP-IDF ([guide](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html))
1. Run
   ```bash
   . $HOME/esp/esp-idf/export.sh
   ``` 
   or, if you use fish
   ```bash
   . $HOME/esp/esp-idf/export.fish
   ``` 
1. Install `esp-clang`
   ```bash
   idf_tools.py install esp-clang
   ```
1. Run
   ```bash
   idf.py build
   ```
   It will generate `build/compile_commands.json` which is used by clangd LSP
   server

## Run
Before doing anything, export ESP-IDF tools:
```bash
. $HOME/esp/esp-idf/export.sh
``` 
or, if you use fish
```bash
. $HOME/esp/esp-idf/export.fish
``` 

- Compile the project
    ```bash
    idf.py build
    ```
- Clean `build/` directory
    ```bash
    idf.py clean
    ```
- Flash the chip (also builds if necessary)
    ```bash
    idf.py flash
    ```
- USB monitor (`C-]` to quit)
    ```bash
    idf.py monitor
    ```
- Flash and monitor
    ```bash
    idf.py flash monitor
    ```

## Configuration
### SDK config
`sdkconfig` is a local auto-generated file which contains the full SDK
configuration. To edit, run
```bash
idf.py menuconfig
```
The changes will be saved locally

`sdkconfig.defaults*` files specify default options for `sdkconfig`. If changed,
remove `sdkconfig` file and rebuild to apply changes.

`sdkconfig.defaults.<target>`, where `<target>` is from `idf.py set-target` (e.
g. esp32) apply when building for the specified target and override
`sdkconfig.defaults` settings

### Miscellaneous
- Create component
  ```bash
  idf.py create-componenet -C components my_component_name
  ```

- Set target
  ```bash
  idf.py set-target esp32
  ```
