/*
	Esp32 Websockets Client

	This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server
        3. Sends the websockets server a message ("Hello Server")
        4. Prints all incoming messages while the connection is open

	Hardware:
        For this sketch you only need an ESP32 board.

	Created 15/02/2019
	By Gil Maimon
	https://github.com/gilmaimon/ArduinoWebsockets

*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>

const char* ssid = "Eldert Lounge"; //Enter SSID
const char* password = "308Loung3"; //Enter Password
const char* websockets_server_host = "192.168.106.153"; //Enter server adress
const uint16_t websockets_server_port = 8081; // Enter server port
String receivedData = "Hello";

using namespace websockets;

WebsocketsClient client;
void setup() {
  Serial.begin(115200);
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Connected to Wifi, Connecting to server.");
  // try to connect to Websockets server
  delay(1000);
  bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
  if (connected) {
    Serial.println("Connected!");
    client.send("Hello");
  } else {
    Serial.println("Not Connected!");
    delay(1000);
  }

  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
    Serial.println(message.data().toInt());
  });
  }



void loop() {
  // let the websockets client check for incoming messages
  if (client.available()) {
    client.poll();
  }
  delay(500);
}
