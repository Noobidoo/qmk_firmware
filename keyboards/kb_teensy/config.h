#ifndef CONFIG_H
#define CONFIG_H

/* USB Device descriptor parameter */
#define VENDOR_ID 0xFEED
#define PRODUCT_ID 0x6060
#define DEVICE_VER 0x0001

#define MANUFACTURER Noobidoo
#define PRODUCT Custom us
#define DESCRIPTION Custom us handwired

#define LINE_PIN0 PAL_LINE(TEENSY_PIN0_IOPORT, TEENSY_PIN0)

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 18
#define DIODE_DIRECTION COL2ROW

/* key matrix pins */
#define MATRIX_ROW_PINS \
  { LINE_PIN0, LINE_PIN1, LINE_PIN2, LINE_PIN3, LINE_PIN4, LINE_PIN5 }
#define MATRIX_COL_PINS \
  { LINE_PIN6, LINE_PIN7, LINE_PIN8, LINE_PIN9, LINE_PIN10, LINE_PIN11, LINE_PIN12, LINE_PIN14, LINE_PIN15, LINE_PIN16, LINE_PIN18, LINE_PIN19, LINE_PIN20, LINE_PIN21, LINE_PIN22, LINE_PIN23, LINE_PIN24 ,LINE_PIN25}
#define UNUSED_PINS \
  { LINE_PIN13, LINE_PIN26, LINE_PIN17 }

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* disable debug print */
#define NO_DEBUG

/* key combination for command */
#define IS_COMMAND() (keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)))

/* prevent stuck modifiers */
#define PREVENT_STUCK_MODIFIERS

#define RGB_DI_PIN LINE_PIN17
#ifdef RGB_DI_PIN
    #define RGBLIGHT_ANIMATIONS
    #define RGBLED_NUM 60
    #define RGBLIGHT_HUE_STEP 8
    #define RGBLIGHT_SAT_STEP 8
    #define RGBLIGHT_VAL_STEP 8
#endif

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
//#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
//#define LOCKING_RESYNC_ENABLE

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_MACRO
//#define NO_ACTION_FUNCTION

#endif
