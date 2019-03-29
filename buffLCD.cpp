#include "Energia.h"
#include <SPI.h>
#include "buffLCD.h"

const uint8_t  _commandLCD = 0x00;
const uint8_t  _dataLCD    = 0x01;

void buffLCD::write(uint8_t dataCommand, uint8_t c) {
  digitalWrite(_pinDataCommand, dataCommand);
  SPI.transfer((char)c);
}

void buffLCD::setXY(uint8_t x, uint8_t y) {
  write(_commandLCD, PCD8544_SETYADDR | y);
  write(_commandLCD, PCD8544_SETXADDR | x);
}

void buffLCD::setX(uint8_t x) {
  write(_commandLCD, PCD8544_SETXADDR | x);
}

void buffLCD::setY(uint8_t y) {
  write(_commandLCD, PCD8544_SETYADDR | y);
}

void buffLCD::begin(uint8_t pinDataCommand) {

  _pinDataCommand = pinDataCommand;

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); // for LM4F120H5QR DIV2 = 4 MHz !

  pinMode(_pinDataCommand, OUTPUT);

  digitalWrite(_pinDataCommand, LOW);
  delay(100); // as per 8.1 Initialisation
  digitalWrite(_pinDataCommand, HIGH);

  write(_commandLCD, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION); // chip is active, horizontal addressing, use extended instruction set
  write(_commandLCD, PCD8544_SETVOP + 0x48); // write VOP to register: 0xC8 for 3V — try other values
  write(_commandLCD, PCD8544_SETBIAS + 0x02); // set Bias System 1:48
  write(_commandLCD, PCD8544_FUNCTIONSET); // chip is active, horizontal addressing, use basic instruction set
  write(_commandLCD, 0x09); // temperature control
  write(_commandLCD, 0x0C); // normal mode
  delay(10);

  clear();
  _font = 0;
}

String buffLCD::WhoAmI() {
  return "LCD Nokia 5110";
}

void buffLCD::setContrast(uint8_t val) {
  if(val > 0x7F)
    val = 0x7F;
  write(_commandLCD, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION); // chip is active, horizontal addressing, use extended instruction set
  write(_commandLCD, PCD8544_SETVOP + val); // write VOP to register: 0xC8 for 3V — try other values
  write(_commandLCD, PCD8544_FUNCTIONSET); // normal mode
}

void buffLCD::clear() {
  setXY(0, 0);
  for (uint16_t i=0; i<6*84; i++) {
    write(_dataLCD, _inverse ^ 0x00);
    _screen[i % LCD_MAX_X][i / LCD_MAX_X] = _inverse ^ 0x00;
  }
  setXY(0, 0);
}

void buffLCD::setFont(uint8_t font, bool inv) {
  _font = font;
  _inverse = inv?BLACK:WHITE;
}

void buffLCD::setDisplayMode(uint8_t mode) {
  _inverse = (mode == INVERTED)?BLACK:WHITE;
}

void buffLCD::text(uint8_t x, uint8_t y, const char* s) {
  uint8_t i;
  uint8_t j;

  if (_font==0) {
    for (j=0; j<strlen(s); j++) {
      for (i=0; i<5; i++) {
        _screen[6*(x+j) + i][y] = _inverse ^ Terminal6x8[s[j]-' '][i];
      }
      _screen[6*(x+j) + 5][y] = _inverse;
    }
  } else {
    if( (_font==1) || (_font==2)) {
      for (j=0; j<min(strlen(s),LCD_MAX_X/12); j++) {
        for (i=0; i<11; i++) {
          if(_font==1) {
            _screen[6*x+12*j+i][y]   = _inverse ^ Terminal11x16[s[j]-' '][2*i];
            _screen[6*x+12*j+i][y+1] = _inverse ^ Terminal11x16[s[j]-' '][2*i+1];
          } else {
            _screen[6*x+12*j+i][y]   = _inverse ^ Greek11x16[s[j]-'a'][2*i];
            _screen[6*x+12*j+i][y+1] = _inverse ^ Greek11x16[s[j]-'a'][2*i+1];
          }
        }
        _screen[6*x + 11][y]   = _inverse;
        _screen[6*x + 11][y+1] = _inverse;
      }
    }
  }

  setXY(6*x, y);

  if (_font==0) {
    for (j=0; j<(6*strlen(s)); j++) {
      write(_dataLCD, _screen[(6*x) + j][y]);
    }
  } else {
    if((_font==1) || (_font==2)) {
      for (j=0; j<(12*strlen(s)); j++) {
        write(_dataLCD, _screen[(6*x) + j][y]);
      }
      setXY(6*x, y+1);
      for (j=0; j<(12*strlen(s)); j++) {
        write(_dataLCD, _screen[(6*x) + j][y+1]);
      }
    }
  }
}

void buffLCD::setChar(uint8_t x, uint8_t y, const char s) {
  uint8_t i;

  if (_font==0) {
    for (i=0; i<5; i++)
      _screen[6*x + i][y] = _inverse ^ Terminal6x8[s-' '][i];
    _screen[6*x + 5][y] = _inverse;
  } else {
    if( (_font==1) || (_font==2)) {
      for (i=0; i<11; i++) {
        if(_font==1) {
          _screen[6*x+i][y]   = _inverse ^ Terminal11x16[s-' '][2*i];
          _screen[6*x+i][y+1] = _inverse ^ Terminal11x16[s-' '][2*i+1];
        } else {
          _screen[6*x+i][y]   = _inverse ^ Greek11x16[s-'a'][2*i];
          _screen[6*x+i][y+1] = _inverse ^ Greek11x16[s-'a'][2*i+1];
        }
      }
      _screen[6*x + 11][y]   = _inverse;
      _screen[6*x + 11][y+1] = _inverse;
    }
  }

  setXY(6*x, y);

  if (_font==0) {
    for (i=0; i<6; i++)
      write(_dataLCD, _screen[6*x+i][y]);
  } else {
    if((_font==1) || (_font==2)) {
      for (i=0; i<12; i++)
        write(_dataLCD, _screen[6*x + i][y]);
      setXY(6*x, y+1);
      for (i=0; i<12; i++)
        write(_dataLCD, _screen[6*x + i][y+1]);
    }
  }
}

void buffLCD::scroll(uint16_t x, uint8_t y, const char* s) {
  uint16_t i = 0;
  uint16_t j = 0;
  uint8_t k;

  if (_font==0) {

    // clear piece left of text
    if(x < LCD_MAX_X)
      for(i = 0; i < (LCD_MAX_X - x); ++i)
        _screen[i][y] = _inverse;

    while((i < LCD_MAX_X) && (j<strlen(s))) {
      j = (i - (LCD_MAX_X - x)) / 6;
      if(j==strlen(s))
        break;
      k = (i - (LCD_MAX_X - x)) % 6;
      if(k%6 != 5)
        _screen[i][y] = _inverse ^ Terminal6x8[s[j]-' '][k];
      else
        _screen[i][y] = _inverse;
      ++i;
    }

    while(i < (LCD_MAX_X - 1)) {
      _screen[i++][y] = _inverse;
    }
  } else if (_font==1) {

    // clear piece left of text
    if(x < LCD_MAX_X)
      for(i = 0; i < (LCD_MAX_X - x); ++i) {
        _screen[i][y] = _inverse;
        _screen[i][y+1] = _inverse;
      }

    while((i < LCD_MAX_X) && (j<strlen(s))) {
      j = (i - (LCD_MAX_X - x)) / 12;
      if(j==strlen(s))
        break;
      k = (i - (LCD_MAX_X - x)) % 12;
      if(k%12 != 11) {
        _screen[i][y] = _inverse ^ Terminal11x16[s[j]-' '][2*k];
        _screen[i][y+1] = _inverse ^ Terminal11x16[s[j]-' '][2*k+1];
      } else {
        _screen[i][y] = _inverse;
        _screen[i][y+1] = _inverse;
      }
      ++i;
    }
  }

  setXY(0, y);
  if (_font==0) {
    for (i=0; i<LCD_MAX_X; i++) {
      write(_dataLCD, _screen[i][y]);
    }
  } else
    if (_font==1) {
      for (i=0; i<LCD_MAX_X; i++) {
        write(_dataLCD, _screen[i][y]);
      }
      setXY(0, y+1);
      for (i=0; i<LCD_MAX_X; i++) {
        write(_dataLCD, _screen[i][y+1]);
      }
    }
}

void buffLCD::progress(uint16_t x, uint8_t y, bool clear) {
  uint16_t i = 0;

//  rect(0, y*6, LCD_MAX_X, 8);
  setXY(0, y);
  while(i<LCD_MAX_X-1) {
    if(clear)
      _screen[i][y] = 0x00;
    _screen[i][y] ^= _inverse ^ (i<x)?0xFF:0x81;
    write(_dataLCD, _screen[i][y]);
    ++i;
  }
  _screen[LCD_MAX_X-1][y] = _inverse ^ 0xFF;
  write(_dataLCD, _screen[LCD_MAX_X-1][y]);
}

void buffLCD::pixel(uint8_t x, uint8_t y) {
  uint8_t row = y / 8;
  if(row > (LCD_MAX_Y/8))
    return;
  uint8_t line = 1 << (y%8);

  _screen[x%LCD_MAX_X][row] ^= line;
  setXY(x, row);
  write(_dataLCD, _screen[x%LCD_MAX_X][row]);
};

void buffLCD::clearPixel(uint8_t x, uint8_t y) {
  uint8_t row = y / 8;
  if(row > (LCD_MAX_Y/8))
    return;
  uint8_t line = 1 << (y%8);

  _screen[x%LCD_MAX_X][row] &= ~line;
  setXY(x, row);
  write(_dataLCD, _screen[x%LCD_MAX_X][row]);
};

void buffLCD::setPixel(uint8_t x, uint8_t y) {
  uint8_t row = y / 8;
  if(row > (LCD_MAX_Y/8))
    return;
  uint8_t line = 1 << (y%8);

  _screen[x%LCD_MAX_X][row] |= line;
  setXY(x, row);
  write(_dataLCD, _screen[x%LCD_MAX_X][row]);
};

boolean buffLCD::getPixel(uint8_t x, uint8_t y) {
  uint8_t row = y / 8;
  if(row > (LCD_MAX_Y/8))
    return false;
  uint8_t line = 1 << (y%8);

  return (_screen[x%LCD_MAX_X][row] & line);
};

void buffLCD::hline(uint8_t x, uint8_t y, uint8_t len) {
  uint8_t row = y / 8;
  uint8_t line = 1 << (y%8);

  setXY(x, row);
  for(uint8_t k = 0; k < len; ++k) {
    _screen[(x+k)%LCD_MAX_X][row] |= line;
    write(_dataLCD, _screen[(x+k)%LCD_MAX_X][row]);
  }
};

void buffLCD::vline(uint8_t x, uint8_t y, uint8_t len) {
  uint8_t row = y / 8;
  uint8_t line = 1 << (y%8);

  while(len > 0) {
    line |= 1 << (y%8);
    ++y;
    --len;
    if((y%8 == 0) || (len == 0)) {
      _screen[x%LCD_MAX_X][row] |= line;
      setXY(x, row);
      write(_dataLCD, _screen[x%LCD_MAX_X][row]);
      ++row;
      line = 0;
    }
  }
};

void buffLCD::box(uint8_t x, uint8_t y, uint8_t lenx, uint8_t leny) {
  uint8_t row = y / 8;
  uint8_t line = 1 << (y%8);

  if(x+lenx>LCD_MAX_X)
    lenx = LCD_MAX_X - x;
  if(y+leny>LCD_MAX_Y)
    leny = LCD_MAX_Y - y;

  while(leny > 0) {
    line |= 1 << (y%8);
    ++y;
    --leny;
    if((y%8 == 0) || (leny == 0)) {
      setX(x);
      setY(row);
      for(uint8_t xt = x; xt < x+lenx; ++xt) {
        _screen[xt%LCD_MAX_X][row] ^= line;
        write(_dataLCD, _screen[xt%LCD_MAX_X][row]);
      }
      ++row;
      line = 0;
    }
  }
};

void buffLCD::rect(uint8_t x, uint8_t y, uint8_t lenx, uint8_t leny) {
  uint8_t row = y / 8;
  uint8_t line = 1 << (y%8);
  uint8_t vline = 1 << (y%8);

  if(x>LCD_MAX_X)
    return;
  if(y>LCD_MAX_Y)
    return;
  if(lenx<1)
    return;
  if(leny<1)
    return;

  if(x+lenx>=LCD_MAX_X)
    lenx = LCD_MAX_X - x - 1;
  if(y+leny>=LCD_MAX_Y)
    leny = LCD_MAX_Y - y - 1;

  while(leny != 0) {
    line |= 1 << (y%8);
    ++y;
    --leny;
   if(y%8 == 0)
     break;
  }

  // draw vertical top left piece (max 8 pixels)
  setXY(x, row);
  _screen[x][row] ^= line;
  write(_dataLCD, _screen[x][row]);

  if(leny == 0)
    vline |= 1 << ((y-1)%8);

  // draw horizontal top border
  if(lenx>2) {
    for(uint8_t xt = x+1; xt < x+lenx; ++xt) {
      _screen[xt][row] ^= vline;
      write(_dataLCD, _screen[xt][row]);
    }
  }

  // draw finishing rightmost top border
  _screen[x+lenx][row] ^= line;
  write(_dataLCD, _screen[x+lenx][row]);

  if(leny == 0)
    return;

  ++row;
  line = 0;

  // draw left and right border in pieces of 8 pixels
  while(leny > 0) {
    vline = 1 << (y%8);
    line |= vline;
    ++y;
    --leny;
    if((y%8 == 0) || (leny == 0)) {

      setXY(x, row);
      _screen[x][row] ^= line;
      write(_dataLCD, _screen[x][row]);

      if(leny == 0) {
        if(lenx>2) {
          for(uint8_t xt = x+1; xt < x+lenx; ++xt) {
            _screen[xt][row] ^= vline;
            write(_dataLCD, _screen[xt][row]);
          }
        }
        _screen[x+lenx][row] ^= line;
        write(_dataLCD, _screen[x+lenx][row]);
        return;
      }

      setXY(x+lenx, row);
      _screen[x+lenx][row] ^= line;
      write(_dataLCD, _screen[x+lenx][row]);

      ++row;
      line = 0;
    }
  }
};

#define SWAP(a,b) {temp=a;a=b;b=temp;}
void buffLCD::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
  boolean steep = abs(y1-y0) > abs(x1-x0);
  uint8_t temp;
  if(steep) {
    SWAP(x0,y0);
    SWAP(x1,y1);
  }
  if(x0>x1) {
    SWAP(x0,x1);
    SWAP(y0,y1);
  }
  int deltax = x1-x0;
  int deltay = abs(y1 - y0);
  int error = deltax / 2;
  int ystep;
  int y = y0;
  if(y0<y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }
  for(uint8_t x=x0; x < x1; ++x) {
    if(steep)
      setPixel(y,x);
    else
      setPixel(x,y);
    error = error - deltay;
    if(error < 0) {
      y = y + ystep;
      error = error + deltax;
    }
  }
};

void buffLCD::circle(uint8_t x0, uint8_t y0, uint8_t radius)
{
  int x = radius;
  int y = 0;
  int radiusError = 1-x;

  while(x >= y) {
    setPixel(x + x0, y + y0);
    setPixel(y + x0, x + y0);
    setPixel(-x + x0, y + y0);
    setPixel(-y + x0, x + y0);
    setPixel(-x + x0, -y + y0);
    setPixel(-y + x0, -x + y0);
    setPixel(x + x0, -y + y0);
    setPixel(y + x0, -x + y0);
    y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    }
    else
    {
      x--;
      radiusError += 2 * (y - x + 1);
    }
  }
}

/*static const unsigned long _dv[] = {            //
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
};*/

uint8_t _numDigits(long x) {
  unsigned long absx = abs(x);
  unsigned char k = 0;
  while((k < 10) && (absx >= _dv[k]))
    ++k;
  return (x < 0)?(k+1):k;
};

void buffLCD::printf(uint8_t line, double val, uint8_t width, uint8_t prec) {
  char _string[width+1];
  char* pp = _string;
  bool neg = val < 0.0;
//  val = isnan(val)?0.0:val;
//  sprintf(_string, "%*.*f", width, 1+prec-_numDigits(val), val);
//  sprintf(_string, "%s%s%*.*f0000", (val<100.0)?" ":"", (val<10.0)?" ":"", width-3, prec, val);
  if(isnan(val))
    sprintf(_string, "NaN");
  else {
    pp += sprintf(_string, "%*i", width-prec-(prec>0), int(val));
    if(neg)
      val *= -1.0;
    if(prec) {
      pp += sprintf(pp, ".");
      val -= int(val);
      while(prec--) {
        val *= 10.0;
        pp += sprintf(pp, "%i", int(val));
        val -= int(val);
      }
    }
  }
  _string[width] = 0;
  text(0, line, _string);
};

void buffLCD::dms(uint8_t line, const double rad, bool sign) {
  signed int d;
  unsigned int m, fracs;
  double s;
  char _string[32];
  bool neg = false;

  s = RAD_TO_DEG * (isnan(rad)?0.0:rad);
  neg = s < 0.0;
  if(neg)
    s *= -1.0;
  d = (signed int)floor(s);
  s -= d;
  if(neg)
    d *= -1;
  s = fabs(s);
  s *= 60.0;
  m = (unsigned int)s;
  s -= m;
  s *= 60.0;

  fracs = 1000.0 * (s - int(s));

  // options:
  // either it is a -90 ... +90 degrees declination or altitude
  //     or it is a 0 ... 360 degrees azimuth
  //                01234567890123
  //                -89*59'59.999"
  //                 89*59'59.999"
  //                359*59'59.999"

  if((rad < 0.0) || sign) //
    sprintf(_string, "%c%02d%c%02d'%02i\.%03i\"", neg?'-':'+', abs(d), 0x7F, m, int(s), fracs);
//  else if(d < 100)
//      sprintf(_string, "%02d%c%02d'%08.5f", d, 0x7F, m, s);
  else
    sprintf(_string, "%03d%c%02d'%02i\.%03i\"", d, 0x7F, m, int(s), fracs);

//  if(sign && (d < 100))
//    sprintf(_string, "%+3d%c%02d'%07.4f", d, 0x7F, m, s);
//  else
//    sprintf(_string, "%3d%c%02d'%07.4f", abs(d), 0x7F, m, s);

  text(0, line, _string);
};

void buffLCD::hour(uint8_t line, const double rad) {
  unsigned int h;
  unsigned int m;
  double s = isnan(rad)?0.0:rad, fracs;
  char _string[32];

  while(s > TWO_PI)
    s -= TWO_PI;
  while(s < 0)
    s += TWO_PI;

  s = fabs(12.0 * s / PI);
  h = (unsigned int)s;
  s -= h;
  s *= 60.0;
  m = (unsigned int)s;
  s -= m;
  s *= 60.0;
  fracs = (10000.0 * s) - (10000.0 * int(s));

  //                01234567890123
  //                23:59:59.00000
//  sprintf(_string, "%02d:%02d:%08.5f", h, m, s);
  //                01234567890123
  //                 23:59:59.0000
  sprintf(_string, " %02d:%02d:%02i\.%04i", h, m, int(s), int(fracs));
  text(0, line, _string);
};

//void buffLCD::dump_screen_buff(HardwareSerial* debug_port, boolean ascii = true, char white = ' '; char black = '#')
void buffLCD::dump_screen_buff(HardwareSerial* debug_port, boolean ascii, char white, char black) {
  if(ascii) {
    for(uint8_t y = 0; y < LCD_MAX_Y; ++y) {
      for(uint8_t x = 0; x < LCD_MAX_X; ++x)
        debug_port->write(getPixel(x,y)?black:white);
      debug_port->println();
    }
  } else {
    // binary
    for(uint8_t y = 0; y < LCD_MAX_Y / 8; ++y) {
      for(uint8_t x = 0; x < LCD_MAX_X; ++x) {
        debug_port->write(_screen[x][y]);
      }
    }
  }
};
