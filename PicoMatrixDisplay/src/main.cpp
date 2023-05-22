#include "DMD_RGB.h"
#include "RGBConverter.h"
#include "CRC32.h"

//Number of panels in x and y axis
#define DISPLAYS_ACROSS 4
#define DISPLAYS_DOWN 1

// Output buffering - false
#define ENABLE_DUAL_BUFFER false

// mux pins - A, B, C... mux pins must be consecutive in ascending order
#define DMD_PIN_A 6
#define DMD_PIN_B 7
#define DMD_PIN_C 8
#define DMD_PIN_D 9
#define DMD_PIN_E 10

// put all mux pins at list
uint8_t mux_list[] = { DMD_PIN_A , DMD_PIN_B , DMD_PIN_C , DMD_PIN_D , DMD_PIN_E };

// pin OE must be one of PB0 PB1 PA6 PA7
#define DMD_PIN_nOE 15
#define DMD_PIN_SCLK 12

uint8_t custom_rgbpins[] = { 11, 0,1,2,3,4,5 }; // CLK, R0, G0, B0, R1, G1, B1
DMD_RGB <RGB32x32_S8_maxmurugan, COLOR_4BITS> dmd(mux_list, DMD_PIN_nOE, DMD_PIN_SCLK, custom_rgbpins, DISPLAYS_ACROSS, DISPLAYS_DOWN, ENABLE_DUAL_BUFFER);
RGBConverter conv;

#define WIDTH_NUMPIXELS 32 * DISPLAYS_ACROSS
#define HEIGHT_NUMPIXELS 32 * DISPLAYS_DOWN

uint16_t data_arr[WIDTH_NUMPIXELS * HEIGHT_NUMPIXELS];

void setup(void)
{
  dmd.init();
  dmd.setBrightness(50);
  Serial.begin();
  Serial.setTimeout(5000);
}

uint8_t serial_data_block(void){
  int data = -1;
  while(data != -1)
    data = Serial.read();

  return (uint8_t)data;
}

void draw_matrix_display(void){
  int n = 0;
  for (int i = 0; i < dmd.height(); i++) {
      for (int j = 0; j < dmd.width(); j++) {
        dmd.drawPixel(j,i, data_arr[n]);
        n++;
      }
  }
}

void get_matrix_frame(void){
  CRC32 crc;

  for(int n = 0; n < WIDTH_NUMPIXELS * HEIGHT_NUMPIXELS; n++){
    uint16_t data = serial_data_block() | (serial_data_block() << 8);
    data_arr[n] = data;
  }

  /*
  uint32_t crc_calc = CRC32::calculate(data_arr, sizeof(data_arr));
  // Get CRC32 from the host
  uint32_t recieved_crc = 
    (serial_data_block())       |
    (serial_data_block() << 8)  | 
    (serial_data_block() << 16) | 
    (serial_data_block() << 24);
  
  if(crc_calc != recieved_crc){
    Serial.write(255);
    delay(10);
  }
  else{
    draw_matrix_display();
  }
  */

 draw_matrix_display();
}

void loop(void) {
  uint8_t state = Serial.read(); 

  switch(state){
    case 0:
      get_matrix_frame();
    break;
    default:
    break;
  }
}