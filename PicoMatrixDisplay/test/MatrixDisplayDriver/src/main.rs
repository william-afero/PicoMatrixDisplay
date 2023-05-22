
struct SerialMatrixDisplay{
    name: String, 
    data_arr: Vec<u8>, 
    size: u32,
    length: u16, 
    width: u16
}

fn serialize_u16(value: u16) -> [u8; 2] {
    let byte1 = (value >> 8) as u8;
    let byte2 = value as u8;

    [byte1, byte2]
}

impl SerialMatrixDisplay{

    fn set_pixel(&mut self, x: u16, y: u16, r: u8, g: u8, b: u8){
        let r5 = (r as u16 >> 3) << 11;
        let g6 = (g as u16 >> 2) << 5;
        let b5 = b as u16 >> 3;
        let output: u16 = r5 | g6 | b5;

        let output_bytearray = serialize_u16(output);
        self.data_arr[(y as usize * self.width as usize + x as usize) * 2] = output_bytearray[0];
        self.data_arr[(y as usize * self.width as usize + x as usize) * 2 + 1] = output_bytearray[1];
    }

    fn show(&mut self){

    }
}

fn main() {
    
}
