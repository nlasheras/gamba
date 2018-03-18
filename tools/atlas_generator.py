from struct import pack
from PIL import Image
import sys
from collections import namedtuple
from datetime import datetime

start_time = datetime.now()

Size = namedtuple('TileSize', ['w', 'h'])

sources_for_file = {"sprites.bin": [
    ["../data/sprites/assets_v1.pcx", Size(16, 16), 12],
    ["../data/sprites/character.pcx", Size(32, 32), 3]
], "background.bin": [
    ["../data/background/background.pcx", Size(8, 8), 12],
    ["../data/background/parallax.pcx", Size(32, 64), 2]
]}

images = []
tile_sizes = []
num_sprites = []
color_indexes = []
palette = []

def gba_rgb8(r, g, b):
    return ((b >> 3) << 10) | ((g >> 3) << 5) | (r >> 3)

def merge_palette(pal):
    indexes = []
    num_colors = len(pal) / 3
    for i in range(0, int(num_colors)):
        color = [pal[i*3], pal[i*3+1], pal[i*3+2]]
        try:
            idx = palette.index(color)
            indexes.append(idx)
        except ValueError:
            indexes.append(len(palette))
            palette.append(color)
    return indexes

def read_source(filename):
    image = Image.open(filename)
    images.append(image)

    pal = image.getpalette()

    # GBA colour 0 is the transparent color
    assert(pal[0] == 0 and pal[1] == 0 and pal[2] == 0)
    indexes = merge_palette(pal)

    color_indexes.append(indexes)

param = sys.argv[1]

sources = sources_for_file[param]
for tuple in sources:
    read_source(tuple[0])
    tile_sizes.append(tuple[1])
    num_sprites.append(tuple[2])

#print("Merged palette to " + str(len(palette)) + " colors")

s_block_count = 0

def write_blocks(file, pixels, indices, x, y, width, height):
    global s_block_count
    assert(width % 8 == 0)
    assert(height % 8 == 0)
    num_blocks_w = int(width / 8)
    num_blocks_h = int(height / 8)
    for by in range(0, num_blocks_h):
        start_y = y + by*8
        for bx in range(0, num_blocks_w):
            start_x = x + bx*8
            for j in range(0, 8):
                for i in range(0, 8):
                    original_pixel = pixels[start_x + i, start_y + j]
                    pixel = indices[original_pixel]
                    file.write(pack('b', pixel))
            s_block_count += 1

def write_tiles(file, image, indices, tile_width, tile_height, num_tiles):
    px = image.load()
    image_rows = int(image.size[1]/tile_height)
    image_columns = int(image.size[0]/tile_width)
    count = 0
    for ty in range(0, image_rows):
        row_y = ty * tile_height
        for tx in range(0, image_columns):
            row_x = tx * tile_width
            write_blocks(file, px, indices, row_x, row_y, tile_width, tile_height)
            count += 1
            if count == num_tiles:
                return

file = open(param, "wb")

# TODO write only the number of used colors?
pal_count = 256
for i in range(0, pal_count):
    if i < len(palette):
        color = palette[i]
    else:
        color = [0, 0, 0]

    color_packed = pack('h', gba_rgb8(color[0], color[1], color[2]))
    file.write(color_packed)

for i in range(0, len(images)):
    write_tiles(file, images[i], color_indexes[i], tile_sizes[i].w, tile_sizes[i].h, num_sprites[i])

file.close()

elapsed_time = datetime.now() - start_time

elapsed = elapsed_time.total_seconds()
elapsed_str = ("{0:.0f} ms".format(elapsed * 1000), "{0:.1f} secs".format(elapsed))[elapsed >= 1]
print("Wrote " + str(s_block_count) + " blocks from " + str(len(images)) + " images in " + elapsed_str)


