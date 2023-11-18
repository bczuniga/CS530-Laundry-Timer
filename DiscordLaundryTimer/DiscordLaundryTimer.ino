/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using the WiFi module.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 Circuit:
 * Board with NINA module (Arduino MKR WiFi 1010, MKR VIDOR 4000 and Uno WiFi Rev.2)

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */


#include <SPI.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>
#include <Timer.h>

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "discord.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;

float zeroX, zeroY, zeroZ, newX, newY, newZ;
float zeroR, newR;
Timer time;

void setup() {
  for(int i = 6; i < 10; i++)
    pinMode(i, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  for(int i = 6; i < 10; i++)
    digitalWrite(i, LOW);
  digitalWrite(6, HIGH);
  IMU.begin();
  connectToWiFi();
  IMU.readAcceleration(zeroX, zeroY, zeroZ);
  zeroR = sqrt(zeroX*zeroX + zeroY*zeroY + zeroZ*zeroZ);
  digitalWrite(6, LOW);
}

void loop() {
  float newR = sqrt(newX*newX + newY*newY + newZ*newZ);
  IMU.readAcceleration(newX, newY, newZ);
  if (time.state() == RUNNING) {
    Serial.print(time.read());
    Serial.print("\t");
    Serial.println(abs(zeroR-newR));
    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
  }
  if (abs(newR-zeroR) >= 0.0025) {
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

  if (time.read() >= 300000) {
    time.pause();
  }

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
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
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

    // wait 10 seconds for connection:
    delay(10000);
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
    digitalWrite(LED_BUILTIN, HIGH);
  }
  Serial.println("Connected to WiFi");
  printWifiStatus();
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
}

void postWebhook() {
    if (client.connect(server, 443)) {
    String postBody = "{\"content\" : \"Washer is finished!\"}";

    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("POST https://discord.com/api/webhooks/1174468441269354577/M9YyWOpfK2TY0ULpsNlE5JXBP-HBB4QiOamKgURO7OHVvXTB_rkvOumCPEXaZfz5GDX1 HTTP/1.1");
    client.println("Host: discord.com");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(postBody.length());
    client.println("Connection: close");
    client.println();
    client.println(postBody);
  }
}