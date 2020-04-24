

#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 4
#define NUMPIXELS 64
#define PIXELTYPE NEO_GRB + NEO_KHZ800

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, PIXELTYPE);


const char* ssid = "Eldert Lounge"; //Enter SSID
const char* password = "308Loung3"; //Enter Password
const char* websockets_server_host = "192.168.106.153"; //Enter server adress
const uint16_t websockets_server_port = 8081; // Enter server port


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
    pixels.begin();
    pixels.setBrightness(50);
    pixels.show(); // Initialize all pixels to 'off'
    Serial.println("Connected!");
    //client.send("Hello from ESP32");
  } else {
    Serial.println("Not Connected!");
    delay(1000);
  }
  rainbow(40);

  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message) {
    Serial.print("Got Message: ");


    Serial.println(message.data());
    client.send("Got the tap!");

    theaterChaseRainbow(50);

  });
  rainbow(20);
}



void loop() {
  // let the websockets client check for incoming messages
  if (client.available()) {
    client.poll();
  }

}
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < pixels.numPixels(); i = i + 3) {
        pixels.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i = 0; i < pixels.numPixels(); i = i + 3) {
        pixels.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    //The lower the number, the more red it is
    // When it gets higher, Blue starts going upwards and Red goes down
    // When Wheelpos is 0, (255,0,0), when it's 85, (0,0,255)
    //RED TO BLUE
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    //BLUE TO GREEN
    //0, (0,0,255) // 170, (0,255,0)
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  //GREEN TO RED
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
