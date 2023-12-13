#include <ezButton.h>  // the library to use for SW pin
#include "U8glib.h"
#include <Wire.h>
#include <IRremote.h>

#define CLK_PIN 13
#define DT_PIN 12
#define SW_PIN 11

#define DIRECTION_CW 0
#define DIRECTION_CCW 1

int counter = 10;
int direction = DIRECTION_CW;
int CLK_state;
int prev_CLK_state;
int IR_SENDER = 3;

uint8_t timer[14] { 0x1 , 0x2 , 0x3 , 0x4 , 0x5 , 0x6 , 0x7 , 0x8 , 0x9 , 0x10 , 0x11 , 0x12 , 0x13 , 0x14 };

ezButton button(SW_PIN);
IRsend IrSender;

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  oled.setFont(u8g_font_helvB10);

  pinMode(CLK_PIN, INPUT);  // configure encoder pins as inputs
  pinMode(DT_PIN, INPUT);
  button.setDebounceTime(50);  // set debounce time to 50 milliseconds

  prev_CLK_state = digitalRead(CLK_PIN);  // read the initial state of the rotary encoder's CLK pin
}

void loop() {
  button.loop();  // MUST call the loop() function first

  // read the current state of the rotary encoder's CLK pin
  CLK_state = digitalRead(CLK_PIN);

  // If the state of CLK is changed, then pulse occurred
  // React to only the rising edge (from LOW to HIGH) to avoid double count
  if (CLK_state != prev_CLK_state && CLK_state == HIGH) {
    // if the DT state is HIGH
    // the encoder is rotating in counter-clockwise direction => decrease the counter
    if (digitalRead(DT_PIN) == HIGH) {
      counter--;
      direction = DIRECTION_CCW;
    } else {
      // the encoder is rotating in clockwise direction => increase the counter
      counter++;
      direction = DIRECTION_CW;
    }

    if (counter > 14) {
      counter = 14;
    }
    else if (counter < 0) {
      counter = 1;
    }
    updateoled(String (counter));
    delay(10);
  }

  prev_CLK_state = CLK_state;  // save last CLK state

  if (button.isReleased()) {
    Serial.println("The button is pressed");
    sendIRSignal(timer[counter-1], 32);
  }
}

void updateoled(String text) {
  oled.firstPage();
  do {
    oled.drawStr(55, 40, text.c_str());
  } while (oled.nextPage());
}

void sendIRSignal(unsigned long irCode, int bits) {
  IrSender.sendNEC(irCode, bits);  // Change to the appropriate protocol and bits
  delay(100);
  IrSender.sendNEC(irCode, bits);
  delay(100);
  IrSender.sendNEC(irCode, bits);

  delay(1000);  // Delay between signals (adjust if needed)
}
