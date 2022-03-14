#include QMK_KEYBOARD_H

#include "wait.h"
#include "quantum.h"
#include "v99slim.h"
#include "pointing_device.h"
//#include "transactions.h"
//#include "transport.h"
//#include "transaction_id_define.h"

void process_mouse(report_mouse_t* mouse_report);
void process_mouse_user(report_mouse_t* mouse_report, int16_t x, int16_t y);
void process_wheel(report_mouse_t* mouse_report);
void process_wheel_user(report_mouse_t* mouse_report, int16_t h, int16_t v);

#define V99_DPI_OPTIONS { 1, 2, 3, 4, 5, 6 }
#define V99_DPI_MASTER_DEFAULT 0
#define V99_DPI_SLAVE_DEFAULT 2
#ifndef V99_DRAGSCROLL_DPI
#define V99_DRAGSCROLL_DPI 1 // Fixed-DPI Drag Scroll
#endif

uint16_t dpi_array[] = V99_DPI_OPTIONS;
#define DPI_OPTION_SIZE (sizeof(dpi_array) / sizeof(uint16_t))

static int8_t V99_X_TRANSFORM_M = -1;
static int8_t V99_Y_TRANSFORM_M =  1;

//static int8_t V99_X_TRANSFORM_S = -5;
//static int8_t V99_Y_TRANSFORM_S =  5;

typedef union {
  uint32_t raw;
  struct {
    uint8_t v99_dpi_master;
    uint8_t v99_dpi_slave;
  };
} user_config_t;

static user_config_t user_config;

static int8_t wheelh = 0;
static int8_t wheelv = 0;

#define V99_WHEEL_H_DIV 10
#define V99_WHEEL_V_DIV 20

static bool is_drag_scroll_m = 0;
//static bool is_drag_scroll_s = 0;

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 16

enum custom_keycodes {
#ifdef VIA_ENABLE
  QWERTY = USER00,
#else
  QWERTY = SAFE_RANGE,
#endif
  LOWER,
  RAISE,
  ADJUST,
  DPI_CONFIG_M,
  DPI_CONFIG_S,
  DRAG_SCROLL_M,
  DRAG_SCROLL_S,
  V99_SAFE_RANGE,
};

//-------------------------------------------------------------------------------------

#define _MAIN 0
#define _FN 1

#define KC_CAPW LGUI(LSFT(KC_3))        // Capture whole screen
#define KC_CPYW LGUI(LSFT(LCTL(KC_3)))  // Copy whole screen
#define KC_CAPP LGUI(LSFT(KC_4))        // Capture portion of screen
#define KC_CPYP LGUI(LSFT(LCTL(KC_4)))  // Copy portion of screen
#define KC_X0 LT(_FN, KC_ESC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT(
    KC_A, KC_B, KC_C,
    KC_D, KC_E, KC_F
  ),

  [_LOWER] = LAYOUT(
    KC_A, KC_B, KC_C,
    KC_D, KC_E, KC_F
  ),

  [_RAISE] = LAYOUT(
    KC_A, KC_B, KC_C,
    KC_D, KC_E, KC_F
  ),

  [_ADJUST] = LAYOUT(
    KC_A, KC_B, KC_C,
    KC_D, KC_E, KC_F
  )
};

//-------------------------------------------------------------------------------------
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
         print("mode just switched to qwerty and this is a huge string\n");
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;

    case DPI_CONFIG_M:
      if (record->event.pressed) {
          user_config.v99_dpi_master = (user_config.v99_dpi_master + 1) % DPI_OPTION_SIZE;
          eeconfig_update_kb(user_config.raw);
          //adns_set_cpi(dpi_array[user_config.v99_dpi_master]);
          V99_X_TRANSFORM_M = dpi_array[user_config.v99_dpi_master] * -1;
          V99_Y_TRANSFORM_M = dpi_array[user_config.v99_dpi_master] *  1;
      }
      return false;
      break;

    case DPI_CONFIG_S:
      if (record->event.pressed) {
    //      user_config.v99_dpi_slave = (user_config.v99_dpi_slave + 1) % DPI_OPTION_SIZE;
    //      eeconfig_update_kb(user_config.raw);
    //      V99_X_TRANSFORM_S = dpi_array[user_config.v99_dpi_slave] * -1;
    //      V99_Y_TRANSFORM_S = dpi_array[user_config.v99_dpi_slave] *  1;
      }
      return false;
      break;

		case DRAG_SCROLL_M:
#ifndef V99PAD_DRAGSCROLL_MOMENTARY
		  if (record->event.pressed)
#endif
			{
				is_drag_scroll_m ^= 1;
			}
      wheelh = 0;
      wheelv = 0;
      return false;
			break;

    case DRAG_SCROLL_S:
//#ifndef V99PAD_DRAGSCROLL_MOMENTARY
//		  if (record->event.pressed)
//#endif
//			{
//				is_drag_scroll_s ^= 1;
//			}
//      wheelh = 0;
//      wheelv = 0;
      return false;
			break;

  }
  return true;
}


__attribute__((weak)) void process_mouse_user(report_mouse_t* mouse_report, int16_t x, int16_t y) {

    //mouse_report->x = x;
    //mouse_report->y = y;

}

float xytrans(uint8_t data,int8_t ts){

  int8_t tmp = (int8_t) data;
  float odata = data * ts;

  if(abs(ts)>2){

    if(abs(tmp)<2){
      //odata = data * 2;
      if(ts > 0 ){
         odata = data *  3;
      }else{
         odata = data * -3;
      }
    }else if(abs(tmp)<5){
      if(ts > 0 ){
         odata = data *  4;
      }else{
         odata = data * -4;
      }
    }

  }
  return odata;
}

float htrans(uint8_t h){
  float i = 0;

  wheelh += h;
  if( wheelh != 0 ){
    i = wheelh / V99_WHEEL_H_DIV;
    wheelh = wheelh % V99_WHEEL_H_DIV;
    if(i != 0){
      wheelv = 0;
    }
  }
  return i;
}

float vtrans(uint8_t v){
  float i = 0;

  wheelv += v;
  if( wheelv != 0 ){
    i = wheelv / V99_WHEEL_V_DIV;
    wheelv = wheelv % V99_WHEEL_V_DIV;
    if(i != 0){
      wheelh = 0;
    }
  }
  return i;
}

__attribute__((weak)) void process_mouse(report_mouse_t* mouse_report) {

	  float xt = 0;
	  float yt = 0;
    float ht = 0;
    float vt = 0;

    //bool isslavexydata = false;

    report_v99_t data = v99_slim_read_burst();

    if(!is_drag_scroll_m){
      xt = xytrans(data.dx, V99_X_TRANSFORM_M);
      yt = xytrans(data.dy, V99_Y_TRANSFORM_M);
      ht = 0;
      vt = 0;
    }else{
      xt = 0;
      yt = 0;
      ht = htrans(data.dx);
      vt = vtrans(data.dy);      
    }

    

    

#if 0
    if (is_keyboard_master()){  // keyboard master
      if(is_drag_scroll_m == is_drag_scroll_s){ // same mode

        if(!is_drag_scroll_m){ // xy mode
          if(data.dx == 0 && data.dy == 0){ // slave data

            //xt = (float) s2m_x * (float) V99_X_TRANSFORM_S;
            //yt = (float) s2m_y * (float) V99_Y_TRANSFORM_S;
            xt = xytrans(s2m_x, V99_X_TRANSFORM_S);
            yt = xytrans(s2m_y, V99_Y_TRANSFORM_S);
            //ht = 0;
            //vt = 0;

          }else{  // master data

            //xt = (float) data.dx * (float) V99_X_TRANSFORM_M;
            //yt = (float) data.dy * (float) V99_Y_TRANSFORM_M;
            xt = xytrans(data.dx, V99_X_TRANSFORM_M);
            yt = xytrans(data.dy, V99_Y_TRANSFORM_M);
            //ht = 0;
            //vt = 0;
            //if(abs(data.dx)>1 || abs(data.dy)>1){
            //  xt = (float) data.dx * (float) V99_X_TRANSFORM_M;
            //  yt = (float) data.dy * (float) V99_Y_TRANSFORM_M;
            //}else{
        		//	xt = (float) data.dx * -1;
            //  yt = (float) data.dy;
        		//}
          }
        }else{ // hv mode
          if(data.dx == 0 && data.dy == 0){ // slave data
            ht = htrans(s2m_x);
            vt = vtrans(s2m_y);
          }else{  // master data
            ht = htrans(data.dx);
            vt = vtrans(data.dy);
          }
        }

      }else{ // different mode

        if(!is_drag_scroll_m){ // m=xy, s=hv
          xt = xytrans(data.dx, V99_X_TRANSFORM_M);
          yt = xytrans(data.dy, V99_Y_TRANSFORM_M);

          ht = htrans(s2m_x);
          vt = vtrans(s2m_y);
        }else{  // m=hv, s=xy
          xt = xytrans(s2m_x, V99_X_TRANSFORM_S);
          yt = xytrans(s2m_y, V99_Y_TRANSFORM_S);

          ht = htrans(data.dx);
          vt = vtrans(data.dy);
        }

      }

      s2m_x = 0;
      s2m_y = 0;
      data.dx = 0;
      data.dy = 0;

    }else{ // keyboard slave
      // send xy value to keyboard master
      s2m_x = data.dx;
      s2m_y = data.dy;

      xt = data.dx;
      yt = data.dy;
      ht = 0;
      vt = 0;
    }
#endif

    mouse_report->h = (int16_t)ht * 1;
    mouse_report->v = (int16_t)vt * 1;
    mouse_report->x = (int16_t)xt * -1;
    mouse_report->y = (int16_t)yt * -1;

    //mouse_report->x = (int16_t)data.dx;
    //mouse_report->y = (int16_t)data.dy;
    data.dx = 0;
    data.dy = 0;

    //int16_t xti = (int16_t)xt;
    //int16_t yti = (int16_t)yt;
    //process_mouse_user(mouse_report, xti, yti);

}

void pointing_device_init(void) {
    v99_slim_init();

    v99_slim_write(REG_CHIP_RESET, 0x5A);

    // wait maximum time before v99 sensor is ready.
    // this ensures that the v99 sensor is actuall ready after reset.
    wait_ms(55);

    // read a burst from the v99 sensor and then discard it.
    // gets the v99 sensor ready for write commands
    // (for example, setting the powerdown mode).
    v99_slim_read_burst();

    //wait_us(30);
    // set the powerdown mode.
    //v99_slim_write(REG_MOUSE_CONTROL, 0x01); // default 0x00
}

void pointing_device_task(void) {

    report_mouse_t mouse_report = pointing_device_get_report();
    //process_wheel(&mouse_report);
    process_mouse(&mouse_report);

    pointing_device_set_report(mouse_report);
    pointing_device_send();
}
#if 1


void eeconfig_init_user(void) {  // EEPROM is getting reset!
  user_config.raw = 0;
  user_config.v99_dpi_master = V99_DPI_MASTER_DEFAULT; // We want this enabled by default
  user_config.v99_dpi_slave = V99_DPI_SLAVE_DEFAULT;
  eeconfig_update_user(user_config.raw); // Write default value to EEPROM now

}

void matrix_init_user(void){
  dprintf("init MS XY transform value \n");
  eeconfig_init_user();
}

void keyboard_post_init_user(void) {
    debug_enable=true;
    debug_matrix=true;
    //transaction_register_rpc(USER_SYNCXY, user_sync_a_slave_handler);

    user_config.raw = eeconfig_read_user();

    V99_X_TRANSFORM_M = (float)dpi_array[user_config.v99_dpi_master] * (float)-1;
    V99_Y_TRANSFORM_M = (float)dpi_array[user_config.v99_dpi_master] * (float)1;
    //V99_X_TRANSFORM_S = (float)dpi_array[user_config.v99_dpi_slave]  * (float)-1;
    //V99_Y_TRANSFORM_S = (float)dpi_array[user_config.v99_dpi_slave]  * (float)1;

}



#endif