#ifndef __LCD_H__
#define __LCD_H__

#include "Energia.h"

#include "Terminal6.h"
#include "Terminal12.h"
#include "Greek11x16.h"

#define NON_INVERTED  false
#define INVERTED      true
#define BLACK         0xFF
#define WHITE         0x00
#define LCD_MAX_X     84
#define LCD_MAX_Y     48

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

class buffLCD {
public:
  buffLCD() {};
  void begin(uint8_t);
  String WhoAmI();

  void clear();
  void setFont(uint8_t font=0, bool inv=false);
  void setContrast(uint8_t val);
  void setDisplayMode(uint8_t mode);

  void pixel(uint8_t x, uint8_t y);
  void setPixel(uint8_t x, uint8_t y);
  boolean getPixel(uint8_t x, uint8_t y);
  void clearPixel(uint8_t x, uint8_t y);
  void hline(uint8_t x, uint8_t y, uint8_t len);
  void vline(uint8_t x, uint8_t y, uint8_t len);
  void box(uint8_t x, uint8_t y, uint8_t lenx, uint8_t leny);
  void rect(uint8_t x, uint8_t y, uint8_t lenx, uint8_t leny);
  void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void circle(uint8_t x0, uint8_t y0, uint8_t radius);

  void text(uint8_t x, uint8_t y, const char* s);
  void setChar(uint8_t x, uint8_t y, const char s);
  void scroll(uint16_t x, uint8_t y, const char* s);
  void progress(uint16_t x, uint8_t y, bool clear = true);
  void printf(uint8_t x, double f, uint8_t w = 14, uint8_t p = 10);
  void dms(uint8_t x, double f, bool sign = true);
  void hour(uint8_t x, double f);

  const uint8_t* get_screen_buff() { return (const uint8_t*)&_screen; };
  void dump_screen_buff(HardwareSerial* debug_port, boolean ascii = true, char white = ' ', char black = '#');

private:
  void setXY(uint8_t x, uint8_t y);
  void setX(uint8_t x);
  void setY(uint8_t y);
  void write(uint8_t dataCommand, uint8_t c);
  uint8_t _font;
  uint8_t _inverse;

  uint8_t _pinSerialData;
  uint8_t _pinDataCommand;
  uint8_t _pinSerialClock;

  //           x      y
  // _screen[0..84][0..5]
  volatile uint8_t _screen[LCD_MAX_X][LCD_MAX_Y / 8];
};

static const unsigned long _dv[] = {            //
             0,                                 // 0
            10,                                 // 1
           100,                                 // 2
          1000,                                 // 3
         10000,                                 // 4
        100000,                                 // 5
       1000000,                                 // 6
      10000000,                                 // 7
     100000000,                                 // 8
    1000000000                                  // 9
};

#endif //ifndef __LCD_H__
