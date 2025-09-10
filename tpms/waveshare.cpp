#include "waveshare.h"
#include "icon.h"

void LCD_Write_Command(uint8_t data) {	
  digitalWrite(LCD_CS_PIN, LOW);
  digitalWrite(LCD_DC_PIN, LOW);
  SPI.transfer(data);
}

void LCD_Write_Data(uint8_t data) {
  digitalWrite(LCD_CS_PIN, LOW);
  digitalWrite(LCD_DC_PIN, HIGH);
  SPI.transfer(data);
  digitalWrite(LCD_CS_PIN, HIGH);
}

void initLCD(void) {

  pinMode(LCD_CS_PIN,  OUTPUT);
  pinMode(LCD_RST_PIN, OUTPUT);
  pinMode(LCD_DC_PIN,  OUTPUT);
  pinMode(LCD_BL_PIN,  OUTPUT);
  analogWrite(LCD_BL_PIN, 140);

  delay(200);
  digitalWrite(LCD_RST_PIN, LOW);
  delay(200);
  digitalWrite(LCD_RST_PIN, HIGH);
  delay(200);
  
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));

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
  digitalWrite(LCD_DC_PIN, HIGH);
  for(uint16_t i = 0; i < LCD_WIDTH; i++){
    for(uint16_t j = 0; j < LCD_HEIGHT; j++){
      SPI.transfer( (BLACK>>8) & 0xff );
      SPI.transfer( BLACK );
    }
  }
  
  // End SPI
  digitalWrite(LCD_CS_PIN, HIGH);
  SPI.endTransaction();
  
}

void paintPixel(uint16_t x, uint16_t y, uint16_t color) {

  LCD_Write_Command(0x2a);
  LCD_Write_Data(x >> 8);
  LCD_Write_Data(x);

  LCD_Write_Command(0x2b);
  LCD_Write_Data(y >> 8);
  LCD_Write_Data(y);
  
  LCD_Write_Command(0x2c);

  digitalWrite(LCD_DC_PIN, HIGH);
  SPI.transfer((color>>8) & 0xff);
  SPI.transfer(color);
  
}

void drawLine(uint16_t start_x, uint16_t x_stop, uint16_t y, uint16_t color) { 

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));
  
  for( uint16_t x = start_x; x <= x_stop; x++ ) {
    paintPixel( x, y, color);
  }
  
  digitalWrite(LCD_CS_PIN, HIGH);
  SPI.endTransaction();

}

void drawRectangle(uint16_t start_x, uint16_t x_stop, uint16_t y_start, uint16_t y_stop, uint16_t color) { 

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));
  
  for( uint16_t x = start_x; x <= x_stop; x++ ) {
    for( uint16_t y = y_start; y <= y_stop; y++ ) {
      paintPixel( x, y, color);
    }
  }
  
  digitalWrite(LCD_CS_PIN, HIGH);
  SPI.endTransaction();

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

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));
  
  while (* pString != '\0') {
	drawChar(x, y, * pString, Font, back_color, fore_color);
    pString++;
	x += Font->Width;
  }
  
  digitalWrite(LCD_CS_PIN, HIGH);
  SPI.endTransaction();

}

void initDisplay(void) {

  uint16_t x, y, i;
  uint8_t  j;
  bool set = false;

  initLCD();

  /* Ribbon */
  drawRectangle( 0, 320, 0, 23, BLUE );
  drawString(55, 2, "PINE Industries", &Font20, BLUE, WHITE);

  /* Left/Right Separators */
  drawLine(  25,  85, 135, BLUE );
  drawLine( 230, 290, 135, BLUE );
  
  /* Car Icon */
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE3));
  
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
  
  digitalWrite(LCD_CS_PIN, HIGH);
  SPI.endTransaction();
  
}

/** display_tire_data()
 *  
 * Input:
 *   byte  grid_num:          Value corresponding to tire sensor
 *   double tire_pressure:    Tire pressure in psi (see byte2psi())
 *   double tire_temperature: Tire temperature in °F
 *   
 * Notes:
 *   Draws Pressure/Temperature values on the LCD
*/
void displayTPMS( byte grid_num, double tire_pressure, double tire_temperature ) {

  char px[6], tx[4];
  uint16_t color = 0;

  /* Values for grid_num = 1 */
  int  pressure_col    = 21;
  byte pressure_row    = 70;
  byte temperature_col = 23;
  byte temperature_row = 97;
  
  dtostrf(tire_pressure, 3, 1, px);
  String pressure = String(px);
  pressure.trim();

  dtostrf(round(tire_temperature), 1, 0, tx);
  String temperature = String(tx);
  temperature.trim();
  temperature.concat("F");

  if (grid_num == 2) {
    
    pressure_col    = 226;
    temperature_col = 228;
    
  } else if (grid_num == 3) {
    
    pressure_row    = 170;
    temperature_col = 23;
    temperature_row = 197;
    
  } else if (grid_num == 4) {
    
    pressure_col    = 226;
    pressure_row    = 170;
    temperature_col = 228;
    temperature_row = 197;
    
  }

  // Adjust Columns
  if (pressure.length() == 3)    { pressure_col = pressure_col + 8; }
  if (temperature.length() == 2) { temperature_col = temperature_col + 17; }
  else if (temperature.length() == 3) { temperature_col = temperature_col + 10; }
  else if (temperature.length() == 4) { temperature_col = temperature_col + 1; } 
  
  // Display Pressure & Temperature
  if (tire_pressure <= 29) {
    color = 0xF800;
  }

  noInterrupts();
  drawRectangle( pressure_col - 5, pressure_col + 65, pressure_row, pressure_row + 45, BLACK );
  drawString( pressure_col,    pressure_row,    pressure.c_str(),    &Font24,  color,  WHITE );
  drawString( temperature_col, temperature_row, temperature.c_str(), &Font20,  BLACK,  WHITE );
  interrupts();
  
}
