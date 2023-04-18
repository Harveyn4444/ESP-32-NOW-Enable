
#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
struct STATION_DATA_STRUCTURE{
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  char Station[32];
  //May change to a bool data type
};
STATION_DATA_STRUCTURE StationData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&StationData, incomingData, sizeof(StationData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(StationData.Station);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("START Rocket");
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}