import struct
import sys

class Voxel:
    def __init__(self, x, y, z, r, g, b, a):
        self.x, self.y, self.z = x, y, z
        self.r, self.g, self.b, self.a = r, g, b, a

def txt_to_vsb(txt_file, vsb_file):
    voxels = []
    with open(txt_file, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            parts = line.split()
            if len(parts) != 4:
                continue

            x, y, z = map(int, parts[:3])
            hexcolor = parts[3]
            if len(hexcolor) < 6:
                continue

            r = int(hexcolor[0:2], 16)
            g = int(hexcolor[2:4], 16)
            b = int(hexcolor[4:6], 16)
            try:
                a = int(hexcolor[6:8], 16)
            except:
                a = int("FF", 16)

            voxels.append(Voxel(x, y, z, r, g, b, a))

    with open(vsb_file, "wb") as f:
        f.write(b"VSB\0") # Заголовок

        # Данные
        for v in voxels:
            f.write(struct.pack("<iiiBBBB", v.x, v.y, v.z, v.r, v.g, v.b, v.a))

    print(f"Сохранено {len(voxels)} вокселей в {vsb_file}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Использование: {sys.argv[0]} input.txt output")
    else:
        txt_to_vsb(sys.argv[1], sys.argv[2])
