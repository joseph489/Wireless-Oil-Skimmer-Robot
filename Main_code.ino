
#define BLYNK_TEMPLATE_ID "TMPL3pAYjJUQe"
#define BLYNK_TEMPLATE_NAME "shonless test"
#define BLYNK_AUTH_TOKEN "4OG5bp4ayO6s-OnGadUg3Wat32FjEykP"// Include required libraries
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Blynk Credentials
char auth[] = "4OG5bp4ayO6s-OnGadUg3Wat32FjEykP";
char ssid[] = "0000000000";
char pass[] = "000000000";

// Define Motor Pins
#define IN1 27  // Left Motor Direction 1
#define IN2 26  // Left Motor Direction 2
#define ENA 14  // Left Motor Speed (PWM)

#define IN3 25  // Right Motor Direction 1
#define IN4 4   // Right Motor Direction 2
#define ENB 13  // Right Motor Speed (PWM)

#define DRUM_IN 5  // Drum Motor ON/OFF
#define DRUM_EN 12 // Drum Motor Speed (PWM)

// Define Ultrasonic Sensor Pins
#define TRIG 32
#define ECHO 33

// Variables for motor control
bool leftMotor = false;
bool rightMotor = false;
bool bothMotors = false;
bool drumMotor = false;

// Function to measure distance using Ultrasonic Sensor
float getDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    long duration = pulseIn(ECHO, HIGH);
    float distance = duration * 0.034 / 2;  // Convert to cm
    return distance;
}

// Blynk Virtual Pins (User Inputs)
BLYNK_WRITE(V1) { leftMotor = param.asInt(); }
BLYNK_WRITE(V2) { rightMotor = param.asInt(); }
BLYNK_WRITE(V3) { bothMotors = param.asInt(); }
BLYNK_WRITE(V4) { drumMotor = param.asInt(); }

void setup() {
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass);

    // Set motor control pins as outputs
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);

    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(DRUM_IN, OUTPUT);
    pinMode(DRUM_EN, OUTPUT);

    // Set ultrasonic sensor pins
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}

void loop() {
    Blynk.run();  // Run Blynk connection

    float distance = getDistance();
    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance < 20) {
        // 🚨 Stop all motors if an obstacle is detected
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        analogWrite(ENA, 0);
        analogWrite(ENB, 0);
    } else {
        // Left Motor Control
        if (leftMotor) {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            analogWrite(ENA, 150);
        } else {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            analogWrite(ENA, 0);
        }

        // Right Motor Control
        if (rightMotor) {
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            analogWrite(ENB, 150);
        } else {
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            analogWrite(ENB, 0);
        }

        // Both Motors Control
        if (bothMotors) {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            analogWrite(ENA, 150);
            analogWrite(ENB, 150);
        }

        // Drum Motor Control (Always Runs)
        if (drumMotor) {
            digitalWrite(DRUM_IN, HIGH);
            analogWrite(DRUM_EN, 100);
        } else {
            digitalWrite(DRUM_IN, LOW);
            analogWrite(DRUM_EN, 0);
        }
    }
}

