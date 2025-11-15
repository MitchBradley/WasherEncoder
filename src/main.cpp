#include <Arduino.h>
#include <Esp.h>
#include <Wire.h>
#include <Adafruit_MCP23X08.h>

Adafruit_MCP23X08 mcp;

const uint8_t MCP_ADDR = 0x25;

const int ledPin = LED_BUILTIN; // GPIO2 on ESP8266 boards

const int buttonPin = 0;
const int pin8 = 16;
const int pin4 = 14;
const int pin2 = 12;
const int pin1 = 13;

bool previousButtonState = LOW;
int sequenceNumber = 0;
int sequence[10] = {
    1,  // Cold wash
    3,  // Rinse
    11, // Self clean
    10, // Sanitize
    2,  // Bulky
    6,  // Towels
    14, // Whites
    15, // Normal
    4,  // Quick wash
    9,  // Delicates
};
void setState(uint8_t relays, uint8_t leds = 0)
{
  uint8_t value = (relays << 3) | leds;
  Serial.println("Setting state to: " + String(value, BIN));
  mcp.writeGPIO(value, 0);
}
void advance()
{
  ++sequenceNumber;
#if 1
  sequenceNumber %= 10;
  setState(sequence[sequenceNumber]);
#else
  sequenceNumber %= 16;
  setState(sequenceNumber);
#endif
}
void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Washer Encoder Starting...");
  pinMode(buttonPin, INPUT);
  for (uint8_t addr = 0x20; addr <= 0x28; addr++)
  {
    if (mcp.begin_I2C(addr))
    {
      Serial.println("Found MCP23008 at address 0x" + String(MCP_ADDR, HEX));
      break;
    }
    if (addr == 0x28)
    {
      Serial.println("Could not find MCP23008.");
      while (1)
        yield();
    }
    yield();
  }
  for (int i = 0; i < 8; i++)
  {
    mcp.pinMode(i, OUTPUT);
  }
  digitalWrite(ledPin, HIGH);

  // set initial to Normal
  setState(sequence[sequenceNumber]);
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
      delay(10);
      // yield();
      // ESP.wdtFeed();
    }
    digitalWrite(ledPin, HIGH);

    delay(80); // debounce
  }
  delay(10);
  // yield();

  // ESP.wdtFeed();
}