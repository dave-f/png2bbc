# png2bbc
A tool for creating BBC Micro graphics.  Reads PNG files and outputs binary data suitable for `INCBIN` to 6502 source.  

No external dependencies are required, just a C++11 compiler.

The program reads in a script which defines how the graphics are created.  This script is built up of 4 very simple commands, which are detailed below.

---

````
MODE <NUMBER>
````
Selects a mode for the graphics (0,1,2,4 or 5).

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

Ordering is important here; colours found in the image will be mapped to pixel values corresponding to their order in this list.  For example, if RED is the second item in the list, any red found in the image will be written as pixel value 2 (actually 1, since it's  zero-based).

---

````
CREATE-FILE <filename> FROM-DATA <x> <y> <w> <h> <number-frames> [DATA-ORDER <BLOCK|LINE|PRESHIFTED>]
````
Creates sprite data using the current image, colours and mode.  There is a choice of data output formats:

- Block. Pixels are written out in blocks of 8 down, then another block of 8.  This mimics the BBC Micro's character-based screen layout, so is handy for writing out graphics which are always drawn on character boundaries (e.g. tiles)
- Line. Pixels are written out just as straight scanlines for normal sprite routines.
- Preshifted. Pixels are written out as above, but preshifted copies are made which will insert the correct amount of empty pixels for the mode (e.g. in mode 5, 4 copies of the sprite are written out).  Files are output with a number on the end to indicate the amount the sprite is shifted by.

---

For example, to create an `8x8` Mode 5 sprite from co-ords `0,0` in `tile.png` with the default colours:
```
MODE 5
IMAGE tile.png
COLOURS BLACK RED YELLOW WHITE
CREATE-FILE player.bin FROM-DATA 0 0 8 8 1
```
