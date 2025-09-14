#include "waveshare.h"
#include "icon.h"

void LCD_Write_Command(uint8_t data) {	
  digitalWrite(PIN_DC, LOW);
  digitalWrite(PIN_CS1, LOW);
  SPI.transfer(data);
}

void LCD_Write_Data(uint8_t data) {
  digitalWrite(PIN_DC, HIGH);
  digitalWrite(PIN_CS1, LOW);
  SPI.transfer(data);
  digitalWrite(PIN_CS1, HIGH);
}

void initLCD(void) {

  pinMode(PIN_CS1, OUTPUT);
  pinMode(PIN_BL,  OUTPUT);
  pinMode(PIN_DC,  OUTPUT);
  pinMode(PIN_BL,  OUTPUT);
  analogWrite(PIN_BL, 140);

  delay(200);
  digitalWrite(PIN_RST, LOW);
  delay(200);
  digitalWrite(PIN_RST, HIGH);
  delay(200);

  // Begin Initialization
  LCD_Write_Command(0x36);
  LCD_Write_Data(0xA0); 

  LCD_Write_Command(0x3A); 
  LCD_Write_Data(0x05);

  LCD_Write_Command(0x21);
  
  LCD_Write_Command(0x2A);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x01);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x3F);

  LCD_Write_Command(0x2B);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0xEF);

  LCD_Write_Command(0xB2);
  LCD_Write_Data(0x0C);
  LCD_Write_Data(0x0C);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0x33);
  LCD_Write_Data(0x33);

  LCD_Write_Command(0xB7);
  LCD_Write_Data(0x35); 

  LCD_Write_Command(0xBB);
  LCD_Write_Data(0x1F);

  LCD_Write_Command(0xC0);
  LCD_Write_Data(0x2C);

  LCD_Write_Command(0xC2);
  LCD_Write_Data(0x01);

  LCD_Write_Command(0xC3);
  LCD_Write_Data(0x12);   

  LCD_Write_Command(0xC4);
  LCD_Write_Data(0x20);

  LCD_Write_Command(0xC6);
  LCD_Write_Data(0x0F); 

  LCD_Write_Command(0xD0);
  LCD_Write_Data(0xA4);
  LCD_Write_Data(0xA1);

  LCD_Write_Command(0xE0);
  LCD_Write_Data(0xD0);
  LCD_Write_Data(0x08);
  LCD_Write_Data(0x11);
  LCD_Write_Data(0x08);
  LCD_Write_Data(0x0C);
  LCD_Write_Data(0x15);
  LCD_Write_Data(0x39);
  LCD_Write_Data(0x33);
  LCD_Write_Data(0x50);
  LCD_Write_Data(0x36);
  LCD_Write_Data(0x13);
  LCD_Write_Data(0x14);
  LCD_Write_Data(0x29);
  LCD_Write_Data(0x2D);

  LCD_Write_Command(0xE1);
  LCD_Write_Data(0xD0);
  LCD_Write_Data(0x08);
  LCD_Write_Data(0x10);
  LCD_Write_Data(0x08);
  LCD_Write_Data(0x06);
  LCD_Write_Data(0x06);
  LCD_Write_Data(0x39);
  LCD_Write_Data(0x44);
  LCD_Write_Data(0x51);
  LCD_Write_Data(0x0B);
  LCD_Write_Data(0x16);
  LCD_Write_Data(0x14);
  LCD_Write_Data(0x2F);
  LCD_Write_Data(0x31);
  
  LCD_Write_Command(0x21);
  LCD_Write_Command(0x11);
  LCD_Write_Command(0x29);
  
  // Set Window
  LCD_Write_Command(0x2a);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0 & 0xff);
  LCD_Write_Data( (LCD_WIDTH - 1) >> 8 );
  LCD_Write_Data( (LCD_WIDTH - 1) & 0xff );

  LCD_Write_Command(0x2b);
  LCD_Write_Data(0x00);
  LCD_Write_Data(0 & 0xff);
  LCD_Write_Data( (LCD_HEIGHT - 1) >> 8 );
  LCD_Write_Data( (LCD_HEIGHT - 1) & 0xff );

  LCD_Write_Command(0x2c);

  // Clear LCD
  digitalWrite(PIN_DC, HIGH);
  for(uint16_t i = 0; i < LCD_WIDTH; i++){
    for(uint16_t j = 0; j < LCD_HEIGHT; j++){
      SPI.transfer( (BLACK>>8) & 0xff );
      SPI.transfer( BLACK );
    }
  }
  
  // End SPI
  digitalWrite(PIN_CS1, HIGH);

}

void paintPixel(uint16_t x, uint16_t y, uint16_t color) {

  LCD_Write_Command(0x2a);
  LCD_Write_Data(x >> 8);
  LCD_Write_Data(x);

  LCD_Write_Command(0x2b);
  LCD_Write_Data(y >> 8);
  LCD_Write_Data(y);
  
  LCD_Write_Command(0x2c);

  digitalWrite(PIN_DC, HIGH);
  SPI.transfer((color>>8) & 0xff);
  SPI.transfer(color);
  
}

void drawLine(uint16_t start_x, uint16_t x_stop, uint16_t y, uint16_t color) { 

  for( uint16_t x = start_x; x <= x_stop; x++ ) {
    paintPixel( x, y, color);
  }
  
  digitalWrite(PIN_CS1, HIGH);

}

void drawRectangle(uint16_t start_x, uint16_t x_stop, uint16_t y_start, uint16_t y_stop, uint16_t color) { 

  for( uint16_t x = start_x; x <= x_stop; x++ ) {
    for( uint16_t y = y_start; y <= y_stop; y++ ) {
      paintPixel( x, y, color);
    }
  }
  
  digitalWrite(PIN_CS1, HIGH);

}

void drawChar(uint16_t x, uint16_t y, const char acsii, sFONT* Font, uint16_t back_color, uint16_t fore_color) {

  uint32_t Char_Offset = (acsii - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
  const unsigned char *ptr = &Font->table[Char_Offset];

  for ( uint16_t Page = 0; Page < Font->Height; Page ++ ) {
	  
    for ( uint16_t Column = 0; Column < Font->Width; Column ++ ) {
		
        if (pgm_read_byte(ptr) & (0x80 >> (Column % 8))) {
          paintPixel (x + Column, y + Page, fore_color );
        } else {
          paintPixel (x + Column, y + Page, back_color );
        }

      if (Column % 8 == 7) { ptr++; }
	  
    }
	
    if (Font->Width % 8 != 0) { ptr++; }
	
  }
}

void drawString(uint16_t x, uint16_t y, const char * pString, sFONT* Font, uint16_t back_color, uint16_t fore_color) {

  while (* pString != '\0') {
	drawChar(x, y, * pString, Font, back_color, fore_color);
    pString++;
	x += Font->Width;
  }
  
  digitalWrite(PIN_CS1, HIGH);

}

void initDisplay(void) {

  uint16_t x, y, i;
  uint8_t  j;
  bool set = false;

  initLCD();

  // Ribbon
  drawRectangle( 0, 320, 0, 23, BLUE );
  drawString(55, 2, "PINE Industries", &Font20, BLUE, WHITE);

  // Left/Right Separators
  drawLine(  25,  85, 135, BLUE );
  drawLine( 230, 290, 135, BLUE );
  
  // Car Icon
  x = 112;  // Start X Pixel
  y = 45;   // Start Y Pixel
  for ( i = 0; i < 787; i++ ) {
    for ( j = 0; j < pgm_read_byte( &(icon[i]) ); j++ ) {
      if ( set ) { paintPixel(x, y, WHITE); }
        x++;
        if ( x >= 202 ) { 
          x = 112;
          y++;
        }
      }
    set = !set;
  }
  
  digitalWrite(PIN_CS1, HIGH);
  
}

/** display_tire_data()
 *  
 * Input:
 *   uint8_t grid_num:          Value corresponding to tire sensor
 *   float   pressure:    Tire pressure in psi (see byte2psi())
 *   float   temperature: Tire temperature in °F
 *   
 * Notes:
 *   Draws Pressure/Temperature values on the LCD
*/
void displayTPMS( uint8_t grid_num, float pressure, float temperature ) {

  // White (0x0000) --> In Limits / Red --> Out of Limits
  uint16_t color = 0xF800 * (pressure <= 29 || pressure >= 38);

  // Rows / Columns
  uint8_t p_col = 21 * (grid_num == 1 || grid_num == 3)
                    + 226 * (grid_num == 2 || grid_num == 4);
                          
  uint8_t t_col = p_col + 4;

  uint8_t p_row = 70 * (grid_num == 1 || grid_num == 2)
                    + 170 * (grid_num == 3 || grid_num == 4);

  uint8_t t_row = p_row + 27;

  // Pressure String
  char px[6];
  dtostrf(pressure, 3, 1, px);
  String p = String(px);
  p.trim();

  // Temperature String
  char tx[4];
  dtostrf(round(temperature), 1, 0, tx);
  String t = String(tx);
  t.trim();
  t.concat("F");

  // Adjust Columns Based on String Length
  unsigned int len = t.length();
  p_col += 8 * (p.length() == 3);
  t_col += 17 * (len == 2)
                     + 10 * (len == 3)
                       + 1 * (len == 4);

  // Prevent Race Conditions on the SPI Bus with the CC1101
  //noInterrupts();

  // Overwrite Old Data with Black Rectangle
  drawRectangle( p_col - 5, p_col + 65, p_row, p_row + 45, BLACK );

  // Draw Pressure String
  drawString( p_col, p_row, p.c_str(), &Font24,  color,  WHITE );

  // Draw Temperature String
  drawString( t_col, t_row, t.c_str(), &Font20,  BLACK,  WHITE );

  // Re-Enable Interrupts
  //interrupts();

}
