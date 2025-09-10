/* Libraries */
#include <stdint.h>
#include <SPI.h>
#include "fonts.h"

/* GPIO Configuration */
#define LCD_RST_PIN   3
#define LCD_BL_PIN    4
#define LCD_DC_PIN    5
#define LCD_CS_PIN    7

/* LCD Size */
#define LCD_WIDTH   320
#define LCD_HEIGHT  240

/* Colors */
#define WHITE               0xFFFF
#define BLACK               0x0000    
#define BLUE                0x001F  
#define BRED                0xF81F
#define GRED                0xFFE0
#define GBLUE               0x07FF
#define RED                 0xF800
#define MAGENTA             0xF81F
#define GREEN               0x07E0
#define CYAN                0x7FFF
#define YELLOW              0xFFE0
#define BROWN               0xBC40 
#define BRRED               0xFC07 
#define GRAY                0x8430 
#define DARKBLUE            0x01CF  
#define LIGHTBLUE           0x7D7C   
#define GRAYBLUE            0x5458 
#define LIGHTGREEN          0x841F 
#define LGRAY               0xC618 
#define LGRAYBLUE           0xA651
#define LBBLUE              0x2B12 

//static void LCD_Write_Command(uint8_t data);
void LCD_Write_Command(uint8_t data);

//static void LCD_Write_Data(uint8_t data);
void LCD_Write_Data(uint8_t data);

void initLCD(void);

void paintPixel(uint16_t x, uint16_t y, uint16_t Color);

void drawLine(uint16_t start_x, uint16_t x_stop, uint16_t y, uint16_t color);

void drawRectangle(uint16_t start_x, uint16_t x_stop, uint16_t y_start, uint16_t y_stop, uint16_t color);

void drawChar(uint16_t x, uint16_t y, const char acsii, sFONT* Font, uint16_t back_color, uint16_t fore_color);

void drawString(uint16_t x, uint16_t y, const char * pString, sFONT* Font, uint16_t back_color, uint16_t fore_color);

void initDisplay(void);

void displayTPMS( byte grid_num, double tire_pressure, double tire_temperature );