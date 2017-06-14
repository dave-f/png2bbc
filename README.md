# png2bbc
A tool for creating BBC Micro graphics.  Reads PNG files and outputs binary data suitable for `INCBIN` to 6502 source.  

No external dependencies are required, just a C++11 compiler.

The script is built up of 4 very simple commands:
````
MODE <0-5>
````
Selects a mode for the graphics (0-5).

---

````
IMAGE <filename>
````
Selects a source image to get the graphics data from.

---

````
COLOURS <COLOUR> <COLOUR> ..
````
Specifies the colours.  Use standard BBC Micro colours, i.e. BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN and WHITE.

Ordering is important here; colours found in the image will be mapped to pixel values corresponding to their order in this list.  For example, if RED is the second item in the list, any red found in the image will be written as pixel value 2.

---

````
CREATE-FILE <filename> FROM-DATA <x> <y> <w> <h> <number-frames>
````
Creates sprite data using the current image, colours and mode.

---

For example, to create an `8x8` Mode 5 sprite from co-ords `0,0` in `tile.png` with the default colours:
```
MODE 5
IMAGE tile.png
COLOURS BLACK RED YELLOW WHITE
CREATE-FILE player.bin FROM-DATA 0 0 8 8 1
```
