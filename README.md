# Snake Game
<img src="https://github.com/user-attachments/assets/5753da13-3667-4302-8b62-0eb25e8f28d5" width="400" alt="Snake gameplay">

This is the classic Snake game recreated by me in C++ with SFML.

## Download

- Note: Only available for Windows.
- Download the .zip file from the [Releases](../../releases) page.
- Extract the zip and run main.exe inside the Snake.Game folder.

## Building from source

You only need this if you want to compile the game yourself. Otherwise, use the release zip above.

**Requirements (Windows):**
- [Visual Studio](https://visualstudio.microsoft.com/) with the "Desktop development with C++" workload
- [CMake](https://cmake.org/download/) 3.28 or newer
- [Git](https://git-scm.com/) (CMake uses it to download SFML automatically)

**Steps:**

```
git clone https://github.com/WilliamLS-dev/cpp-snake.git
cd cpp-snake\"Snake Game"
cmake -B build
cmake --build build --config Release
```

The first build takes a few minutes because it downloads and compiles SFML. When it finishes, the game is at `build/bin/Release/main.exe`.

The font and sound files need to be in the same folder as `main.exe` to run.

## Controls

- WASD to move.
- R to restart.
- ESC to quit.

 ## Credits

- Made with [SFML](https://www.sfml-dev.org/)
- Font: Pixel Digivolve by Pixel Sagas (dafont.com)
- Sound effects made with Bfxr

## License

Code is MIT licensed (see `LICENSE`). The font is not covered by this license.
