/*
 * Kết nối:
            S401                          Arduino
            Đỏ                               5V
            đen                               GND
            vàng                              2

 */
/* Kết nối:
            nút nhấn                       Arduino
            chân 1                            GND
            chân 2                             3

 */
 /*
 * Kết nối: LCD
            LCD                             Arduino
            VCC                               5V
            GND                               GND
            SCL                               A5
            SDA                               A4

   Nạp code mở Serial Monitor chọn No line ending, baud 9600.
 */
byte sensorInterrupt = 0; //khai báo pin ngắt
byte sensorPin = 2;       //chân tín hiệu nối với cảm biến

float calibrationFactor = 4.5;  //hằng số tính toán

volatile byte pulseCount; //biến đếm

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;
#define nutnhan 3
#define bom 4 //chân điều khiển bơm là chân 4
#define led 5 //chân đk led
#define coi 6
long time;
int thoigianset = 10;  //Thời gian chạy động cơ tính = giây
int i =0,j=0;
 #include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void setup()
{

    // Initialize a serial connection for reporting values to the host
    Serial.begin(9600);
    lcd.init();                      // initialize the lcd 
  lcd.backlight();
  pinMode(led, OUTPUT);
  pinMode(coi, OUTPUT);
  pinMode(bom, OUTPUT);
  pinMode(nutnhan, INPUT_PULLUP);
    pinMode(sensorPin, INPUT);
    digitalWrite(sensorPin, HIGH);

    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    oldTime = 0;

    // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
    // Configured to trigger on a FALLING state change (transition from HIGH
    // state to LOW state)
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    lcd.setCursor(0, 0);
  lcd.print("LUU LUONG:   l/p");
  lcd.setCursor(10, 0);
  lcd.print(0);
  lcd.setCursor(0, 1);
  lcd.print("TG:            s");
  lcd.setCursor(4, 1);
  lcd.print(0);
}

void loop()
{

    if ((millis() - oldTime) > 1000) // Only process counters once per second
    {
        detachInterrupt(sensorInterrupt);
        flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
        oldTime = millis();
        flowMilliLitres = (flowRate / 60) * 1000;
        totalMilliLitres += flowMilliLitres;

        unsigned int frac;

        // Print the flow rate for this second in litres / minute
        Serial.print("Flow rate: ");
        Serial.print(int(flowRate)); // Print the integer part of the variable
        Serial.print("L/min");
        Serial.print("\t"); // Print tab space
          
        // Print the cumulative total of litres flowed since starting
        Serial.print("Output Liquid Quantity: ");
        Serial.print(totalMilliLitres);
        Serial.println("mL");
        Serial.print("\t"); // Print tab space
        Serial.print(totalMilliLitres / 1000);
        Serial.print("L");
        lcd.setCursor(0, 0);
  lcd.print("LUU LUONG:   l/p");
  lcd.setCursor(10, 0);
  lcd.print(flowRate,1);
   if(flowRate > 4.0){
  digitalWrite(coi, HIGH);
  digitalWrite(led, LOW);
  digitalWrite(bom, LOW);
  i=0;
 }
 if(flowRate < 4.0){
  digitalWrite(coi, LOW);
  digitalWrite(led, HIGH);
 }
        // Reset the pulse counter so we can start incrementing again
        pulseCount = 0;

        // Enable the interrupt again now that we've finished sending output
        attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    }
    if(digitalRead(nutnhan) == 0){
      i =1;
      time = millis();
    }
    if(i == 1){
      if(millis() - time < (thoigianset*1000)){
  digitalWrite(bom, HIGH);
   lcd.setCursor(0, 1);
  lcd.print("TG:            s");
  lcd.setCursor(4, 1);
  lcd.print(thoigianset - ((millis()-time)/1000));
      }
      else{
        digitalWrite(bom, LOW);
        i = 0;
      }
}
 else{
        digitalWrite(bom, LOW);
      }
delay(100);
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
    // Increment the pulse counter
    pulseCount++;
}
