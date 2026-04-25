# MultiGameBoy (title WIP)
DIY Multiplayer Game Pad

[Design doc](https://typst.app/project/wWPyvWHVRvhnejkS593nv4)

# Developing

## Install
1. Install ESP-IDF ([guide](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html))
1. Run
   ```bash
   . export.sh [path/to/esp/export/script]
   ``` 
   or, if you use fish
   ```bash
   . export.fish [path/to/esp/export/script]
   ``` 
   This will export the required ESP-IDF's tools and environment variables
   
   If you are using neovim with mason, it can override the `clangd` LSP server
   executable. To use the correct one, configure mason to *append* instead of
   *prepend* it's binaries to PATH:
   ```lua
   require("mason").setup({
      -- put system's binaries first
      PATH = "append",
   })
   ```
   You can check if neovim is using the correct `clangd` executable by running
   `:!which clangd`
1. Install `esp-clang`
   ```bash
   idf_tools.py install esp-clang
   ```
   This will install a specific clang version suitable for ESP-IDF. Make sure to
   reapply the prevoius export.
1. Run
   ```bash
   idf.py build
   ```
   This will generate `build/compile_commands.json` which is used by clangd LSP
   server

## Build & Run
Before doing anything, export ESP-IDF tools:
```bash
. export.sh
``` 
or, if you use fish
```bash
. export.fish
``` 

- Compile the project
    ```bash
    idf.py build
    ```
- Clean build outputs
    ```bash
    idf.py clean
    ```
    ```bash
    idf.py fullclean
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

`sdkconfig.defaults.<target>` where `<target>` is from `idf.py set-target` (e.
g. `esp32`) apply when building for the specified target and override
`sdkconfig.defaults` settings

### Miscellaneous
- Create component
  ```bash
  idf.py create-componenet -C components my_component_name
  ```
  It is important to put all components in `components/` directory, otherwise
  they are not recognized

- Set target
  ```bash
  idf.py set-target esp32
  ```
