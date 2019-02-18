/**
 * @file Arduboy2Core.cpp
 * \brief
 * The Arduboy2Core class for Arduboy hardware initilization and control.
 */

#include "Arduboy2Core.h"

Arduboy2Core::Arduboy2Core() { }

void Arduboy2Core::boot()
{
  bootPins();
}

// Pins are set to the proper modes and levels for the specific hardware.
// This routine must be modified if any pins are moved to a different port
void Arduboy2Core::bootPins()
{
#ifdef ARDUBOY_10
  pinMode(14, INPUT); // A_BUTTON     (PB8)
  pinMode(24, INPUT); // B_BUTTON     (PB9)
  pinMode(31, INPUT); // RIGHT_BUTTON (PB12)
  pinMode(32, INPUT); // LEFT_BUTTON  (PB13)
  pinMode(33, INPUT); // UP_BUTTON    (PB14)
  pinMode(34, INPUT); // DOWN_BUTTON  (PB15)

  pinMode(15, OUTPUT); // dc   (PC0)
  pinMode(16, OUTPUT); // wclk (PC1)
  pinMode(17, OUTPUT); // d0   (PC2)
  pinMode(18, OUTPUT); // d1   (PC3)
#elif defined(AB_DEVKIT)

#endif
}

uint8_t Arduboy2Core::width() { return WIDTH; }

uint8_t Arduboy2Core::height() { return HEIGHT; }

/* Drawing */

void Arduboy2Core::paintScreen(uint8_t image[], bool clear, uint8_t lowerByte, uint8_t upperByte)
{
  gpio_write_bit(GPIOC, 0, HIGH);  // dc HIGH

  for (uint8_t t = 0; t < 8; t++) // eight 'pages'
  {
    for (uint8_t r = 0; r < 8; r++) // eight bits
    {
      uint16_t a = t * 128; // starting address

      uint8_t bitMask = B00000001 << r;

      for (uint8_t i = 0; i < 16; i++) // 16 horizontal bytes = 128 pixels
      {
        if (image[a] & bitMask) gpio_write_bit(GPIOC, 2, HIGH);
        else                    gpio_write_bit(GPIOC, 2, LOW);

        a++;

        if (image[a] & bitMask) gpio_write_bit(GPIOC, 3, HIGH);
        else                    gpio_write_bit(GPIOC, 3, LOW);

        a++;

        gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
        gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH

        if (image[a] & bitMask) gpio_write_bit(GPIOC, 2, HIGH);
        else                    gpio_write_bit(GPIOC, 2, LOW);

        a++;

        if (image[a] & bitMask) gpio_write_bit(GPIOC, 3, HIGH);
        else                    gpio_write_bit(GPIOC, 3, LOW);

        a++;

        gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
        gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH

        if (image[a] & bitMask) gpio_write_bit(GPIOC, 2, HIGH);
        else                    gpio_write_bit(GPIOC, 2, LOW);

        a++;

        if (image[a] & bitMask) gpio_write_bit(GPIOC, 3, HIGH);
        else                    gpio_write_bit(GPIOC, 3, LOW);

        a++;

        gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
        gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH

        if (image[a] & bitMask) gpio_write_bit(GPIOC, 2, HIGH);
        else                    gpio_write_bit(GPIOC, 2, LOW);

        a++;

        if (image[a] & bitMask) gpio_write_bit(GPIOC, 3, HIGH);
        else                    gpio_write_bit(GPIOC, 3, LOW);

        a++;

        gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
        gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH
      }

      gpio_write_bit(GPIOC, 2, LOW);
      gpio_write_bit(GPIOC, 3, LOW);

      for (uint8_t i = 0; i < 56; i++) // 128 + 112 = 240 horizontal pixels
      {
        gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
        gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH
      }
    }
  }

  gpio_write_bit(GPIOC, 0, LOW);  // dc LOW

  for (uint8_t i = 0; i < 4; i++)
  {
    if (upperByte & B10000000) gpio_write_bit(GPIOC, 2, HIGH);
    else                       gpio_write_bit(GPIOC, 2, LOW);
    if (upperByte & B01000000) gpio_write_bit(GPIOC, 3, HIGH);
    else                       gpio_write_bit(GPIOC, 3, LOW);

    gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
    gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH

    upperByte = upperByte << 2;
  }
  for (uint8_t i = 0; i < 4; i++)
  {
    if (lowerByte & B10000000) gpio_write_bit(GPIOC, 2, HIGH);
    else                       gpio_write_bit(GPIOC, 2, LOW);
    if (lowerByte & B01000000) gpio_write_bit(GPIOC, 3, HIGH);
    else                       gpio_write_bit(GPIOC, 3, LOW);

    gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
    gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH

    lowerByte = lowerByte << 2;
  }
}

void Arduboy2Core::blank()
{
  gpio_write_bit(GPIOC, 2, LOW);
  gpio_write_bit(GPIOC, 3, LOW);

  gpio_write_bit(GPIOC, 0, HIGH);  // dc HIGH

  for (uint16_t i = 0; i < 19200; i++)
  {
    gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
    gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH
  }

  gpio_write_bit(GPIOC, 0, LOW);  // dc LOW
  gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
  gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH
}

// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void Arduboy2Core::allPixelsOn(bool on)
{
  if (on)
  {
    gpio_write_bit(GPIOC, 2, HIGH);
    gpio_write_bit(GPIOC, 3, HIGH);

    gpio_write_bit(GPIOC, 0, HIGH);  // dc HIGH

    for (uint16_t i = 0; i < 19200; i++)
    {
      gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
      gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH
    }

    gpio_write_bit(GPIOC, 0, LOW);  // dc LOW
    gpio_write_bit(GPIOC, 1, LOW);  // wclk LOW
    gpio_write_bit(GPIOC, 1, HIGH); // wclk HIGH
  }
}

/* Buttons */

uint8_t Arduboy2Core::buttonsState()
{
  uint8_t buttons;

#ifdef ARDUBOY_10

  buttons = ~((GPIOB->regs->IDR) >> 8);

#elif defined(AB_DEVKIT)

#endif

  return buttons;
}

// delay in ms with 16 bit duration
void Arduboy2Core::delayShort(uint16_t ms)
{
  delay((unsigned long) ms);
}
