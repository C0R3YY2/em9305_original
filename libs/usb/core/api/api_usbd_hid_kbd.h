/***************************************************************************
 *
 *            Copyright (c) 2009-2020 by HCC Embedded
 *
 * This software is copyrighted by and is the sole property of
 * HCC.  All rights, title, ownership, or other interests
 * in the software remain the property of HCC.  This
 * software may only be used in accordance with the corresponding
 * license agreement.  Any unauthorized use, duplication, transmission,
 * distribution, or disclosure of this software is expressly forbidden.
 *
 * This Copyright notice may not be removed or modified without prior
 * written consent of HCC.
 *
 * HCC reserves the right to modify this software without notice.
 *
 * HCC Embedded
 * Budapest 1133
 * Vaci ut 76
 * Hungary
 *
 * Tel:  +36 (1) 450 1302
 * Fax:  +36 (1) 450 1303
 * http: www.hcc-embedded.com
 * email: info@hcc-embedded.com
 *
 ***************************************************************************/
#ifndef _API_USBD_HID_KBD_H_
#define _API_USBD_HID_KBD_H_

#include "../psp/include/psp_types.h"
#include "api_usbd_hid.h"

#include "../version/ver_usbd_hid.h"
#if VER_USBD_HID_MAJOR != 8 || VER_USBD_HID_MINOR != 7
 #error "Incorrect HID class-driver version."
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* List of key scan codes. */
#define KEY_NONE                            0x00
#define KEY_ERRORROLLOVER                   0x01
#define KEY_POSTFAIL                        0x02
#define KEY_ERRORUNDEFINED                  0x03
#define KEY_A                               0x04
#define KEY_B                               0x05
#define KEY_C                               0x06
#define KEY_D                               0x07
#define KEY_E                               0x08
#define KEY_F                               0x09
#define KEY_G                               0x0A
#define KEY_H                               0x0B
#define KEY_I                               0x0C
#define KEY_J                               0x0D
#define KEY_K                               0x0E
#define KEY_L                               0x0F
#define KEY_M                               0x10
#define KEY_N                               0x11
#define KEY_O                               0x12
#define KEY_P                               0x13
#define KEY_Q                               0x14
#define KEY_R                               0x15
#define KEY_S                               0x16
#define KEY_T                               0x17
#define KEY_U                               0x18
#define KEY_V                               0x19
#define KEY_W                               0x1A
#define KEY_X                               0x1B
#define KEY_Y                               0x1C
#define KEY_Z                               0x1D
#define KEY_1_EXCLAMATION_MARK              0x1E
#define KEY_2_AT                            0x1F
#define KEY_3_NUMBER_SIGN                   0x20
#define KEY_4_DOLLAR                        0x21
#define KEY_5_PERCENT                       0x22
#define KEY_6_CARET                         0x23
#define KEY_7_AMPERSAND                     0x24
#define KEY_8_ASTERISK                      0x25
#define KEY_9_OPARENTHESIS                  0x26
#define KEY_0_CPARENTHESIS                  0x27
#define KEY_ENTER                           0x28
#define KEY_ESCAPE                          0x29
#define KEY_BACKSPACE                       0x2A
#define KEY_TAB                             0x2B
#define KEY_SPACEBAR                        0x2C
#define KEY_MINUS_UNDERSCORE                0x2D
#define KEY_EQUAL_PLUS                      0x2E
#define KEY_OBRACKET_AND_OBRACE             0x2F
#define KEY_CBRACKET_AND_CBRACE             0x30
#define KEY_BACKSLASH_VERTICAL_BAR          0x31
#define KEY_NONUS_NUMBER_SIGN_TILDE         0x32
#define KEY_SEMICOLON_COLON                 0x33
#define KEY_SINGLE_AND_DOUBLE_QUOTE         0x34
#define KEY_GRAVE_ACCENT_AND_TILDE          0x35
#define KEY_COMMA_AND_LESS                  0x36
#define KEY_DOT_GREATER                     0x37
#define KEY_SLASH_QUESTION                  0x38
#define KEY_CAPS_LOCK                       0x39
#define KEY_F1                              0x3A
#define KEY_F2                              0x3B
#define KEY_F3                              0x3C
#define KEY_F4                              0x3D
#define KEY_F5                              0x3E
#define KEY_F6                              0x3F
#define KEY_F7                              0x40
#define KEY_F8                              0x41
#define KEY_F9                              0x42
#define KEY_F10                             0x43
#define KEY_F11                             0x44
#define KEY_F12                             0x45
#define KEY_PRINTSCREEN                     0x46
#define KEY_SCROLL_LOCK                     0x47
#define KEY_PAUSE                           0x48
#define KEY_INSERT                          0x49
#define KEY_HOME                            0x4A
#define KEY_PAGEUP                          0x4B
#define KEY_DELETE                          0x4C
#define KEY_END1                            0x4D
#define KEY_PAGEDOWN                        0x4E
#define KEY_RIGHTARROW                      0x4F
#define KEY_LEFTARROW                       0x50
#define KEY_DOWNARROW                       0x51
#define KEY_UPARROW                         0x52
#define KEY_KEYPAD_NUM_LOCK_AND_CLEAR       0x53
#define KEY_KEYPAD_SLASH                    0x54
#define KEY_KEYPAD_ASTERIKS                 0x55
#define KEY_KEYPAD_MINUS                    0x56
#define KEY_KEYPAD_PLUS                     0x57
#define KEY_KEYPAD_ENTER                    0x58
#define KEY_KEYPAD_1_END                    0x59
#define KEY_KEYPAD_2_DOWN_ARROW             0x5A
#define KEY_KEYPAD_3_PAGEDN                 0x5B
#define KEY_KEYPAD_4_LEFT_ARROW             0x5C
#define KEY_KEYPAD_5                        0x5D
#define KEY_KEYPAD_6_RIGHT_ARROW            0x5E
#define KEY_KEYPAD_7_HOME                   0x5F
#define KEY_KEYPAD_8_UP_ARROW               0x60
#define KEY_KEYPAD_9_PAGEUP                 0x61
#define KEY_KEYPAD_0_INSERT                 0x62
#define KEY_KEYPAD_DECIMAL_SEPARATOR_DELETE 0x63
#define KEY_NONUS_BACK_SLASH_VERTICAL_BAR   0x64
#define KEY_APPLICATION                     0x65

/* Modifier keys for kbd_set_modifier. */
#define MODIFIER_LEFT_CTRL                  ( 1ul << 0 )
#define MODIFIER_LEFT_SHIFT                 ( 1ul << 1 )
#define MODIFIER_LEFT_ALT                   ( 1ul << 2 )
#define MODIFIER_LEFT_GUI                   ( 1ul << 3 )
#define MODIFIER_RIGHT_CTRL                 ( 1ul << 4 )
#define MODIFIER_RIGHT_SHIFT                ( 1ul << 5 )
#define MODIFIER_RIGHT_ALT                  ( 1ul << 6 )
#define MODIFIER_RIGHT_GUI                  ( 1ul << 7 )

/* LED mask. */
#define KBDLED_NUMLOCK                      ( 1ul << 0 )
#define KBDLED_CAPSLOCK                     ( 1ul << 1 )
#define KBDLED_SCROLLOCK                    ( 1ul << 2 )
#define KBDLED_COMPOSE                      ( 1ul << 3 )
#define KBDLED_KANA                         ( 1ul << 4 )


#define USBD_KBD_LED_CB                     0
#define USBD_KBD_IN_CB                      1

int usbd_kbd_init ( void );
int usbd_kbd_start ( void );
int usbd_kbd_stop ( void );
int usbd_kbd_delete ( void );

int usbd_kbd_register_ntf ( uint8_t unit, t_usbd_hid_ntf ntf, t_usbd_hid_ntf_fn ntf_fn, uint32_t ntf_fn_param );

void usbd_kbd_set_modifier ( uint8_t unit, uint8_t modifier );
int  usbd_kbd_set_key_state ( uint8_t unit, uint8_t key, uint8_t state );
int usbd_kbd_set_report ( uint8_t unit, uint8_t modifier, uint8_t * p_keys, uint8_t key_count );
void usbd_kbd_clear_report ( uint8_t unit );
int usbd_kbd_get_led_state ( uint8_t unit, uint8_t * p_led_state );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _API_USBD_HID_KBD_H_ */
