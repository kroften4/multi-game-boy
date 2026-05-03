# MultiGameBoy (title WIP)
DIY Multiplayer Game Pad

[Design doc](https://typst.app/project/wWPyvWHVRvhnejkS593nv4)

# Developing

## Install prerequisites & Configure
TODO: clean build.clang/'s

1. Install ESP-IDF
   ([guide](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html))
   (Using `eim` tool is recommended)
1. To use ESP-IDF's tools, source the export script, usually located at
   `~/.espressif/tools/activate*`

   Alternatively, you can configure a `.env` file (see the next step).

The following steps are required only if you plan to use clangd LSP server.
1. Create a `.env` file and configure it with the ESP-IDF export script,
   according to `example.env`
1. If you are using neovim with mason, it can override the `clangd` LSP server
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
   This will install a specific clang version suitable for ESP-IDF.
1. Run
   ```sh
   source scripts/lsp.sh
   ```
   or 
   ```fish
   source scripts/lsp.fish
   ```
   This will generate `build.clang/compile_commands.json` which is used by
   clangd LSP server.

## Build & Run
Before doing anything, export ESP-IDF tools, for example:
```sh
source scripts/export.sh
``` 
or, if you use fish:
```sh
source scripts/export.fish
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

## Test
### Register
To create tests for a component, create a `test` sub-directory in it. Place a
CMakeLists.txt file that lists `unity` and the component you want to test as
dependencies. For example, for `components/my-component`:
```cmake
# components/my-component/test/CMakeLists.txt
idf_component_register(SRC_DIRS "."
                       INCLUDE_DIRS "."
                       REQUIRES unity my-component)
```
The `test` directory is a sub-project for testing all your components. 
Register the components you want to test in `test/CMakeLists.txt`:
```cmake
# test/CMakeLists.txt
# ...
# Set the components to include the tests for.
set(TEST_COMPONENTS "my-component" CACHE STRING "List of components to test")
# ...
```
### Build
```sh
cd test
idf.py build
```

### Write
TODO: develop a custom test runner that is not shitty

As per the official documentation, use ESP-IDF's TEST_CASE().

### Run
TODO: make this separate from test

- On host (possible if no unportable dependencies used):

  Set target
  ```sh
  cd test
  source ../scripts/set_target_linux.sh # (or .fish) 
  ```
  This workaround will export host's binaries (`as`, `gcc`, `ld`, etc.) that are
  needed for the target. Then proceed building/debugging/etc. as normal.
  ```sh
  cd test
  idf.py build
  idf.py gdbtui
  idf.py monitor
  ```
  To restore the previous `$PATH`, run:
  ```sh
  source scripts/reset_target_linux.sh # (or .fish)
  ```

- On device
  ```sh
  cd test
  idf.py set-target esp32
  idf.py flash monitor
  ```

## Configuration
### SDK config
`sdkconfig` is a local auto-generated file which contains the full SDK
configuration. To edit, run
```sh
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
  ```sh
  idf.py create-componenet -C components my_component_name
  ```
  It is important to put all components in `components/` directory, otherwise
  they are not recognized

- Set target
  ```sh
  idf.py set-target esp32
  ```
