# CS530-Laundry-Timer

## How-To
[Product Demonstration](https://youtu.be/teNc_26mirE)

### Components Needed:
- Board with Wi-Fi NINA Module:
  - Arduino MKR WiFi 1010;
  - Arduino MKR VIDOR 4000;
  - Arduino Uno WiFi Rev. 2; or
  - Nano 33 IoT (what we used for testing)
- Micro-USB to USB-A/USB-C for Power
- A way to secure board to washer/dryer.

### Setup:
1. Download [DiscordLaundryTimer.ino](DiscordLaundryTimer/DiscordLaundryTimer.ino) and Arduino_Secrets.h (example file can be found [here](DiscordLaundryTimer/arduino_secrets.h.example.h)).
2. Download Dependencies listed below or [here](Source Code/libraries).
3. Modify `arduino_secrets.h` with Wi-Fi credentials
4. Modify `DiscordLaundryTimer.ino` `webhookLink` field with Discord server webhook link
5. (Optional) Calibrate thresholds to your specific machine
6. Upload code to board

### Dependencies:
- ArduinoBLE
- Arduino_LSM6DS3
- FastIMU
- Timer
- WiFiNINA

### Using the device
Once you've completed the setup steps above, you can now start using the Discord Laundry Timer:
1. Load washer/dryer with clothes
2. Attach the board to a safe place on washer/dryer.
3. Connect board to power
4. Wait for Discord notification that timer has started
5. Start washer/dryer

## [Source Code](DiscordLaundryTimer/DiscordLaundryTimer.ino)
```c++
#include <SPI.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>
#include <Timer.h>

#include "arduino_secrets.h"

char[] ssid[] = SECRET_SSID
char[] pass = SECRET_PASS
int keyIndex = 0
int status = WL_IDLE_STATUS

char server[] = "discord.com"

WIFISSLClient client;

float zeroX, zeroY, zeroZ, newX, newY, newZ;
float zeroR, newR;
Timer time;

void setup() {
  for (int i = 6; i < 10; i++) 
    Set up pins to allow writing to them
  Set up built-in LED
  for (int i = 6; i < 10; i++)
    write to all external LEDs to not light up
  Write to one external LED to light up to signal that it is connecting to the internet
  connectoWifi()
  Set the zero for the accelerometer
  Write to the previous external LED earlier to signal that it finished connecting to the internet
}

void loop() {
  Calculate the movement the arduino is experiencing
  if (time.state() == RUNNING) {
    Set one LED to light up to signal that the timer is active
    Set one LED to off to signal the timer has not reached 5 minutes
  }
  if movement is greater than the threshold, then
    Restart the timer
    Have the previous LED turn off to signal to signal that timer is not active
    Have the built-in LED blink to signal that the arudino is experiencing significant movement.

  if the timer reaches 5 minutes, then
    Pause the timer

  if (time.state() == PAUSED) {
    postWebhook()
    Have the LED that signals the timer has not reached 5 minutes turn on
    Stop the timer
}

void connectToWifi() {
  Check if wifi module exists
  Check if firmware is out of date
  A loop to attempt to connect to wifi
}

void postWebhook() {
  Check if the arduino can communicate to Discord
  String postBody = "{\"content\" : \"Washer is finished!\"}";
  HTTP Request details
}
```
## [Arduino Secrets Header File](DiscordLaundryTimer/arduino_secrets.h.example.h)
```
#define SECRET_SSID ""
#define SECRET_PASS ""
```
