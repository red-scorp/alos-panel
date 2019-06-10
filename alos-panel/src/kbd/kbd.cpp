/*!
  \file kbd.cpp
  \brief ALoS-Panel Project Keyboard code
  \author Andriy Golovnya (andriy.golovnya@googlemail.com)
*/

#include "../private.h"
#include "kbd.h"

/* Keyboard Configuration sanity check */
#if (!defined(KBD_NONE) && !defined(KBD_D_MATRIX) && !defined(KBD_A_JOYSTICK) \
  && !defined(KBD_A_KEYPAD) && !defined(KBD_ROTARY_ENCODER) && !defined(KBD_A_MATRIX) \
  && !defined(KBD_I2C_RGB)) \
  || (defined(KBD_NONE) && defined(KBD_D_MATRIX)) \
  || (defined(KBD_NONE) && defined(KBD_A_JOYSTICK)) \
  || (defined(KBD_NONE) && defined(KBD_A_KEYPAD)) \
  || (defined(KBD_NONE) && defined(KBD_ROTARY_ENCODER)) \
  || (defined(KBD_NONE) && defined(KBD_A_MATRIX)) \
  || (defined(KBD_NONE) && defined(KBD_I2C_RGB)) \
  || (defined(KBD_D_MATRIX) && defined(KBD_A_JOYSTICK)) \
  || (defined(KBD_D_MATRIX) && defined(KBD_A_KEYPAD)) \
  || (defined(KBD_D_MATRIX) && defined(KBD_ROTARY_ENCODER)) \
  || (defined(KBD_D_MATRIX) && defined(KBD_A_MATRIX)) \
  || (defined(KBD_D_MATRIX) && defined(KBD_I2C_RGB)) \
  || (defined(KBD_A_JOYSTICK) && defined(KBD_A_KEYPAD)) \
  || (defined(KBD_A_JOYSTICK) && defined(KBD_ROTARY_ENCODER)) \
  || (defined(KBD_A_JOYSTICK) && defined(KBD_A_MATRIX)) \
  || (defined(KBD_A_JOYSTICK) && defined(KBD_I2C_RGB)) \
  || (defined(KBD_A_KEYPAD) && defined(KBD_ROTARY_ENCODER)) \
  || (defined(KBD_A_KEYPAD) && defined(KBD_A_MATRIX)) \
  || (defined(KBD_A_KEYPAD) && defined(KBD_I2C_RGB)) \
  || (defined(KBD_ROTARY_ENCODER) && defined(KBD_A_MATRIX)) \
  || (defined(KBD_ROTARY_ENCODER) && defined(KBD_I2C_RGB)) \
  || (defined(KBD_A_MATRIX) && defined(KBD_I2C_RGB))
#error You should define KBD_NONE, KBD_D_MATRIX, KBD_A_JOYSTICK, KBD_A_KEYPAD, KBD_ROTARY_ENCODER, KBD_A_MATRIX or KBD_I2C_RGB and only one of them!
#endif

#if defined(KBD_D_MATRIX) && \
  (!defined(KBD_ROWS) || !defined(KBD_COLS))
#error You should define KBD_ROWS and KBD_COLS for KBD_D_MATRIX!
#endif

#if defined(KBD_D_MATRIX) && \
    (((KBD_COLS >= 1) && (!defined(KBD_PIN_C1))) \
  || ((KBD_COLS >= 2) && (!defined(KBD_PIN_C2))) \
  || ((KBD_COLS >= 3) && (!defined(KBD_PIN_C3))) \
  || ((KBD_COLS >= 4) && (!defined(KBD_PIN_C4))))
#error You should define proper amount of LCD_PIN_Cx for KBD_COLS of KBD_D_MATRIX!
#endif

#if defined(KBD_D_MATRIX) && \
    (((KBD_ROWS >= 1) && (!defined(KBD_PIN_R1))) \
  || ((KBD_ROWS >= 2) && (!defined(KBD_PIN_R2))) \
  || ((KBD_ROWS >= 3) && (!defined(KBD_PIN_R3))) \
  || ((KBD_ROWS >= 4) && (!defined(KBD_PIN_R4))))
#error You should define proper amount of LCD_PIN_Rx for KBD_ROWS of KBD_D_MATRIX!
#endif

#if defined(KBD_A_JOYSTICK) && \
  (!defined(KBD_PIN_X) || !defined(KBD_PIN_Y) \
  || !defined(KBD_PIN_BTN))
#error You should define KBD_PIN_X, KBD_PIN_Y and KBD_PIN_BTN for KBD_A_JOYSTICK!
#endif

#if defined(KBD_A_KEYPAD) && \
  (!defined(KBD_PIN_DATA))
#error You should define LCD_PIN_DATA for KBD_A_KEYPAD!
#endif

#if defined(KBD_A_KEYPAD) && \
  (!defined(KBD_DATA_NONE_MIN) || !defined(KBD_DATA_NONE_MAX) || (KBD_DATA_NONE_MIN > KBD_DATA_NONE_MAX) \
  || !defined(KBD_DATA_UP_MIN) || !defined(KBD_DATA_UP_MAX) || (KBD_DATA_UP_MIN > KBD_DATA_UP_MAX) \
  || !defined(KBD_DATA_DOWN_MIN) || !defined(KBD_DATA_DOWN_MAX) || (KBD_DATA_DOWN_MIN > KBD_DATA_DOWN_MAX) \
  || !defined(KBD_DATA_LEFT_MIN) || !defined(KBD_DATA_LEFT_MAX) || (KBD_DATA_LEFT_MIN > KBD_DATA_LEFT_MAX) \
  || !defined(KBD_DATA_RIGHT_MIN) || !defined(KBD_DATA_RIGHT_MAX) || (KBD_DATA_RIGHT_MIN > KBD_DATA_RIGHT_MAX) \
  || !defined(KBD_DATA_SELECT_MIN) || !defined(KBD_DATA_SELECT_MAX) || (KBD_DATA_SELECT_MIN > KBD_DATA_SELECT_MAX))
#error You should define proper ranges KBD_DATA_*_MIN and KBD_DATA_*_MAX for KBD_A_KEYPAD!
#endif

#if defined(KBD_ROTARY_ENCODER) && \
  (!defined(KBD_PIN_D1) || !defined(KBD_PIN_D2) \
  || !defined(KBD_PIN_BTN))
#error You should define KBD_PIN_D1, KBD_PIN_D2 and KBD_PIN_BTN for KBD_ROTARY_ENCODER!
#endif

#if defined(KBD_A_MATRIX) && \
  (!defined(KBD_ROWS) || !defined(KBD_COLS))
#error You should define KBD_ROWS and KBD_COLS for KBD_A_MATRIX!
#endif

#if defined(KBD_A_MATRIX) && \
  (!defined(KBD_PIN_DATA))
#error You should define LCD_PIN_DATA for KBD_A_MATRIX!
#endif

#if defined(KBD_A_MATRIX) && \
  (!defined(KBD_DATA_TOLERANCE) || !defined(KBD_DATA_ARRAY))
#error You should define proper KBD_DATA_TOLERANCE and KBD_DATA_ARRAY for KBD_A_MATRIX!
#endif

#if defined(LCD_I2C_RGB)
#include <Adafruit_RGBLCDShield.h>
extern Adafruit_RGBLCDShield lcd; /* defined in lcd.cpp */
#endif

/*! \brief Initialization of keyboard
 */
void kbd_init() {
#if defined(KBD_NONE)
  /* nothig to do here */
#elif defined(KBD_D_MATRIX)
#if KBD_COLS >= 1
  pinMode(KBD_PIN_C1, OUTPUT);
#if KBD_COLS >= 2
  pinMode(KBD_PIN_C2, OUTPUT);
#if KBD_COLS >= 3
  pinMode(KBD_PIN_C3, OUTPUT);
#if KBD_COLS >= 4
  pinMode(KBD_PIN_C4, OUTPUT);
#endif
#endif
#endif
#endif
#if KBD_ROWS >= 1
  pinMode(KBD_PIN_R1, INPUT_PULLUP);
#if KBD_ROWS >= 2
  pinMode(KBD_PIN_R2, INPUT_PULLUP);
#if KBD_ROWS >= 3
  pinMode(KBD_PIN_R3, INPUT_PULLUP);
#if KBD_ROWS >= 4
  pinMode(KBD_PIN_R4, INPUT_PULLUP);
#endif
#endif
#endif
#endif
#elif defined(KBD_A_JOYSTICK)
  pinMode(KBD_PIN_BTN, INPUT_PULLUP);
#elif defined(KBD_A_KEYPAD)
  /* No initialization for analog pins needed */
#elif defined(KBD_ROTARY_ENCODER)
  pinMode(KBD_PIN_D1, INPUT_PULLUP);
  pinMode(KBD_PIN_D2, INPUT_PULLUP);
  pinMode(KBD_PIN_BTN, INPUT_PULLUP);
  /* TODO: use attachInterrupt() and ISR if/when we have bad read timing! */
#elif defined(KBD_A_MATRIX)
  /* No initialization for analog pins needed */
#elif defined(KBD_I2C_RGB)
  /* The RGB Keypad is already initialized in lcd.cpp */
#endif
}

/*! \brief Read current key
 */
uint8_t kbd_getkey() {
#if defined(KBD_NONE)
  /* No keys to read */
#elif defined(KBD_D_MATRIX)
  uint8_t cols[] = {
#if KBD_COLS >= 1
    KBD_PIN_C1,
#if KBD_COLS >= 2
    KBD_PIN_C2,
#if KBD_COLS >= 3
    KBD_PIN_C3,
#if KBD_COLS >= 4
    KBD_PIN_C4,
#endif
#endif
#endif
#endif
  };
  uint8_t rows[] = {
#if KBD_ROWS >= 1
    KBD_PIN_R1,
#if KBD_ROWS >= 2
    KBD_PIN_R2,
#if KBD_ROWS >= 3
    KBD_PIN_R3,
#if KBD_ROWS >= 4
    KBD_PIN_R4,
#endif
#endif
#endif
#endif
  };
  for(uint8_t c = 0; c < KBD_COLS; c++) {
    delayMicroseconds(5);
    for(uint8_t i = 0; i < KBD_COLS; i++) {
      digitalWrite(cols[i], i == c? LOW: HIGH);
    }
    delayMicroseconds(5);
    for(uint8_t r = 0; r < KBD_ROWS; r++) {
      uint16_t v = analogRead(rows[r]);
      if(v < 100) {
        uint8_t x = 0;
        x |= c << 4;
        x |= 1 << r;
        for(uint8_t i = 0; i < KBD_COLS; i++) {
          digitalWrite(cols[i], HIGH);
        }
        return x;
      }
    }
  }
  for(uint8_t i = 0; i < KBD_COLS; i++) {
    digitalWrite(cols[i], HIGH);
  }
#elif defined(KBD_A_JOYSTICK)
  uint16_t x = analogRead(KBD_PIN_X);
  uint16_t y = analogRead(KBD_PIN_Y);
  uint8_t btn = digitalRead(KBD_PIN_BTN);
  if(btn == LOW)
    return KBD_KEY_SELECT;
  if(x < (KBD_X_CENTER - KBD_THRESHOLD))
    return KBD_KEY_LEFT;
  if(x > (KBD_X_CENTER + KBD_THRESHOLD))
    return KBD_KEY_RIGHT;
  if(y < (KBD_Y_CENTER - KBD_THRESHOLD))
    return KBD_KEY_DOWN;
  if(y > (KBD_Y_CENTER + KBD_THRESHOLD))
    return KBD_KEY_UP;
#elif defined(KBD_A_KEYPAD)
  uint16_t data = analogRead(KBD_PIN_DATA);
  if(data >= KBD_DATA_UP_MIN && data < KBD_DATA_UP_MAX)
    return KBD_KEY_UP;
  if(data >= KBD_DATA_DOWN_MIN && data < KBD_DATA_DOWN_MAX)
    return KBD_KEY_DOWN;
  if(data >= KBD_DATA_LEFT_MIN && data < KBD_DATA_LEFT_MAX)
    return KBD_KEY_LEFT;
  if(data >= KBD_DATA_RIGHT_MIN && data < KBD_DATA_RIGHT_MAX)
    return KBD_KEY_RIGHT;
  if(data >= KBD_DATA_SELECT_MIN && data < KBD_DATA_SELECT_MAX)
    return KBD_KEY_SELECT;
#elif defined(KBD_ROTARY_ENCODER)
/*action| full left | full right| left&back | right&back|
  ------+-----------+-----------+-----------+-----------+
  !d1   | 0 1 1 0 0 | 0 0 1 1 0 | 0 1 1 1 0 | 0 0 1 0 0 |
  !d2   | 0 0 1 1 0 | 0 1 1 0 0 | 0 0 1 0 0 | 0 1 1 1 0 |
  d     | 0 1 3 2 0 | 0 2 3 1 0 | 0 1 3 1 0 | 0 2 3 2 0 |
  sum   | 0 1 4[6]0 | 0 2 5[6]0 | 0 1 4[5]0 | 0 2 5[7]0 |
  p_sum | ? 0 1[4]6 | ? 0 2[5]6 | ? 0 1 4 5 | ? 0 2 5 7 |
  ------+-----------+-----------+-----------+-----------+
  check |correct sum|correct sum| wrong sum!| wrong sum!|
  answer| p_sum = 4 | p_sum = 5 |  ignore!  |  ignore!  |*/
  uint8_t d1 = !digitalRead(KBD_PIN_D1);
  uint8_t d2 = !digitalRead(KBD_PIN_D2);
  uint8_t btn = digitalRead(KBD_PIN_BTN);
  uint8_t d = (d1 << 0) | (d2 << 1);
  static uint8_t p_d = 0;
  static uint8_t sum = 0;
  static uint8_t p_sum = 0;
  if(d != p_d) { /* do we have a state change? */
    p_d = d;
    if(d != 0) { /* are we still turning? */
      p_sum = sum;
      sum += d;
    } else { /* we have completed the turn */
      if(sum == 6) { /* do we have correct sum value? (see table) */
        sum = 0;
        if(p_sum == 4) /* check direction (see table) */
          return KBD_KEY_LEFT;
        if(p_sum == 5) /* check direction (see table) */
          return KBD_KEY_RIGHT;
      }
      /* something went wrong? -> ignore! */
      sum = 0;
    }
  }
  if(btn == LOW)
    return KBD_KEY_SELECT;
#elif defined(KBD_A_MATRIX)
  static const uint16_t data_array[KBD_ROWS * KBD_COLS] = KBD_DATA_ARRAY;
  uint16_t data = analogRead(KBD_PIN_DATA);
  for(uint8_t c = 0 ; c < KBD_COLS; c++)
    for(uint8_t r = 0 ; r < KBD_ROWS; r++) {
      if(data >= data_array[r * KBD_COLS + c] - KBD_PIN_DATA &&
        data <= data_array[r * KBD_COLS + c] + KBD_PIN_DATA) {
        uint8_t x = 0;
        x |= c << 4;
        x |= 1 << r;
        return x;
      }
    }
#elif defined(KBD_I2C_RGB)
  uint8_t buttons = lcd.readButtons();
  if(buttons & BUTTON_UP)
    return KBD_KEY_UP;
  if(buttons & BUTTON_DOWN)
    return KBD_KEY_DOWN;
  if(buttons & BUTTON_LEFT)
    return KBD_KEY_LEFT;
  if(buttons & BUTTON_RIGHT)
    return KBD_KEY_RIGHT;
  if(buttons & BUTTON_SELECT)
    return KBD_KEY_SELECT;
#endif
  return KBD_KEY_NONE;
}