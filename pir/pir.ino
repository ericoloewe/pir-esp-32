

#include "WiFi.h" // ESP32 WiFi include
#include "esp_wpa2.h"

#define EAP_IDENTITY "0118340@feevale.br"
#define EAP_PASSWORD "160996"

const uint16_t port = 9999;
const char * host = "10.47.18.131";
// const char* ssid = "eduroam"; // Eduroam SSID
const char* ssid     = "DESKTOP-LPD8U10 5875";
const char* password = "deboas123";
int counter = 0;

int motionSensorPort = 27;
WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  connect();
  establishContact();
  attachInterrupt(digitalPinToInterrupt(motionSensorPort), detectsMovement, RISING);
}

void loop() {
  if (!client.connect(host, port)) {
    Serial.println("Connection to host failed");

    delay(1000);
    return;
  }

  int sensorValue = digitalRead(motionSensorPort);

  Serial.print("VALOR ATUAL: ");
  Serial.print(digitalRead(motionSensorPort));
  Serial.print("\n");
  client.print(sensorValue);
  delay(1000);
}

void establishContact() {
  int count = 0;

  while (Serial.available() <= 0) {
    Serial.print("Carregando ");
    Serial.print(count++);
    Serial.print("\n");
    delay(300);
  }
}

void detectsMovement() {
  Serial.println("Movimento detectado!!!");
}

void connect()//Sub-rotina para verificar a conexao com o host.
{
  WiFi.mode(WIFI_STA);//Define o ESP32 como Station.
  Serial.print("Connecting to network: ");

  if (WiFi.status() != WL_CONNECTED)//Caso nao esteja conectado ao host, ira se conectar.
  {
    WiFi.begin(ssid, password);//Conecta à rede do host.

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void connectToWiFiWithEduroam(const char * ssid)
{
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.println(ssid);

  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username --> identity and username is same
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
  esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
  WiFi.begin(ssid); //connect to wifi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;

    if (counter >= 60) { //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
}
