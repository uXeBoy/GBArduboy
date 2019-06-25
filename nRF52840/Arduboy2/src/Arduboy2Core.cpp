/**
 * @file Arduboy2Core.cpp
 * \brief
 * The Arduboy2Core class for Arduboy hardware initilization and control.
 */

#include "Arduboy2Core.h"

Arduboy2Core::Arduboy2Core() { }

uint8_t Arduboy2Core::upperByte = 0;
uint8_t Arduboy2Core::lowerByte = 0;
uint16_t Arduboy2Core::duration = 0;

bool Arduboy2Core::tonesPlaying = false;
static uint16_t *tonesStart = 0;
static uint16_t *tonesIndex = 0;

void Arduboy2Core::tone(uint16_t freq, uint16_t dur)
{
  uint16_t toneSequence[3] = {freq, dur, TONES_END};

  tone(toneSequence);
}

void Arduboy2Core::tone(uint16_t *tones)
{
  uint16_t freq;

  tonesStart = tonesIndex = tones; // set to start of sequence array

  freq = *tonesIndex++; // get tone frequency

  duration = *tonesIndex++; // get tone duration
  upperByte = 0xF0 + ((freq & 0x0700) >> 8); // volume full + upper 3 bits of freq
  lowerByte = (freq & 0x00FF); // lower 8 bits of freq

  tonesPlaying = true;
}

void Arduboy2Core::timer()
{
  uint16_t freq;

  if (duration == 0)
  {
    if (tonesPlaying)
    {
      freq = *tonesIndex++; // get tone frequency

      if (freq == TONES_END)
      {
        tonesPlaying = false; // if freq is actually an "end of sequence" marker
      }
      else
      {
        duration = *tonesIndex++; // get tone duration
        if (freq == NOTE_REST)
        {
          upperByte &= ~0xF0; // mute
        }
        else
        {
          upperByte = 0xF0 + ((freq & 0x0700) >> 8); // volume full + upper 3 bits of freq
          lowerByte = (freq & 0x00FF); // lower 8 bits of freq
        }
      }
    }
  }
  else if (--duration < 2)
  {
    upperByte &= ~0xF0; // mute
  }
}

void Arduboy2Core::boot()
{
  bootPins();
}

// Pins are set to the proper modes and levels for the specific hardware.
// This routine must be modified if any pins are moved to a different port
void Arduboy2Core::bootPins()
{
#ifdef ARDUBOY_10
  pinMode(A4, INPUT); // A_BUTTON     (P0.02)
  pinMode(A5, INPUT); // B_BUTTON     (P0.03)
  pinMode(A0, INPUT); // RIGHT_BUTTON (P0.04)
  pinMode(A1, INPUT); // LEFT_BUTTON  (P0.05)
  pinMode(11, INPUT); // UP_BUTTON    (P0.06)
  pinMode(6,  INPUT); // DOWN_BUTTON  (P0.07)

  pinMode(5,  INPUT); // CS_IS25LP128F (P1.08)
  pinMode(13, INPUT); // CRESET_B      (P1.09)

  pinMode(A2, OUTPUT); // dc   (P0.30)
  pinMode(A3, OUTPUT); // wclk (P0.28)
  pinMode(10, OUTPUT); // d0   (P0.27)
  pinMode(9,  OUTPUT); // d1   (P0.26)

  pinMode(12, OUTPUT); // Spare! (P0.08 / FPGA pin 30)
  NRF_P0->OUTCLR = 0x08;
#elif defined(AB_DEVKIT)

#endif
}

uint8_t Arduboy2Core::width() { return WIDTH; }

uint8_t Arduboy2Core::height() { return HEIGHT; }

/* Drawing */

void Arduboy2Core::paintScreen(uint8_t image[], bool clear)
{
  uint8_t upperByteTemp = upperByte;
  uint8_t lowerByteTemp = lowerByte;

  NRF_P0->OUTSET = DC_BIT; // dc HIGH

  for (uint8_t t = 0; t < 8; t++) // eight 'pages'
  {
    for (uint8_t r = 0; r < 8; r++) // eight bits
    {
      uint16_t a = t * 128; // starting address

      uint8_t bitMask = B00000001 << r;

      for (uint8_t i = 0; i < 16; i++) // 16 horizontal bytes = 128 pixels
      {
        if (image[a] & bitMask) NRF_P0->OUTSET = D0_BIT;
        else                    NRF_P0->OUTCLR = D0_BIT;

        a++;

        if (image[a] & bitMask) NRF_P0->OUTSET = D1_BIT;
        else                    NRF_P0->OUTCLR = D1_BIT;

        a++;

        NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
        NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH

        if (image[a] & bitMask) NRF_P0->OUTSET = D0_BIT;
        else                    NRF_P0->OUTCLR = D0_BIT;

        a++;

        if (image[a] & bitMask) NRF_P0->OUTSET = D1_BIT;
        else                    NRF_P0->OUTCLR = D1_BIT;

        a++;

        NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
        NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH

        if (image[a] & bitMask) NRF_P0->OUTSET = D0_BIT;
        else                    NRF_P0->OUTCLR = D0_BIT;

        a++;

        if (image[a] & bitMask) NRF_P0->OUTSET = D1_BIT;
        else                    NRF_P0->OUTCLR = D1_BIT;

        a++;

        NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
        NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH

        if (image[a] & bitMask) NRF_P0->OUTSET = D0_BIT;
        else                    NRF_P0->OUTCLR = D0_BIT;

        a++;

        if (image[a] & bitMask) NRF_P0->OUTSET = D1_BIT;
        else                    NRF_P0->OUTCLR = D1_BIT;

        a++;

        NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
        NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH
      }

      NRF_P0->OUTCLR = D0_BIT;
      NRF_P0->OUTCLR = D1_BIT;

      for (uint8_t i = 0; i < 56; i++) // 128 + 112 = 240 horizontal pixels
      {
        NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
        NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH
      }
    }
  }

  NRF_P0->OUTCLR = DC_BIT; // dc LOW

  for (uint8_t i = 0; i < 4; i++)
  {
    if (upperByteTemp & B10000000) NRF_P0->OUTSET = D0_BIT;
    else                           NRF_P0->OUTCLR = D0_BIT;
    if (upperByteTemp & B01000000) NRF_P0->OUTSET = D1_BIT;
    else                           NRF_P0->OUTCLR = D1_BIT;

    NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
    NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH

    upperByteTemp = upperByteTemp << 2;
  }
  for (uint8_t i = 0; i < 4; i++)
  {
    if (lowerByteTemp & B10000000) NRF_P0->OUTSET = D0_BIT;
    else                           NRF_P0->OUTCLR = D0_BIT;
    if (lowerByteTemp & B01000000) NRF_P0->OUTSET = D1_BIT;
    else                           NRF_P0->OUTCLR = D1_BIT;

    NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
    NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH

    lowerByteTemp = lowerByteTemp << 2;
  }
}

void Arduboy2Core::blank()
{
  NRF_P0->OUTCLR = D0_BIT;
  NRF_P0->OUTCLR = D1_BIT;

  NRF_P0->OUTSET = DC_BIT; // dc HIGH

  for (uint16_t i = 0; i < 19200; i++)
  {
    NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
    NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH
  }

  NRF_P0->OUTCLR = DC_BIT;   // dc LOW
  NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
  NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH
}

// turn all display pixels on, ignoring buffer contents
// or set to normal buffer display
void Arduboy2Core::allPixelsOn(bool on)
{
  if (on)
  {
    NRF_P0->OUTSET = D0_BIT;
    NRF_P0->OUTSET = D1_BIT;

    NRF_P0->OUTSET = DC_BIT; // dc HIGH

    for (uint16_t i = 0; i < 19200; i++)
    {
      NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
      NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH
    }

    NRF_P0->OUTCLR = DC_BIT;   // dc LOW
    NRF_P0->OUTCLR = WCLK_BIT; // wclk LOW
    NRF_P0->OUTSET = WCLK_BIT; // wclk HIGH
  }
}

/* Buttons */

uint8_t Arduboy2Core::buttonsState()
{
  uint8_t buttons;

#ifdef ARDUBOY_10

  buttons = ~((NRF_P0->IN) & 0xFC);

#elif defined(AB_DEVKIT)

#endif

  return buttons;
}

// delay in ms with 16 bit duration
void Arduboy2Core::delayShort(uint16_t ms)
{
  delay((unsigned long) ms);
}
