#ifndef __LCD_H__
#define __LCD_H__

#include "Energia.h"

#include "Terminal6.h"
#include "Terminal12.h"

#define NON_INVERTED  false
#define INVERTED      true
#define BLACK         0xFF
#define WHITE         0x00
#define LCD_MAX_X     84
#define LCD_MAX_Y     48

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
  void clearPixel(uint8_t x, uint8_t y);
  void hline(uint8_t x, uint8_t y, uint8_t len);
  void vline(uint8_t x, uint8_t y, uint8_t len);
  void box(uint8_t x, uint8_t y, uint8_t lenx, uint8_t leny);
  void rect(uint8_t x, uint8_t y, uint8_t lenx, uint8_t leny);
  void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void circle(uint8_t x0, uint8_t y0, uint8_t radius);

  void text(uint8_t x, uint8_t y, String s);
  void scroll(uint16_t x, uint8_t y, String s);
  void progress(uint16_t x, uint8_t y);
  void printf(uint8_t x, float f);
  void dms(uint8_t x, float f);
  void hour(uint8_t x, float f);

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

#endif //ifndef __LCD_H__
