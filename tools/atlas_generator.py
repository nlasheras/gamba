from struct import pack
from PIL import Image

sources = [
    ["../data/test_sheet.pcx", 16, 12],
    ["../data/character.pcx", 32, 3]
]

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
    indexes = merge_palette(image.getpalette())
    color_indexes.append(indexes)


for tuple in sources:
    read_source(tuple[0])
    tile_sizes.append(tuple[1])
    num_sprites.append(tuple[2])

print("Merged palette to " + str(len(palette)) + " colors")

# TODO assert evertyhing!
def write_tiles(file, image, indices, tile_size, num_tiles):
    px = image.load()
    count = 0
    for ty in range(0, int(image.size[1]/tile_size)):
        tile_start_y = ty*tile_size
        for tx in range(0, int(image.size[0]/tile_size)):
            tile_start_x = tx*tile_size
            num_blocks = int(tile_size / 8)
            for by in range(0, num_blocks):
                block_start_y = tile_start_y + by*8
                for bx in range(0, num_blocks):
                    block_start_x = tile_start_x + bx*8
                    for j in range(0, 8):
                        for i in range(0, 8):
                            original_pixel = px[block_start_x + i, block_start_y + j]
                            pixel = indices[original_pixel]
                            file.write(pack('b', pixel))
            count += 1
            if count == num_tiles:
                return

# TODO this should be a parameter!
file = open("graphics.bin", "wb")

# TODO write only the number of used colors
# TODO make sure color 0 is always 0,0,0 (or not use that)
for i in range(0, 256):
    if i < len(palette):
        color = palette[i]
    else:
        color = [0, 0, 0]
    color_packed = pack('h', gba_rgb8(color[0], color[1], color[2]))
    file.write(color_packed)

for i in range(0, len(images)):
    write_tiles(file, images[i], color_indexes[i], tile_sizes[i], num_sprites[i])

file.close()

