/*
@author Brylle Isaiah Villoso
@author Brendel Zuniga
@title CS 530 Systems Programming Project, Discord Laundry Timer
@date December 11, 2023
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>
#include <Timer.h>

// file that contains Wi-Fi credentials
#include "arduino_secrets.h" 
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
char server[] = "discord.com";    // Discord host name

// Insert server webhook
String webhookLink = "https://discord.com/api/webhooks/1174468441269354577/M9YyWOpfK2TY0ULpsNlE5JXBP-HBB4QiOamKgURO7OHVvXTB_rkvOumCPEXaZfz5GDX1";

// Initialize Wi-Fi client to interface with HTTPS requests for webhooks
WiFiSSLClient client;

// values defined at program start as the zero value
float zeroX, zeroY, zeroZ, newX, newY, newZ;
float zeroR, newR;
Timer time;

/** LED Legend:
LED_BUILTIN: Blinking = Wi-Fi module not detected, incompatible board
9 - Yellow - Wi-Fi Status
    - Slow blink: Attempting to connect to Wi-Fi
    - Solid: Connection to Wi-Fi successful
8 - Blue:    Timer in-progress
7 - Green:   Timer complete, notification sent
6 - Red:     Setup in-progress
*/

void setup() {
  // set relevant pins to output
  for(int i = 6; i < 10; i++)
    pinMode(i, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // set all LEDs to LOW
  for(int i = 6; i < 10; i++)
    digitalWrite(i, LOW);
  
  // Set Red LED to HIGH to indicate start of setup
  digitalWrite(6, HIGH);

  // Initialize IMU (LSM6DSL for Nano 33 IoT)
  IMU.begin();

  // Connect to Wi-Fi and send start notification
  connectToWiFi();

  // Calibrate and zero-out values
  IMU.readAcceleration(zeroX, zeroY, zeroZ);
  zeroR = sqrt(zeroX*zeroX + zeroY*zeroY + zeroZ*zeroZ);

  // Set Red LED to LOW to indicate setup complete
  digitalWrite(6, LOW);
}

void loop() {
  // take current accelerator values, calculate position from zero
  newR = sqrt(newX*newX + newY*newY + newZ*newZ);
  IMU.readAcceleration(newX, newY, newZ);

  // flash out status
  if (time.state() == RUNNING) {
    Serial.print(time.read());
    Serial.print("\t");
    Serial.println(abs(zeroR-newR));
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
  }

  // if the difference of new and zero radius is greater than threshold (currently 0.0025)
  // reset timer
  // blink Blue to indicate reset
  if (abs(newR-zeroR) >= 0.0025) {
    // Serial output testing
    // Serial.print("X = ");
    // Serial.print(oldX);
    // Serial.print("\t");
    // Serial.print("Y = ");
    // Serial.print(oldY);
    // Serial.print("\t");
    // Serial.print("Z = ");
    // Serial.println(oldZ);
    time.start();
    digitalWrite(8, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

  // if no movement detected for X amount of minutes (currently set to 5 min)
  // indicate timer end
  if (time.read() >= 300000) {
    time.pause();
  }

  // if timer ended, post webhook, set Green to HIGH and Blue to LOW
  // end of laundry timer until next reset
  if (time.state() == PAUSED) {
    Serial.println("Laundry is finished!");
    Serial.println(time.read());
    postWebhook();
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    time.stop();
    while(true);
  }

}

// for use in debugging
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connectToWiFi() {
  // initialize serial and wait for port to open:
  Serial.begin(9600);

  // check for the Wi-Fi module:
  // if no Wi-Fi module detected, indicate connection error
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with Wi-Fi module failed!");
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // flash Wi-Fi status
    for (int i = 0; i < 3; i++) {
      digitalWrite(9, HIGH);
      delay(250);
      digitalWrite(9, LOW);
      delay(250);
    }

    // wait 10 seconds for connection:
    delay(10000);
  }
  digitalWrite(9, HIGH);
  Serial.println("Connected to WiFi");
  printWifiStatus();
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
}

void postWebhook() {
  // connect to host server defined at beginning of code
  // port = 80 for HTTP
  // port = 443 for HTTPS
  if (client.connect(server, 443)) {
    String postBody = "{\"content\" : \"Washer is finished!\"}";

    Serial.println("connected to server");
    // Make a HTTPS POST request:
    // sends notification to discord webhook
    client.println("POST " + webhookLink + " HTTP/1.1");
    client.println("Host: discord.com");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(postBody.length());
    client.println("Connection: close");
    client.println();
    client.println(postBody);
  }
}