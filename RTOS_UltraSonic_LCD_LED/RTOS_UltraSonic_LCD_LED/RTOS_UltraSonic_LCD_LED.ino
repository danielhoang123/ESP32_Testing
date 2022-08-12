#include <LiquidCrystal_I2C.h>

#define SOUND_SPEED 0.034

LiquidCrystal_I2C lcd(0x27, 16,2);

TaskHandle_t Task_UltraSonic; //khai bao cai goi la "task handle" bao gom
TaskHandle_t Task_LCD;        //2 tasks la "Task1" va "Task2"
TaskHandle_t Task_WarningLed;

// LED pins
const int WarningLed = 2; //led1 la chan so GPIO 2
const int trigPin = 5;  //Trig pin cua SR-0x
const int echoPin = 18; //Echo pin cua SR-0x



long duration;
float distanceCm;

void setup() {

  Serial.begin(115200);

  pinMode(WarningLed, OUTPUT);
  digitalWrite(WarningLed, LOW);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input


  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.print("ESP32 Daniel");

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Ten cua ham chua task can thuc hien */
                    "Task1",     /* Ten cua cai task do theo kieu ky tu */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task_UltraSonic,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task_LCD,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
  delay(500);

}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task_UltraSonic running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(trigPin, LOW);

    vTaskDelay(2 / portTICK_PERIOD_MS); //su dung vTaskDelay thay vi
                                        //cac ham delay khac

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    // delayMicroseconds(10);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distanceCm = duration * SOUND_SPEED/2;

    vTaskDelay(5 / portTICK_PERIOD_MS);
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task_LCD running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    lcd.setCursor(0, 0);
    lcd.print("Dist(Cm): ");
    lcd.print(distanceCm);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    
    if(distanceCm < 10.0){
    digitalWrite(2, HIGH);
    //delayMicroseconds(5);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    }else{
      digitalWrite(2, LOW);
      //delayMicroseconds(5);
      vTaskDelay(5 / portTICK_PERIOD_MS);
    }
  }

  
}

void loop() {
  
}
