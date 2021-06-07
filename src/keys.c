/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "keys.h"
#include "main.h"
#include "console.h"
#include "debug.h"
#include "delay.h"
#include "wdt.h"

#include <libopencm3/stm32/rcc.h>

const uint8_t keysMap[] = {
    TRM_RH_UP, TRM_RH_DWN, TRM_RV_UP, TRM_RV_DWN,
    TRM_LV_UP, TRM_LV_DWN, TRM_LH_UP, TRM_LH_DWN,
    KEY_DOWN, KEY_UP, KEY_ENTER, KEY_EXIT};

uint8_t key_state[NUM_KEYS];

uint32_t scanMatrix(uint32_t columnStart, uint32_t columnEnd)
{
  uint32_t result = 0;
  uint8_t column = 0;
  uint8_t line = 0;
  uint16_t index = columnStart * KEY_MATRIX_LINES;
  for (column = columnStart; column <= columnEnd; column++)
  {
    //set to low
    gpio_clear(KEYS_MATRIX_COLUMNS_GPIO, columns[column]);

    //read lines
    for (line = 0; line < KEY_MATRIX_LINES; line++)
    {
      if ((gpio_port_read(KEYS_MATRIX_LINES_GPIO) & lines[line]) == 0)
      {
        result |= (1 << ((uint32_t)keysMap[index]));
      }
      index++;
    }
    // Set to high
    gpio_set(KEYS_MATRIX_COLUMNS_GPIO, columns[column]);
  }
  return result;
}

uint32_t readKeys()
{
  uint32_t result = scanMatrix(2, 2);
  //bind active low
  if ((gpio_port_read(KEYS_BIND_GPIO) & KEYS_BIND_PIN) == 0)
  {
    if (!result)
    {
      result |= 1 << KEY_BIND;
    }
    else
    {
      //bind as shift
      if (result & (1 << KEY_DOWN))
      {
        result &= ~(1 << KEY_DOWN);
        result |= 1 << KEY_LEFT;
      }
      if (result & (1 << KEY_UP))
      {
        result &= ~(1 << KEY_UP);
        result |= 1 << KEY_RIGHT;
      }
    }
  }
  return result;
}

uint32_t readTrims()
{
  uint32_t result = scanMatrix(0, 1);
  // TRACE("readTrims(): result=0x%02x", result);
  return result;
}

uint8_t trimDown(uint8_t idx)
{
  return readTrims() & (1 << idx);
}

bool keyDown()
{
  return readKeys() || readTrims();
}

void readKeysAndTrims()
{
  uint8_t index = 0;
  uint32_t in = readKeys();
  for (uint8_t i = 1; i != (uint8_t)(1 << TRM_BASE); i <<= 1)
  {
    key_state[index++] = (in & i) > 0;
  }

  in = readTrims();
  for (uint8_t i = 1; i != (uint8_t)(1 << 8); i <<= 1)
  {
    key_state[index++] = (in & i) > 0;
  }
}

uint8_t keyState(uint8_t index)
{
  return key_state[index];
}

void keysInit()
{
  rcc_periph_clock_enable(GPIO_RCC(KEYS_MATRIX_COLUMNS_GPIO));
  rcc_periph_clock_enable(GPIO_RCC(KEYS_MATRIX_LINES_GPIO));
  rcc_periph_clock_enable(GPIO_RCC(KEYS_BIND_GPIO));

  // Setup rows GPIOS as input
  gpio_mode_setup(KEYS_MATRIX_LINES_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEYS_LINES_PINS);
  gpio_set_output_options(KEYS_MATRIX_LINES_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYS_LINES_PINS);

  // Setup columns GPIOS as output
  gpio_mode_setup(KEYS_MATRIX_COLUMNS_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, KEYS_COLUMNS_PINS);
  gpio_set_output_options(KEYS_MATRIX_COLUMNS_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEYS_COLUMNS_PINS);

  // Set to high
  gpio_set(KEYS_MATRIX_COLUMNS_GPIO, KEYS_COLUMNS_PINS);
}

void keys_test(void)
{
  uint32_t keys;
  uint32_t trims;

  while (1)
  {
    console_clear();
    debug("KEYS TEST\n");
    debug_put_newline();
    keys = readKeys();
    trims = readTrims();
    debug("KEYS: ");
    debug_put_hex32(keys);
    debug_put_newline();
    debug("TRIMS: ");
    debug_put_hex32(trims);
    debug_put_newline();
    debug("KEYS TEST END\n");
    debug_put_newline();
    debug_flush();
    wdt_reset();
    delay_ms(50);
  }
}