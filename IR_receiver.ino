#include <IRremote.h>
#include <Servo.h>

int irPin = 8;  // IR receiver pin
IRrecv IrReceiver(irPin);
decode_results results;
Servo myservo;

void setup() {
  Serial.begin(9600);
  IrReceiver.enableIRIn();  // Start the IR receiver
  myservo.attach(9);
  myservo.write(0);
}

void loop() {
  if (IrReceiver.decode(&results)) {
    Serial.println(results.value, HEX);  // Print the received code in hexadecimal
    int timer = results.value;  // Store the received code in a variable
    delay(1000);
    myservo.write(95);
    if (timer >= 10) {
      delay(timer * 650);
    }
    else {
      delay(timer * 1000);
    }
    myservo.write(0);
    delay(1000);
    IrReceiver.resume();  // Receive the next value
  }
}
