#include <Servo.h>
// #include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int TRIG_PIN_1 	= 2;  // Trigger pin for hand detection ultrasonic sensor
const int ECHO_PIN_1 	= 3;  // Echo pin for hand detection ultrasonic sensor

const int TRIG_PIN_2 	= 4;  // Trigger pin for bin full detection ultrasonic sensor
const int ECHO_PIN_2 	= 5;  // Echo pin for bin full detection ultrasonic sensor

const int BUZZER_PIN 	= 6;  // Pin for buzzer
const int SERVO_PIN 	= 9;  // Pin for servo motor

String lcdLine1 = "";
String lcdLine2 = "";

// Distance thresholds
const int HAND_DISTANCE = 20;  // Distance threshold for hand detection (20 cm)
const int FULL_DISTANCE = 5;  // Distance threshold for bin full detection (10 cm)

Servo servoMotor;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD object with address 0x27, 16 columns, and 2 rows

void setup()
{
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);
  
  servoMotor.attach(SERVO_PIN);
  servoMotor.write(0);
  lcdScreen("Welcome!","");
}

void loop()
{
  int handDistance 	= getDistance(TRIG_PIN_1, ECHO_PIN_1);
  int binDistance 	= getDistance(TRIG_PIN_2, ECHO_PIN_2);

  Serial.println("Sensor Atas (hand) : "  + (String) handDistance + "cm");
  Serial.println("Sensor Bawah (bin) : "  + (String) binDistance + "cm");
  Serial.println("----------------------------------------------------");

  if(handDistance <= HAND_DISTANCE && handDistance > 0){
    tone(BUZZER_PIN,1000);
    delay(500);
    noTone(BUZZER_PIN);
    Serial.println("Lid Opening...");
    lcdScreen("Smart Bin!","Lid Opening...");
    servoMotor.write(150);
    delay(5000);
    tone(BUZZER_PIN,1000);
    delay(500);
    noTone(BUZZER_PIN);
    Serial.println("Lid Closing...");
    lcdScreen("Smart Bin!","Lid Closing...");
    for (int angle = 150; angle > 5; angle -= 1){
      servoMotor.write(angle);
      delay(50);
    }
    servoMotor.write(5);
    delay(2000);
    lcdScreen("Welcome!","");
  }

  if(binDistance <= FULL_DISTANCE && binDistance > 0){
    Serial.println("Bin Full");
    tone(BUZZER_PIN, 1000);
  	lcdScreen("Warning!!!","Bin is FULL");
  } 
  
  if(binDistance > FULL_DISTANCE){
    lcdScreen("Welcome!","");
    noTone(BUZZER_PIN);	
  }
  
  delay(1000);
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
  int distance = (duration * 0.034) / 2;
  return distance;
}

void lcdScreen(String firstLine, String secondLine){
  if(lcdLine1 != firstLine || lcdLine2 != secondLine){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(firstLine);
    lcd.setCursor(0, 1);
    lcd.print(secondLine);
    lcdLine1 = firstLine;
  	lcdLine2 = secondLine;
  }
}
