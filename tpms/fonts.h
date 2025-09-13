// COPYRIGHT(c) 2014 STMicroelectronics

#include <stdint.h>
#include <avr/pgmspace.h>

typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;

extern sFONT Font24;
extern sFONT Font20;