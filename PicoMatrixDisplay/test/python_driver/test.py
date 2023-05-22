import serial 
import struct 

def to_rgb565(red, green, blue):
    # Convert RGB components to RGB565 format
    red = (red >> 3) & 0x1F
    green = (green >> 2) & 0x3F
    blue = (blue >> 3) & 0x1F

    rgb565 = (red << 11) | (green << 5) | blue  # Combine RGB components

    return rgb565

class MatrixDisplay:
    def __init__(self, width, height, port):
        self.port = serial.Serial(port)
        self.width = width
        self.height = height

        self.bytearray = bytearray(width * height * 2)

    def set_pixel(self, x, y, r, g, b):
        rgb565 = to_rgb565(r, g, b)
        byte_array = struct.pack('>H', rgb565)

        pos = (y * self.width + x) * 2
        self.bytearray[pos] = byte_array[0]
        self.bytearray[pos + 1] = byte_array[1]

    
    def show(self):
        self.port.write(self.bytearray)


display = MatrixDisplay(128, 32, "/dev/ttyACM1")
display.set_pixel(0, 12, 100, 100, 100)
display.show()