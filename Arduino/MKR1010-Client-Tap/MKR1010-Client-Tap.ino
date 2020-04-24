
//LIS
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

//WIFI
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h> /*Need to implement*/
//https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/SimpleWebSocket/SimpleWebSocket.ino
//https://www.arduino.cc/en/Tutorial/WiFiNINAConnectWithWPA


char ssid[] = "Eldert Lounge";        // your network SSID (name)
char pass[] = "308Loung3";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status
char serverAddress[] = "192.168.106.153";  // server address
int port = 8081;

WiFiClient wifi;

WebSocketClient client = WebSocketClient(wifi, serverAddress, port);

// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();


// Adjust this number for the sensitivity of the 'click' force
// this strongly depend on the range! for 16G, try 5-10
// for 8G, try 10-20. for 4G try 20-40. for 2G try 40-80
#define CLICKTHRESHHOLD 80

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);


  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 1 second for connection:
    delay(1000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();


  lis.setRange(LIS3DH_RANGE_2_G);

  // 0 = turn off click detection & interrupt
  // 1 = single click only interrupt output
  // 2 = double click only interrupt output, detect single click
  // Adjust threshhold, higher numbers are less sensitive
  lis.setClick(2, CLICKTHRESHHOLD);
}

void loop() {
  // check the network connection once every 10 seconds:
  delay(1000);

  Serial.println("starting WebSocket client");
  client.begin();
  Serial.print("Is client connected? -> ");
  Serial.println(client.connected());

  while (client.connected()) {
    uint8_t click = lis.getClick();
    if (click == 0) return;
    if (! (click & 0x30)) return;
    Serial.print("Click detected (0x"); Serial.print(click, HEX); Serial.print("): ");
    while (click & 0x10) {
      Serial.print(" single click");

      // send tap to esp32
      client.beginMessage(TYPE_TEXT);
      client.print("real tap");
      client.endMessage();
      delay(10000);
      
    }

  }


}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

}
