# png2bbc
A tool for creating BBC Micro graphics.  Reads PNG files and outputs binary data suitable for `INCBIN` to 6502 source.  

No external dependencies are required, just a C++11 compiler.  Tested on gcc version 5.4.0.

The program reads in a script which defines what output files to create and how to create them.  With the `-l` switch, `png2bbc` will just list these output files without making them, and with the `-i` switch it will list the source png files referenced from the script.  These options are so you can easily include dependencies in your makefile.

The script is built up of very simple commands, which are detailed below.  Any lines starting with a `;` are considered comments.

---

````
IMAGE <filename>
````
Selects a source image to get the graphics data from.

---

````
MODE <number>
````
Selects a mode for the graphics (0,1,2,4 or 5).

---

````
COLOURS <colour> <colour> ..
````
Specifies the colours.  Use standard BBC Micro colours, i.e. BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN and WHITE.

Ordering is important here; colours found in the image will be mapped to pixel values corresponding to their order in this list.  For example, if RED is the second item in the list, any red found in the image will be written as pixel value 2 (actually 1, since it's zero-based).

---

````
CUSTOM-COLOUR <RGB> <number>
````
Overrides any colour.  This is useful if you have special RGB values in your source image you want to map to specific pixel values (e.g. using grey for masks).  Custom colours take precedence over normal colours and are reset on the next `COLOURS` command.

---

````
CUSTOM-NULA-COLOURS <x> <y> <n> <step> [start-index] [FILE <filename>]
````
Creates a NuLA palette of `n` colours from pixels in the image, optionally writing out to `filename`.  NuLA colours are stored internally as "custom" colours, and the pixels in the source image should be stored horizontally.  The `step` parameter specifies the size in pixels of each palette entry, so you can work with larger palettes on the source PNG. The optional parameter `start-index` starts the indexing from a specific value.

---

````
CREATE-FILE <filename> FROM-DATA <x> <y> <w> <h> <number-frames> [DATA-ORDER <LINE|PRESHIFTED|BLOCK>]
````
Creates sprite data using the current image, colours and mode.  There is a choice of data output formats:

- Line. This is the default. Pixels are written out just as straight scanlines for normal sprite routines.
- Preshifted. Pixels are written out as above, but preshifted copies are made which will insert the correct amount of empty (black) pixels for the mode (e.g. in mode 5, 4 copies of the sprite are written out - shifted by 0,1,2 and 3 pixels).
- Block. Pixels are packed into bytes for the required mode, and written out in blocks of 8 down, then another block of 8.  This mimics the BBC Micro's character-based screen layout, so is handy for writing out graphics which are always drawn on character boundaries (e.g. tiles), or creating data which will be directly loaded to the screen.

Instead of `CREATE-FILE` you can use `APPEND-FILE` which has the same syntax but appends data to a file rather than create a new one. This is useful if you want to keep graphics data together which are made from separate regions in the source image.

---

For example, to create one `8x8` Mode 5 sprite from co-ords `0,0` in `tile.png` with the default colours:
```
MODE 5
IMAGE tile.png
COLOURS BLACK RED YELLOW WHITE
CREATE-FILE player.bin FROM-DATA 0 0 8 8 1 DATA-ORDER LINE
```
