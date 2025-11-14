#include <Arduino.h>
#include <Esp.h>

const int ledPin = LED_BUILTIN; // GPIO2 on ESP8266 boards

const int buttonPin = 0;
const int pin8 = 16;
const int pin4 = 14;
const int pin2 = 12;
const int pin1 = 13;

bool previousButtonState = LOW;
int sequenceNumber = 0;
int sequence[10] = {
    0,  // Quick wash
    9,  // Delicates
    8,  // Cold wash
    10, // Rinse
    11, // Self clean
    3,  // Sanitize
    2,  // Bulky
    6,  // Towels
    7,  // Whites
    15, // Normal
};
void setState(int state)
{
  int value = sequence[state];
  digitalWrite(pin8, value & 8 ? HIGH : LOW);
  digitalWrite(pin4, value & 4 ? HIGH : LOW);
  digitalWrite(pin2, value & 2 ? HIGH : LOW);
  digitalWrite(pin1, value & 1 ? HIGH : LOW);
}
void advance()
{
  sequenceNumber = (sequenceNumber + 1) % 10;
  Serial.print("Washing machine set to sequence: ");
  Serial.println(sequence[sequenceNumber]);
  setState(sequenceNumber);
}
void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(pin8, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin1, OUTPUT);

  // set initial to Normal
  setState(9);
}
void loop()
{
  // read button state, HIGH when pressed, LOW when not
  int buttonState = digitalRead(buttonPin);

  // if the push button pressed, switch on the LED
  if (buttonState == LOW)
  {

    digitalWrite(ledPin, LOW);
    advance();
    // wait for button release
    while (digitalRead(buttonPin) == LOW)
    {
      yield();
      ESP.wdtFeed();
    }
    digitalWrite(ledPin, HIGH);

    delay(50); // debounce
  }
  delay(10);
  yield();

  ESP.wdtFeed();
}