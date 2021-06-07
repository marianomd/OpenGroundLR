
#ifndef KEYS_H_
#define KEYS_H_

#include <stdint.h>
#include <libopencm3/stm32/gpio.h>

#define KEYS_MATRIX_LINES_GPIO          GPIOD
#define KEYS_MATRIX_COLUMNS_GPIO        GPIOC

// columns
#define KEYS_MATRIX_R1_PIN              GPIO6
#define KEYS_MATRIX_R2_PIN              GPIO7
#define KEYS_MATRIX_R3_PIN              GPIO8

// rows
#define KEYS_MATRIX_L1_PIN              GPIO12
#define KEYS_MATRIX_L2_PIN              GPIO13
#define KEYS_MATRIX_L3_PIN              GPIO14
#define KEYS_MATRIX_L4_PIN              GPIO15

#define KEYS_BIND_GPIO                  GPIOF
#define KEYS_BIND_PIN                   GPIO2

#define KEYS_COLUMNS_PINS               (KEYS_MATRIX_R1_PIN | KEYS_MATRIX_R2_PIN | KEYS_MATRIX_R3_PIN)
#define KEYS_LINES_PINS                 (KEYS_MATRIX_L1_PIN | KEYS_MATRIX_L2_PIN | KEYS_MATRIX_L3_PIN | KEYS_MATRIX_L4_PIN)


// Keys driver
enum EnumKeys
{
  KEY_MENU,
  KEY_ENTER=KEY_MENU,
  KEY_EXIT,
  KEY_DOWN,
  KEY_MINUS = KEY_DOWN,
  KEY_UP,
  KEY_PLUS = KEY_UP,
  KEY_RIGHT,
  KEY_LEFT,
  KEY_BIND,
  TRM_BASE,
  TRM_LH_DWN = TRM_BASE,
  TRM_LH_UP,
  TRM_LV_DWN,
  TRM_LV_UP,
  TRM_RV_DWN,
  TRM_RV_UP,
  TRM_RH_DWN,
  TRM_RH_UP,
  TRM_LAST = TRM_RH_UP,
  NUM_KEYS
};

#define KEY_MATRIX_LINES 4
#define KEY_MATRIX_COLUMNS 3

static const uint16_t columns[] = {
    KEYS_MATRIX_R1_PIN,
    KEYS_MATRIX_R2_PIN,
    KEYS_MATRIX_R3_PIN};

static const uint16_t lines[] = {
    KEYS_MATRIX_L1_PIN,
    KEYS_MATRIX_L2_PIN,
    KEYS_MATRIX_L3_PIN,
    KEYS_MATRIX_L4_PIN};

/*
      R1	    R2	        R3
  L1	Roll R	Throttle U	Down
  L2	Roll L	Throttle D	Up
  L3	Pitch U	Yaw R	    OK
  L4	Pitch D	Yaw L	    Cancel

*/

void keysInit(void);
uint8_t keyState(uint8_t index);
uint32_t readKeys(void);
uint32_t readTrims(void);
uint32_t scanMatrix(uint32_t columnStart, uint32_t columnEnd);
uint8_t trimDown(uint8_t idx);
bool keyDown(void);
void readKeysAndTrims(void);
void keys_test(void);

#endif  // KEYS_H_