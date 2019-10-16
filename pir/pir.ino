#include "WiFi.h" // ESP32 WiFi include

const uint16_t port = 9999;
const char * host = "192.168.137.1";
const char* ssid     = "DESKTOP-LPD8U10 5875";
const char* password = "deboas123";
int counter = 0;

int motionSensorPort = 27;
WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(motionSensorPort, INPUT);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  connect();
  establishContact();
  // attachInterrupt(digitalPinToInterrupt(motionSensorPort), detectsMovement, CHANGE);
}

void loop() {
  sendInfo();
}

void detectsMovement() {
  Serial.println("Movimento detectado!!!");
  sendInfo();
}

void establishContact() {
  int count = 0;

  while (Serial.available() <= 0) {
    Serial.print("Carregando ");
    Serial.print(count++);
    Serial.print("\n");
    delay(1000);
  }
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

void sendInfo() {
  int sensorValue = digitalRead(motionSensorPort);
  
  Serial.print("Valor atual: ");
  Serial.println(sensorValue);

  Serial.println(WiFi.localIP());

  if (!client.connect(host, port)) {
    Serial.println("Connection to host failed");

    delay(1000);
    return;
  }

  Serial.println("Sending sensor value to IP");
  client.print(sensorValue);
  delay(1000);
}
