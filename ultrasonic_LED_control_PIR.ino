// Define pins for the ultrasonic sensors
const int trigPin1 = 9;
const int echoPin1 = 10;
const int trigPin2 = 11;
const int echoPin2 = 12;
const int trigPin3 = 13;
const int echoPin3 = 8;

// Define pins for the relays
const int relayPin1 = 2;
const int relayPin2 = 3;
const int relayPin3 = 4;
const int relayPin4 = 5;

// Define pin for the IR sensor
const int irSensorPin = A0; // Change this to the actual pin you're using

// Variables to store the distance
float distance1, distance2, distance3;

// Distance threshold for triggering the relays
const float distanceThreshold = 10.0; // 10 cm

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Set the trigger pins as OUTPUT and the echo pins as INPUT
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  // Set the relay pins as OUTPUT
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);

  // Set the IR sensor pin as INPUT
  pinMode(irSensorPin, INPUT);

  // Initialize all relays to HIGH (off) because they are active-low
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);
  digitalWrite(relayPin3, HIGH);
  digitalWrite(relayPin4, HIGH);
}

void loop() {
  // Measure distance from Sensor 1
  distance1 = getDistance(trigPin1, echoPin1);
  // Measure distance from Sensor 2
  distance2 = getDistance(trigPin2, echoPin2);
  // Measure distance from Sensor 3
  distance3 = getDistance(trigPin3, echoPin3);

  // Read the IR sensor value
  int irSensorValue = digitalRead(irSensorPin);

  // Print the distances and IR sensor value to the Serial Monitor
  Serial.print("Distance Sensor 1: ");
  Serial.print(distance1);
  Serial.println(" cm");
  
  Serial.print("Distance Sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");
  
  Serial.print("Distance Sensor 3: ");
  Serial.print(distance3);
  Serial.println(" cm");

  Serial.print("IR Sensor Value: ");
  Serial.println(irSensorValue);

  // Control Relay 1 based on distance from Sensor 1
  if (distance1 < distanceThreshold) {
    digitalWrite(relayPin1, LOW); // Turn on Relay 1 (active-low)
  } else {
    digitalWrite(relayPin1, HIGH); // Turn off Relay 1 (active-low)
  }

  // Control other relays based on distance measurements
  controlRelays(distance2, relayPin2);
  controlRelays(distance3, relayPin3);

  // Control Relay 4 based on IR sensor value
  if (irSensorValue == HIGH) { // Change the condition based on your IR sensor's logic
    digitalWrite(relayPin4, LOW); // Turn on Relay 4 (active-low)
  } else {
    digitalWrite(relayPin4, HIGH); // Turn off Relay 4 (active-low)
  }

  // Wait for 500 milliseconds before the next measurement
  delay(500);
}

// Function to get distance from an ultrasonic sensor
float getDistance(int trigPin, int echoPin) {
  // Variable to store the duration of the pulse
  long duration;

  // Send a pulse to trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters
  float distance = duration * 0.0344 / 2;

  return distance;
}

// Function to control relays based on distance
void controlRelays(float distance, int relayPin) {
  if (distance < distanceThreshold) {
    digitalWrite(relayPin, LOW); // Turn on the relay (active-low)
  } else {
    digitalWrite(relayPin, HIGH); // Turn off the relay (active-low)
  }
}
