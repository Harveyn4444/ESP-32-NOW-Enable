// REPLACE WITH THE MAC Address of your receiver 
//MAC address - 70:B8:F6:5C:03:84 Rocket
//MAC address - 34:86:5D:3B:1C:34 Station
//uint8_t broadcastAddress[] = {0x70, 0xB8, 0xF6, 0x5C, 0x03, 0x84}; //This is being sent from the station
//uint8_t broadcastAddress[] = {0x34, 0x86, 0x5D, 0x3B, 0x1C, 0x34}; //This is being sent from the rocket
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0x34, 0x86, 0x5D, 0x3B, 0x1C, 0x34}; //This is being sent from the rocket

char Rocketdata[32];
String ReceivedText; 

typedef struct struct_message {
  char Station[32];
  char Rocket[32];
  int b;
  float c;
  bool d;
} struct_message;

struct STATION_DATA_STRUCTURE{
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  char Station[32];
  //May change to a bool data type
};
STATION_DATA_STRUCTURE StationData;
struct ROCKET_DATA_STRUCTURE{
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  char Rocket[32];
    //May change to a bool data type
};
ROCKET_DATA_STRUCTURE RocketData;

//struct_message Outgoing;
//struct_message Incoming;

String success;

esp_now_peer_info_t peerInfo;
// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received - What to do when it recieves data
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  //    incomingReadings struct, incoming data is created in the line before - also size
  memcpy(&StationData, incomingData, sizeof(StationData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  //The struct read  
  Serial.println( StationData.Station);
  Serial.println();
  ReceivedText = String(StationData.Station);
}


void setup() {
  Serial.begin(115200);
  Serial.println("START Station");

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);  
  
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);  
}

void loop() {
  
//if stationdata is "ARM"  
//send "Rocket is armed"
/*
  if(StationData.Station == "ARM"){
    strcpy(RocketData.Rocket, "Rocket is ARMED");
  } else if (StationData.Station == "DISARM"){
    strcpy(RocketData.Rocket, "Rocket is DISARM");
  } else {
    strcpy(RocketData.Rocket, "TEST");
    Serial.println("Char Pass Error");
  }
*/
  if(ReceivedText == "ARM"){
    strcpy(RocketData.Rocket, "Rocket is ARMED");
  } else if (ReceivedText == "DISARM"){
    strcpy(RocketData.Rocket, "Rocket is DISARM");
  } else {
    strcpy(RocketData.Rocket, "TEST");
    Serial.println("Char Pass Error");
  }

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &RocketData, sizeof(RocketData)); 
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10000);
  
 
}



