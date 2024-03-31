#include <WiFi.h>
#include <WiFiClient.h>
#include <array>

const char* ssid = "FAMILIA MONTOY"; // Name WiFi Network
const char* password = "N12JP16SR"; // Password WiFi Network
const char* serverIP = "192.168.0.8"; // Server IP 
const int serverPort = 12345; // Server Port

const int PIN_RED = 27;
const int PIN_YELLOW = 14;
const int PIN_GREEN = 12;

//Creation of LED class
class LED {
private:
    int pin;
public:
    LED(int pin) : pin(pin) {
        pinMode(pin, OUTPUT);
    }
    void turn_On_or_OFF(bool value) {
        digitalWrite(pin, value);
    }
};

void internet_Connection() {
    Serial.println("Conectando a WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Conexion exitosa");
}

std::array<int, 3> server_Connection_actuator() {
    // Conecction to server
    WiFiClient client;
    if (!client.connect(serverIP, serverPort)) {
        Serial.println("Error al conectar con el servidor");
        delay(1000);
    }
    client.println("get");
    // Wait and read server answer
    while (client.available() == 0) {
        delay(1);
    }
    String response = client.readStringUntil('\n');
    // Eliminate [ ]
    response.replace("[", "");
    response.replace("]", "");
    // Divide answer in tokens separated by comas
    std::array<int, 3> array_actuators;
    int index = 0;
    char* token = strtok((char*)response.c_str(), ",");
    while (token != NULL && index < 3) {
        array_actuators[index++] = atoi(token);
        token = strtok(NULL, ",");
    }
    return array_actuators;
}

void manageLEDs(std::array<int, 3> array_actuators, LED& red, LED& yellow, LED& green) {
    red.turn_On_or_OFF(array_actuators[0]);
    yellow.turn_On_or_OFF(array_actuators[1]);
    green.turn_On_or_OFF(array_actuators[2]);
}

void setup() {
    Serial.begin(9600);
    internet_Connection();
}

void loop() {
    LED red(PIN_RED), yellow(PIN_YELLOW), green(PIN_GREEN);
    std::array<int, 3> array_actuators = server_Connection_actuator();
    manageLEDs(array_actuators, red, yellow, green);
}
