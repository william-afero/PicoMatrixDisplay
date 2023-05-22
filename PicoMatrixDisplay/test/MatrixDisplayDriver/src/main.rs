use serialport::{self, SerialPort};
use std::time::Duration;
struct SerialMatrixDisplay{
    data_arr: Vec<u8>, 
    size: u32,
    length: u16, 
    width: u16, 
    serial_port: Box<dyn SerialPort>
}

fn serialize_u16(value: u16) -> [u8; 2] {
    let byte1 = (value >> 8) as u8;
    let byte2 = value as u8;

    [byte1, byte2]
}

impl SerialMatrixDisplay{
    fn new(length: u16, width: u16, port: &str) -> Self {
        
        let new_size = 2 * width * length;
        let arr = vec![0; new_size as usize];
    
        
        let port = serialport::new(port, 2000000000)
            .timeout(Duration::from_millis(1000))
            .open()
            .expect("Failed to open port");

        SerialMatrixDisplay {
            data_arr :arr,
            size: new_size as u32,
            length,
            width,
            serial_port: port
        }
    }

    fn set_pixel(&mut self, x: u16, y: u16, r: u8, g: u8, b: u8){
        let r5 = (r as u16 >> 3) << 11;
        let g6 = (g as u16 >> 2) << 5;
        let b5 = b as u16 >> 3;
        let output: u16 = r5 | g6 | b5;

        let output_bytearray = serialize_u16(output);

        let pos = y as usize * self.width as usize + x as usize * 2;
        self.data_arr[pos] = output_bytearray[0];
        self.data_arr[pos + 1] = output_bytearray[1];
    }

    fn show(&mut self){
        let zero = [0];
        self.serial_port.write(&zero);
        self.serial_port.write(&self.data_arr);
    }
}

fn main() {
    let port = "/dev/ttyACM1";
    let mut display = SerialMatrixDisplay::new(32, 128, port);

    println!("{}", display.size);
    display.set_pixel(0, 0, 100, 100, 100);
    display.show();
}
