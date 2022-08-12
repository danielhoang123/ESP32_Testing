#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16,2);

const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration;
float distanceCm;
float distanceInch;

void setup() {

  Serial.begin(115200);
  // put your setup code here, to run once:
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.print("ESP32 Daniel");

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode(17, OUTPUT);

  digitalWrite(17, LOW);
  digitalWrite(trigPin, LOW);
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distanceCm = duration * SOUND_SPEED/2;

  // Convert to inches
  //distanceInch = distanceCm * CM_TO_INCH;

  lcd.setCursor(0, 0);
  lcd.print("Dist(Cm): ");
  lcd.print(distanceCm);
  // Serial.print("Distance (inch): ");
  // Serial.println(distanceInch);
  if(distanceCm < 10.0){
    digitalWrite(17, HIGH);
    delayMicroseconds(5);
  }else{
    digitalWrite(17, LOW);
    delayMicroseconds(5);
  }

  // UltraSonic();
}

void UltraSonic(){

  //khai bao cac bien
  const unsigned long ThoigianTrigHIGH = 2; //convert to 2ms
  const unsigned long ThoigianTrigLOW = 10; //convert to 2ms
  const unsigned long ThoigianEcho = 1;
  const unsigned long ThoigianHT = 1;


  unsigned long currentMillis1 = millis(); //millis dem nhu cua Arduino
  unsigned long currentMillis2 = millis();
  unsigned long currentMillis3 = millis();
  
  unsigned long previousTrigHIGH = 0;
  unsigned long previousTrigLOW = 0;
  unsigned long previousEcho = 0;
  unsigned long previousHT = 0;

  int ledOnTime = 100 * 10;

  int previousLedOnTime = 0;

  digitalWrite(trigPin, HIGH);
  Serial.println(currentMillis1);
  //Transmit ultrasonic wave every 2ms
  //xuat song sieu am moi 2ms
  // if((currentMillis1 - previousTrigHIGH) >= ThoigianTrigHIGH){
  //   digitalWrite(trigPin, HIGH);
  //   previousTrigHIGH = currentMillis1;
  // }

  if((currentMillis1 - previousTrigLOW) >= ThoigianTrigHIGH){
    digitalWrite(trigPin, LOW);
    previousTrigLOW = currentMillis1;
  }

  //Receive ultrasonic wave every 10ms
  //thu song sieu am moi 10ms
  if((currentMillis2 - previousEcho) >= ThoigianEcho){
    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * SOUND_SPEED/2;
    previousEcho = currentMillis2;
  }

  //hien len LCD moi 20ms
  if((currentMillis3 - previousHT) >= ThoigianHT){
    lcd.setCursor(0, 0);
    lcd.print("Dist(Cm): ");
    lcd.print(distanceCm);
    previousHT = currentMillis3;
  }

  // if((currentMillis1 - previousLedOnTime) >= ledOnTime){
  //   if(distanceCm <= 10.0){
  //     digitalWrite(17, HIGH);
  //   }
  // }
}
