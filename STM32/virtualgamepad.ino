// adapted from https://github.com/eried/ArduboyVirtualGamepad/

#include <Arduboy2.h>

Arduboy2 arduboy;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(120);
}

bool dirty = true;

void loop() {
  if (!arduboy.nextFrame())
    return;

  arduboy.timer();

  dirty = !arduboy.nextFrame();

  if (dirty)
  {
      arduboy.clear();

      // Draw joystick

      if (arduboy.pressed(LEFT_BUTTON))
      {
        arduboy.fillTriangle(18, 13, 30, 25, 18, 37, WHITE); arduboy.fillRect(7, 13, 12, 25);
        arduboy.drawTriangle(46, 13, 46, 37, 34, 25, WHITE); arduboy.drawRect(46, 13, 12, 25);
        arduboy.drawLine(46, 14, 46, 36, BLACK);
      }
      else if (arduboy.pressed(RIGHT_BUTTON))
      {
        arduboy.drawTriangle(18, 13, 30, 25, 18, 37, WHITE); arduboy.drawRect(7, 13, 12, 25);
        arduboy.fillTriangle(46, 13, 46, 37, 34, 25, WHITE); arduboy.fillRect(46, 13, 12, 25);
        arduboy.drawLine(18, 14, 18, 36, BLACK);
      }
      else
      {
        arduboy.drawTriangle(18, 13, 30, 25, 18, 37, WHITE); arduboy.drawRect(7, 13, 12, 25);
        arduboy.drawTriangle(46, 13, 46, 37, 34, 25, WHITE); arduboy.drawRect(46, 13, 12, 25);
        arduboy.drawLine(18, 14, 18, 36, BLACK); arduboy.drawLine(46, 14, 46, 36, BLACK);
      }

      if (arduboy.pressed(UP_BUTTON))
      {
        arduboy.fillTriangle(32, 23, 44, 11, 20, 11, WHITE); arduboy.fillRect(20, 0, 25, 12);
        arduboy.drawTriangle(32, 27, 44, 39, 20, 39, WHITE); arduboy.drawRect(20, 39, 25, 12);
        arduboy.drawLine(21, 39, 43, 39, BLACK);
      }
      else if (arduboy.pressed(DOWN_BUTTON))
      {
        arduboy.drawTriangle(32, 23, 44, 11, 20, 11, WHITE); arduboy.drawRect(20, 0, 25, 12);
        arduboy.fillTriangle(32, 27, 44, 39, 20, 39, WHITE); arduboy.fillRect(20, 39, 25, 12);
        arduboy.drawLine(43, 11, 21, 11, BLACK);
      }
      else
      {
        arduboy.drawTriangle(32, 23, 44, 11, 20, 11, WHITE); arduboy.drawRect(20, 0, 25, 12);
        arduboy.drawTriangle(32, 27, 44, 39, 20, 39, WHITE); arduboy.drawRect(20, 39, 25, 12);
        arduboy.drawLine(43, 11, 21, 11, BLACK); arduboy.drawLine(21, 39, 43, 39, BLACK);
      }

    if (arduboy.pressed(B_BUTTON))
    {
      arduboy.fillCircle(88, 30, 12);
      arduboy.tone(1750, 5); // 440Hz
    }
    else
      arduboy.drawCircle(88, 30, 12);

    if (arduboy.pressed(A_BUTTON))
    {
      arduboy.fillCircle(115, 24, 12);
      arduboy.tone(1899, 5); // 880Hz
    }
    else
      arduboy.drawCircle(115, 24, 12);

    arduboy.display();
    dirty = false;
  }
}
