# Game Engine 👾

This is my first cross platform game engine. It is written in C and uses SDL2 & Glad for graphics and input.

## Building

The building process should be simple at Windows and Linux platforms.

```bash

# Linux
./build.sh
===
OR
===
make (Makefile will be done soon)


# Windows
make (Makefile will be done soon)

```

This should successfully build the project and generate a **build/Game** or **build/Game.exe** file.
If you want to cross compile for Windows from a Linux platform, you can use the following command:

```bash

./build.sh windows
OR
make windows (Makefile will be done soon)

```

This will generate a **build/Game.exe** file that can be run on Windows.
