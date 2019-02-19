/**
 * @file Arduboy2Core.h
 * \brief
 * The Arduboy2Core class for Arduboy hardware initilization and control.
 */

#ifndef ARDUBOY2_CORE_H
#define ARDUBOY2_CORE_H

#include <Arduino.h>

// main hardware compile flags

#if !defined(ARDUBOY_10) && !defined(AB_DEVKIT)
/* defaults to Arduboy Release 1.0 if not using a boards.txt file
 *
 * we default to Arduboy Release 1.0 if a compile flag has not been
 * passed to us from a boards.txt file
 *
 * if you wish to compile for the devkit without using a boards.txt
 * file simply comment out the ARDUBOY_10 define and uncomment
 * the AB_DEVKIT define like this:
 *
 *     // #define ARDUBOY_10
 *     #define AB_DEVKIT
 */
#define ARDUBOY_10   //< compile for the production Arduboy v1.0
// #define AB_DEVKIT    //< compile for the official dev kit
#endif

// bit values for button states
// these are determined by the buttonsState() function
#define A_BUTTON     _BV(0)     /**< The A button value for functions requiring a bitmask */
#define B_BUTTON     _BV(1)     /**< The B button value for functions requiring a bitmask */
#define RIGHT_BUTTON _BV(4) /**< The Right button value for functions requiring a bitmask */
#define LEFT_BUTTON  _BV(5)  /**< The Left button value for functions requiring a bitmask */
#define UP_BUTTON    _BV(6)    /**< The Up button value for functions requiring a bitmask */
#define DOWN_BUTTON  _BV(7)  /**< The Down button value for functions requiring a bitmask */

#define WIDTH 128 /**< The width of the display in pixels */
#define HEIGHT 64 /**< The height of the display in pixels */

#define NOTE_REST       0
#define NOTE_C3         44
#define NOTE_CS3        156
#define NOTE_D3         262
#define NOTE_DS3        363
#define NOTE_E3         457
#define NOTE_F3         547
#define NOTE_FS3        631
#define NOTE_G3         710
#define NOTE_GS3        786
#define NOTE_A3         854
#define NOTE_AS3        923
#define NOTE_B3         986
#define NOTE_C4         1046
#define NOTE_CS4        1102
#define NOTE_D4         1155
#define NOTE_DS4        1205
#define NOTE_E4         1253
#define NOTE_F4         1297
#define NOTE_FS4        1339
#define NOTE_G4         1379
#define NOTE_GS4        1417
#define NOTE_A4         1452
#define NOTE_AS4        1486
#define NOTE_B4         1517
#define NOTE_C5         1546
#define NOTE_CS5        1575
#define NOTE_D5         1602
#define NOTE_DS5        1627
#define NOTE_E5         1650
#define NOTE_F5         1673
#define NOTE_FS5        1694
#define NOTE_G5         1714
#define NOTE_GS5        1732
#define NOTE_A5         1750
#define NOTE_AS5        1767
#define NOTE_B5         1783
#define NOTE_C6         1798
#define NOTE_CS6        1812
#define NOTE_D6         1825
#define NOTE_DS6        1837
#define NOTE_E6         1849
#define NOTE_F6         1860
#define NOTE_FS6        1871
#define NOTE_G6         1881
#define NOTE_GS6        1890
#define NOTE_A6         1899
#define NOTE_AS6        1907
#define NOTE_B6         1915
#define NOTE_C7         1923
#define NOTE_CS7        1930
#define NOTE_D7         1936
#define NOTE_DS7        1943
#define NOTE_E7         1949
#define NOTE_F7         1954
#define NOTE_FS7        1959
#define NOTE_G7         1964
#define NOTE_GS7        1969
#define NOTE_A7         1974
#define NOTE_AS7        1978
#define NOTE_B7         1982
#define NOTE_C8         1985
#define NOTE_CS8        1988
#define NOTE_D8         1992
#define NOTE_DS8        1995
#define NOTE_E8         1998
#define NOTE_F8         2001
#define NOTE_FS8        2004
#define NOTE_G8         2006
#define NOTE_GS8        2009
#define NOTE_A8         2011
#define NOTE_AS8        2013
#define NOTE_B8         2015

/** \brief
 * Lower level functions generally dealing directly with the hardware.
 *
 * \details
 * This class is inherited by Arduboy2Base and thus also Arduboy2, so wouldn't
 * normally be used directly by a sketch.
 *
 * \note
 * A friend class named _Arduboy2Ex_ is declared by this class. The intention
 * is to allow a sketch to create an _Arduboy2Ex_ class which would have access
 * to the private and protected members of the Arduboy2Core class. It is hoped
 * that this may eliminate the need to create an entire local copy of the
 * library, in order to extend the functionality, in most circumstances.
 */
class Arduboy2Core
{
  friend class Arduboy2Ex;

  public:
    Arduboy2Core();

    /** \brief
     * Get the width of the display in pixels.
     *
     * \return The width of the display in pixels.
     *
     * \note
     * In most cases, the defined value `WIDTH` would be better to use instead
     * of this function.
     */
    uint8_t static width();

    /** \brief
     * Get the height of the display in pixels.
     *
     * \return The height of the display in pixels.
     *
     * \note
     * In most cases, the defined value `HEIGHT` would be better to use instead
     * of this function.
     */
    uint8_t static height();

    /** \brief
     * Get the current state of all buttons as a bitmask.
     *
     * \return A bitmask of the state of all the buttons.
     *
     * \details
     * The returned mask contains a bit for each button. For any pressed button,
     * its bit will be 1. For released buttons their associated bits will be 0.
     *
     * The following defined mask values should be used for the buttons:
     *
     * LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, A_BUTTON, B_BUTTON
     */
    uint8_t static buttonsState();

    /** \brief
     * Paints an entire image directly to the display from an array in RAM.
     *
     * \param image A byte array in RAM representing the entire contents of
     * the display.
     * \param clear If `true` the array in RAM will be cleared to zeros upon
     * return from this function. If `false` the RAM buffer will remain
     * unchanged. (optional; defaults to `false`)
     *
     * \details
     * The contents of the specified array in RAM is written to the display.
     * Each byte in the array represents a vertical column of 8 pixels with
     * the least significant bit at the top. The bytes are written starting
     * at the top left, progressing horizontally and wrapping at the end of
     * each row, to the bottom right. The size of the array must exactly
     * match the number of pixels in the entire display.
     *
     * If parameter `clear` is set to `true` the RAM array will be cleared to
     * zeros after its contents are written to the display.
     *
     * \see paint8Pixels()
     */
    void static paintScreen(uint8_t image[], bool clear = false);

    /** \brief
     * Blank the display screen by setting all pixels off.
     *
     * \details
     * All pixels on the screen will be written with a value of 0 to turn
     * them off.
     */
    void static blank();

    /** \brief
     * Turn all display pixels on or display the buffer contents.
     *
     * \param on `true` turns all pixels on. `false` displays the contents
     * of the hardware display buffer.
     *
     * \details
     * Calling this function with a value of `true` will override the contents
     * of the hardware display buffer and turn all pixels on. The contents of
     * the hardware buffer will remain unchanged.
     *
     * Calling this function with a value of `false` will set the normal state
     * of displaying the contents of the hardware display buffer.
     *
     * \note
     * All pixels will be lit even if the display is in inverted mode.
     *
     * \see invert()
     */
    void static allPixelsOn(bool on);

    /** \brief
     * Initialize the Arduboy's hardware.
     *
     * \details
     * This function initializes the display, buttons, etc.
     *
     * This function is called by begin() so isn't normally called within a
     * sketch. However, in order to free up some code space, by eliminating
     * some of the start up features, it can be called in place of begin().
     * The functions that begin() would call after boot() can then be called
     * to add back in some of the start up features, if desired.
     * See the README file or documentation on the main page for more details.
     *
     * \see Arduboy2Base::begin()
     */
    void static boot();

    /** \brief
     * Delay for the number of milliseconds, specified as a 16 bit value.
     *
     * \param ms The delay in milliseconds.
     *
     * \details
     * This function works the same as the Arduino `delay()` function except
     * the provided value is 16 bits long, so the maximum delay allowed is
     * 65535 milliseconds (about 65.5 seconds). Using this function instead
     * of Arduino `delay()` will save a few bytes of code.
     */
    void static delayShort(uint16_t ms) __attribute__ ((noinline));

  /** \brief
   * The counter used by the `timer()` function to time the duration of a tone.
   *
   * \details
   * This variable is set by the `dur` parameter of the `tone()` function.
   * It is then decremented each time the `timer()` function is called, if its
   * value isn't 0. When `timer()` decrements it to 0, a tone that is playing
   * will be stopped.
   *
   * A sketch can determine if a tone is currently playing by testing if
   * this variable is non-zero (assuming it's a timed tone, not a continuous
   * tone).
   *
   * Example:
   * \code{.cpp}
   * beep.tone(beep.freq(1000), 15);
   * while (beep.duration != 0) { } // wait for the tone to stop playing
   * \endcode
   *
   * It can also be manipulated directly by the sketch, although this should
   * seldom be necessary.
   */
  static uint8_t duration;
  static uint8_t upperByte;
  static uint8_t lowerByte;

  /** \brief
   * Play a tone for a given duration.
   *
   * \param count The count to be loaded into the timer/counter to play
   *              the desired frequency.
   * \param dur The duration of the tone, used by `timer()`.
   *
   * \details
   * A tone is played for the specified duration, or until replaced by another
   * tone or stopped using `noTone()`.
   *
   * The tone's frequency is determined by the specified count, which is loaded
   * into the timer/counter that generates the tone. A desired frequency can be
   * converted into the required count value using the `freq()` function.
   *
   * The duration value is the number of times the `timer()` function must be
   * called before the tone is stopped.
   *
   * \see freq() timer() noTone()
   */
  static void tone(uint16_t freq, uint8_t dur);

  /** \brief
   * Handle the duration that a tone plays for.
   *
   * \details
   * This function must be called at a constant interval, which would normally
   * be once per frame, in order to stop a tone after the desired tone duration
   * has elapsed.
   *
   * If the value of the `duration` variable is not 0, it will be decremented.
   * When the `duration` variable is decremented to 0, a playing tone will be
   * stopped.
   */
  static void timer();

  protected:
    // internals
    void static bootPins();
};

#endif
