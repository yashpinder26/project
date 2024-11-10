#include <WiFiNINA.h>
#include <NewPing.h>
#include <AccelStepper.h>

#define TRIG_PIN 5
#define ECHO_PIN 6
#define MAX_DISTANCE 200

#define STEP_PIN 4
#define DIR_PIN 3

const char* ssid = "pavilion";           // Your Wi-Fi SSID
const char* password = "Yashu750$";      // Your Wi-Fi password
const char* serverIP = "192.168.137.251"; // Raspberry Pi 5 IP address
const int serverPort = 5000;

WiFiClient client;

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  stepper.enableOutputs();
  stepper.setMaxSpeed(600);
  stepper.setAcceleration(300);
}

void sendCaptureRequest() {
  if (client.connect(serverIP, serverPort)) {
    client.println("GET /trigger HTTP/1.1");
    client.println("Host: " + String(serverIP));
    client.println("Connection: close");
    client.println();
    client.stop();
  } else {
    Serial.println("Failed to connect to server.");
  }
}

void loop() {
  unsigned int distance = sonar.ping_cm();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < 20) {
    Serial.println("Object detected! Running motor");
    delay(1000);
    stepper.setSpeed(-300);

    unsigned long startTime = millis();
    while (millis() - startTime < 2050) {
      stepper.runSpeed();
    }

    Serial.println("Stopping motor.");

    // Send HTTP GET request to Raspberry Pi
    sendCaptureRequest();

    delay(500); 
  }
}
