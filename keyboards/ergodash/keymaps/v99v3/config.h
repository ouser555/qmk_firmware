/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

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

#pragma once

//#define USB_POLLING_INTERVAL_MS 16
/* Use I2C or Serial, not both */

#define USE_SERIAL
// #define USE_I2C

/* Select hand configuration */

#define MASTER_LEFT
//#define MASTER_RIGHT
//#define EE_HANDS
#define SPLIT_USB_DETECT

#define DYNAMIC_KEYMAP_LAYER_COUNT 4

//#define SERIAL_USE_MULTI_TRANSACTION
#define SPLIT_TRANSACTION_IDS_USER USER_SYNCXY

#define VIAL_KEYBOARD_UID {0x89, 0x10, 0x0B, 0x23, 0x91, 0xA6, 0x88, 0x5E}

#define VIAL_UNLOCK_COMBO_ROWS { 0, 2 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 3 }
//#define VIAL_COMBO_ENTRIES 1
