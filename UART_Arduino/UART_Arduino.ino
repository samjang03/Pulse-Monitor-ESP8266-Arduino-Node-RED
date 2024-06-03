// Arduino code

//#include <SoftwareSerial.h>
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library. 

#define USE_ARDUINO_INTERRUPTS true

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 

PulseSensorPlayground pulseSensor; 

const int PULSE_INPUT_PIN = A0; // Пін для зчитування пульсового сигналу
const int PULSE_SAMPLE_PERIOD = 2000; // Період вибірки пульсу в мілісекундах
const int PULSE_MIN = 60; // Мінімальне значення пульсу (ударів на хвилину)
const int PULSE_MAX = 90; // Максимальне значення пульсу (ударів на хвилину)
const int BUZZER_PIN = 3; // Пін для зумера
const int RED_LED_PIN = 4; // Пін для червоного світлодіода
const int GREEN_LED_PIN = 5; // Пін для зеленого світлодіода
const int BLUE_LED_PIN = 6; // Пін для синього світлодіода

unsigned long lastPulseSampleTime = 0; // Час останнього зчитування пульсу
int currentPulse = 60; // Поточне значення пульсу (ударів на хвилину)
                               
//SoftwareSerial SMESerial (7, 8);


void setup() {
//  SMESerial.begin(9600);
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT); // Ініціалізація зумера як виходу
  pinMode(RED_LED_PIN, OUTPUT); // Ініціалізація червоного світлодіода як виходу
  pinMode(GREEN_LED_PIN, OUTPUT); // Ініціалізація зеленого світлодіода як виходу
  pinMode(BLUE_LED_PIN, OUTPUT); // Ініціалізація синього світлодіода як виходу

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}

void loop() {

  // Перевірка, чи настав час для зчитування нового значення пульсу
  if (millis() - lastPulseSampleTime >= PULSE_SAMPLE_PERIOD) {
    lastPulseSampleTime = millis(); // Оновлення часу останнього зчитування

    // Генерація нового значення пульсу
    currentPulse = random(PULSE_MIN - 10, PULSE_MAX + 3);

    Serial.println(currentPulse);

//   Serial.println("♥  A HeartBeat Happened ! "); 
//   Serial.println(String("BPM: ") + currentPulse);                        
                     
//   SMESerial.print('\r');
//   SMESerial.print(currentPulse);
//   SMESerial.print('|');
//   SMESerial.print('\n');
  
//   Serial.print('\r');
//   Serial.print(currentPulse);
//   Serial.print('|');
//   Serial.print('\n');

//    Serial.print("Pulse: ");
//    Serial.print(currentPulse);
//    Serial.println(" BPM");

    // Перевірка стану пульсу та відображення на світлодіодах і зумері
    if (currentPulse > PULSE_MAX) {
      // Високий пульс
      digitalWrite(RED_LED_PIN, HIGH); // Увімкнути червоний світлодіод
      digitalWrite(GREEN_LED_PIN, LOW); // Вимкнути зелений світлодіод
      digitalWrite(BLUE_LED_PIN, LOW); // Вимкнути синій світлодіод
      tone(BUZZER_PIN, 1000); // Увімкнути зумер з частотою 1000 Гц
    } else if (currentPulse < PULSE_MIN) {
      // Низький пульс
      digitalWrite(RED_LED_PIN, HIGH); // Увімкнути червоний світлодіод
      digitalWrite(GREEN_LED_PIN, LOW); // Вимкнути зелений світлодіод
      digitalWrite(BLUE_LED_PIN, LOW); // Вимкнути синій світлодіод
      tone(BUZZER_PIN, 500); // Увімкнути зумер з частотою 500 Гц
    } else {
      // Нормальний пульс
      digitalWrite(RED_LED_PIN, LOW); // Вимкнути червоний світлодіод
      digitalWrite(GREEN_LED_PIN, HIGH); // Увімкнути зелений світлодіод
      digitalWrite(BLUE_LED_PIN, LOW); // Вимкнути синій світлодіод
      noTone(BUZZER_PIN); // Вимкнути зумер
    }
  }
}
