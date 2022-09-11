/*
 * 
 * CH55x Macropad Example
 * Based on HIDkeyboard example (https://github.com/DeqingSun/ch55xduino)
 * 
 */


#ifndef USER_USB_RAM
#error "This example needs to be compiled with a USER USB setting"
#endif

#include "src/userUsbHidKeyboard/USBHIDKeyboard.h"

/*
 * Pin Assignment
 * 
 * ROW A (Input) :P1.4/14
 * ROW B (Input) :P1.5/15
 * ROW C (Input) :P1.6/16
 * ROW D (Input) :P1.7/17
 * 
 * COL a (Output):P3.1/31
 * COL b (Output):P3.2/32
 * COL c (Output):P3.3/33
 * COL d (Output):P3.4/34
 * 
 */

#define ROW_A 14
#define ROW_B 15
#define ROW_C 16
#define ROW_D 17

#define COL_a 31
#define COL_b 32
#define COL_c 33
#define COL_d 34

const uint8_t colTable[4] = {COL_a,COL_b,COL_c,COL_d};
const char keymap[16] = {
  'z','w','5','7',
  'x','3','t','u',
  'q','e','6','i',
  '2','r','y','p'
  };

bool keyPrev[16];
bool keyCurrent[16];

void selectCol(uint8_t col){
  switch(col){
    case COL_a:
      digitalWrite(COL_a,LOW);
      digitalWrite(COL_b,HIGH);
      digitalWrite(COL_c,HIGH);
      digitalWrite(COL_d,HIGH);
      break;
    case COL_b:
      digitalWrite(COL_a,HIGH);
      digitalWrite(COL_b,LOW);
      digitalWrite(COL_c,HIGH);
      digitalWrite(COL_d,HIGH);
      break;
    case COL_c:
      digitalWrite(COL_a,HIGH);
      digitalWrite(COL_b,HIGH);
      digitalWrite(COL_c,LOW);
      digitalWrite(COL_d,HIGH);
      break;
    case COL_d:
      digitalWrite(COL_a,HIGH);
      digitalWrite(COL_b,HIGH);
      digitalWrite(COL_c,HIGH);
      digitalWrite(COL_d,LOW);
      break;
    default:
      digitalWrite(COL_a,HIGH);
      digitalWrite(COL_b,HIGH);
      digitalWrite(COL_c,HIGH);
      digitalWrite(COL_d,HIGH);            
  }
}

void setup() {
  uint8_t i;
  
  USBInit();
  
  //Row
  pinMode(ROW_A, INPUT_PULLUP);
  pinMode(ROW_B, INPUT_PULLUP);
  pinMode(ROW_C, INPUT_PULLUP);
  pinMode(ROW_D, INPUT_PULLUP);
  
  //Column  
  pinMode(COL_a, OUTPUT);
  pinMode(COL_b, OUTPUT);
  pinMode(COL_c, OUTPUT);
  pinMode(COL_d, OUTPUT);

  selectCol(COL_a);

  for(i = 0;i < 16;i++){
    keyPrev[i] = false;
    keyCurrent[i] = false;
  }
}

void loop() {
  uint8_t i;

  //Read key status
  for(i = 0;i < 4;i++){
    selectCol(colTable[i]);
    delay(1);
    keyCurrent[i * 4]     = !digitalRead(ROW_A);
    keyCurrent[i * 4 + 1] = !digitalRead(ROW_B);
    keyCurrent[i * 4 + 2] = !digitalRead(ROW_C);
    keyCurrent[i * 4 + 3] = !digitalRead(ROW_D);
  }

  //Check press/release
  for(i = 0;i < 15;i++){
    if(keyPrev[i] != keyCurrent[i]){
      if(keyCurrent[i] == true){
        Keyboard_press(keymap[i]);
      }
      else{
        Keyboard_release(keymap[i]);
      }
      keyPrev[i] = keyCurrent[i];
    }
  }
  
  delay(50);  //naive debouncing

}
