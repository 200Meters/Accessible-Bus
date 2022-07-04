/*
 * Adafruit NeoPixel use guide is at https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
 * Adafruit API library for the NeoPixel is at https://adafruit.github.io/Adafruit_NeoPixel/html/class_adafruit___neo_pixel.html#ae7c444138dd0e6ac1008621d0a67048b
 * Adafruit Huzzah use guide is at https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/pinouts
 * Require adafruit Neopixel by Adafruit library to be installed for the LED board
 * RGB Color codes were found at https://www.ditig.com/256-colors-cheat-sheet
 * WiFi communication example can be found at https://siytek.com/communication-between-two-esp8266/
 */


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

 
// Set AP credentials
#define AP_SSID "TheOtherESP"
#define AP_PASS "flashmeifyoucan"
 
// UDP
WiFiUDP UDP;
IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);
#define UDP_PORT 4210
 
// UDP Buffer
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

// Define pin that controls the NeoPixel
#define LED_PIN 15

//Define how many lights are on the NeoPixel
#define LED_COUNT 32

//Define how bright to make the lights
#define BRIGHTNESS 150

// Define delay between pixels in milliseconds
#define DELAY_VAL 5

// Create pixels object to represent the NeoPixel board
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {

  // Initialize NeoPixel
  pixels.begin();

  // Set brightness of LED. Do it one time during setup
  pixels.setBrightness(BRIGHTNESS);
   
  // Setup serial port
  Serial.begin(115200); //Make sure your COM is set to communicate at this speed
  Serial.println();
 
  // Begin Access Point
  Serial.println("Starting access point...");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println(WiFi.localIP());
 
  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);

  // Report setup is complete
  Serial.println("Setup Complete");
 
}
 
void loop() {
  // Receive UDP packet
  UDP.parsePacket();
  UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
  if (packetBuffer[0]){
    // If magnet is not close, color the LED green to indicate seat is available
    for(int i=0; i<LED_COUNT; i++) { // Go through each LED, set the color, and turn it on
      pixels.setPixelColor(i, pixels.Color(0,255,0)); // Set the color to Green
      pixels.show(); // Send the color to the pixels and light them up
      delay(DELAY_VAL); // Short delay between light coloring
    }
    
    Serial.println("Magnet is not close - seat is available");
  } else {
    //If magnet is close, color the LED red to indicate seat is taken
    for(int i=0; i<LED_COUNT; i++) { // Go through each LED, set the color, and turn it on
      pixels.setPixelColor(i, pixels.Color(255,0,0)); // Set the color to red
      pixels.show(); // Send the color to the pixels and light them up
      delay(DELAY_VAL); // Short delay between light coloring
    }
    
    Serial.println("Magnet is close - seat is not aviailable"); // This is when the magnet is close 
  }      

 
  
 
}
