//ControlTemperature.ino
#include <SPI.h>
#include <WiFi.h>
#include <dht11.h>

dht11 DHT_A,DHT_B,DHT_C;
//#define DHT11_C_PIN 4
#define DHT11_A_PIN 2


char ssid[] = "ASUS1"; //  your network SSID (name) 
char pass[] = "fss150591";    // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;            // your network key Index number (needed only for WEP)
int ledPin = 8;
int status = WL_IDLE_STATUS;
const char* CMD_LEDON = "ledon";
const char* CMD_LEDOFF = "ledoff";
const char* CMD_PING = "ping";

char read_buffer[128];//for client
WiFiServer server(23);

boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
  
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. 
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(1000);
  } 
  // start the server:
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
 }


void loop() {
  int i = 0;
   int chk,chk_2,chk_3;
  chk = DHT_A.read(DHT11_A_PIN); 
  float humi = DHT_A.humidity;
  float temp = DHT_A.temperature;
  
  // wait for a new client:
  WiFiClient client = server.available();
  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clead out the input buffer:
      client.flush();    
     // Serial.println("We have a new client");
      client.println("online"); 
      alreadyConnected = true;
    } 

      while(client.available()) {
        
      
       
       
         char thisClient = client.read();
             if(thisClient == 'l') {
               digitalWrite(ledPin,HIGH);
             }
             else if(thisClient == 'f' ){
               digitalWrite(ledPin,LOW);
             }
           else if(thisClient == 'p' ){
             Serial.println("Command 'ping': reply ok");
             }
       
        server.write(thisClient);
       
      // echo the bytes to the server as well:
        Serial.write(thisClient);
             
      }
   }
   client.println(humi);
   client.println(temp);
  delay(2000); 

}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
