/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"

/*
 * scan matrix
 */
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "wait.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

#if (DIODE_DIRECTION == ROW2COL) || (DIODE_DIRECTION == COL2ROW)
static const ioline_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const ioline_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
#endif
#define ROW_SHIFTER ((matrix_row_t)1)

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(uint8_t current_row);
static void init_cols(void);
static void unselect_rows(void);
static void unselect_row(ioline_t row);
static void select_row(ioline_t row);


inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

#define LED_ON()    do { palSetLine (LINE_LED) ;} while (0)
#define LED_OFF()   do { palClearLine(LINE_LED); } while (0)
#define LED_TGL()   do { palToggletLine(LINE_LED); } while (0)

void matrix_init(void)
{
    // initialize row and col
    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    //debug
    debug_matrix = true;
    LED_ON();
    wait_ms(500);
    LED_OFF();
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(row_pins[i]);
        wait_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols(i);
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_row(row_pins[i]);
    }

    if (debouncing) {
        if (--debouncing) {
            wait_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (ROW_SHIFTER<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

/* Column pin configuration
 */
static void  init_cols(void)
{
    // internal pull-up
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
      palSetLineMode(col_pins[i], PAL_MODE_INPUT_PULLUP);
    }
}

/* Returns status of switches(1:on, 0:off) */
static matrix_row_t read_cols(uint8_t current_row)
{
  matrix_row_t rowValue = 0;
  for(uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++) {

    // Select the col pin to read (active low)
    uint8_t line = col_pins[col_index];
    uint8_t pin_state = palReadLine(line);

    // Populate the matrix row with the state of the col pin
    rowValue |=  pin_state==PAL_HIGH ? 0 : (ROW_SHIFTER << col_index);
  }
  return rowValue;
}

/* Row pin configuration
 */
static void unselect_rows(void)
{
      for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        unselect_row(row_pins[i]);
      }
}

/* Row pin configuration
 */
static void unselect_row(ioline_t row)
{
    palSetLineMode(row, PAL_MODE_INPUT);
}

static void select_row(ioline_t row)
{

    palSetLineMode(row, PAL_MODE_OUTPUT_PUSHPULL);
    palClearLine(row);
}
