
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

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(1000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  //printCurrentNet();

    Serial.println("starting WebSocket client");
    client.begin();
    Serial.print("Is client connected? -> ");
    Serial.println(client.connected());

      client.beginMessage(TYPE_TEXT);
      client.print("Fire alarm from arduino mkr1010 - sensor1 ");
      client.endMessage();

    while (client.connected()) {
      Serial.print("Sending fire alarm ");
      // send fire alarm
      client.beginMessage(TYPE_TEXT);
      client.print("1");
      client.endMessage();
    }
    /*
      // check if a message is available to be received
      int messageSize = client.parseMessage();

      if (messageSize > 0) {
        Serial.println("Received a message:");
        Serial.println(client.readString());
      }*/

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

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}
