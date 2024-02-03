# disk2png

Create a PNG image from a disk image (or basically any file).

## Usage

```bash
disk2png inFile outFile.png [blockSize level]
```

inFile
: file name of the disk image

outFile.png
: file name of the output PNG image

blockSize
: optional size of one block (column in the PNG image), default = 1024

level
: optional output level see Output level section, default = b (small)

The command line parsing is quite naive for now, if you want to set the ```level```
you have to include a ```blockSize```.

## Output level

The ```disk2png``` program can generate output PNG files with different levels. Small ```b``` and large ```B```

### ```b``` output

Every pixel in the resulting image corresponds to one byte in the source file. The pixel colour 
represents the value of the byte:

- blue pixels for bytes 0x01 to 0x19
- green pixels for classic readable text 0x20 to 0x7f
- red pixels are bytes 0x80 and above

Example for [exfat.img](examlpes%2Fexfat.img):

![Example of the small ```b```, every pixel represents a byte in the source image.](examlpes%2Fexample_small_b.png)

### ```B``` output

Every pixel represents eight bytes from the source disk image. The brightness value represents the 
number of non-zero bytes (so black for 8 zero bytes, almost white for 8 non-zero bytes)

Example for [exfat.img](examlpes%2Fexfat.img):

![Example of the large ```B```, every pixel represents eight bytes from the source image.](examlpes%2Fexample_large_B.png)

## Warning

This is a direct rewrite from a really ancient PHP script I wrote decades ago, so it is quire
suboptimal and probably unsafe :-) It may crash, do something, you don't want, use with caution.

## ToDo (someday maybe)

- "sanitize" and code clean up
- check with valgrind
- ability to create more "rows" (probably makes sense only for ```B``` output)
- borders and padding
- in-image label
- maybe multi-threading
