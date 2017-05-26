# png2bbc
A general tool for creating BBC Micro graphics from a PNG input.

Reads PNG files and outputs binary data suitable for `INCBIN` to 6502 source.  Script will be text or lua-based, specifying where to get the data from, eg:

```
WITH-OUTPUT "player.bin" USING-MODE 5 CREATE-FROM X=0 Y=0 W=8 H=8 FRAMES=8
WITH-OUTPUT "alien1.bin" USING-MODE 5 CREATE-FROM X=0 Y=8 W=8 H=8 FRAMES=2 
WITH-OUTPUT "alien2.bin" USING-MODE 5 CREATE-FROM X=16 Y=8 W=8 H=8 FRAMES=2
```
