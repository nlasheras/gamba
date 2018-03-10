from struct import pack
from PIL import Image

image = Image.open("../data/test_sheet.pcx")
pal = image.getpalette()
px = image.load()

def gba_rgb8(r, g, b):
    return ((b >> 3) << 10) | ((g >> 3) << 5) | (r >> 3)

file = open("graphics.bin", "wb")

# TODO make sure color 0 is always 0,0,0 (or not use that)
for i in range(0, 256):
    if i == 1 or i == 4:
        color = pack('h', gba_rgb8(255, 0, 0))
    elif i == 2 or i == 5:
        color = pack('h', gba_rgb8(0, 255, 0))
    elif i == 3 or i == 6:
        color = pack('h', gba_rgb8(0, 0, 255))
    else:
        color = pack('h', gba_rgb8(255, 0, 255))
    color = pack('h', gba_rgb8(pal[i*3], pal[i*3+1], pal[i*3+2]))
    file.write(color)

for ty in range(0, int(image.size[1]/16)):
    starty = ty*16;
    for tx in range(0, int(image.size[0]/16)):
        startx = tx*16;
        for k in range(0, 4):
            for j in range(0, 8):
                for i in range(0, 8):
                    pixel = px[startx + i + int(k%2)*8, starty + j + int(k/2)*8]
                    file.write(pack('b', pixel))

file.close()