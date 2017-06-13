# png2bbc
A tool for creating BBC Micro graphics from a PNG input.  Reads PNG files and outputs binary data suitable for `INCBIN` to 6502 source.  

No external dependencies are required, just a C++11 compiler.

The script is built up of very simple commands:

```
MODE 5
COLOURS WHITE BLUE RED GREEN
IMAGE input.png
CREATE-FILE "player.bin" FROM-DATA 0 0 8 8 8
CREATE-FILE "alien1.bin" FROM-DATA 0 8 8 8 2 
CREATE-FILE "alien2.bin" FROM-DATA 16 8 8 8 2
```
