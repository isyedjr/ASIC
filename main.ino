#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <MS5611.h>

double referencePressure;

// 5 is GC pin, 7 is CS for SD

MS5611 MS5611(0x77);

const int cs = 7;
const int GC = 5;
int count;
long ti = 0;
long tf = 0;

int lim = 30000;

long tree;
double tree2;
float altitude;

int temp, UV1, UV2;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin();
  if (MS5611.begin() == true)
  {
    Serial.println("MS5611 found.");
  }
  else
  {
    Serial.println("MS5611 not found. halt.");
    while (1)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }

  referencePressure = MS5611.getPressure();
  MS5611.setOversampling(OSR_STANDARD);
  
  delay(3000);
  pinMode(A0, INPUT);
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  pinMode(GC, INPUT);
  
  
  if (!SD.begin(cs)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }

}

void loop()
{
  ti = millis();
  count = 0;
  while((millis() - ti) < lim) {
    if(digitalRead(GC) == HIGH) {
      while(digitalRead(GC) == HIGH) {
      }
      count++;
    }
    
  }
  tf = millis();

  temp = analogRead(A0);
  UV1 = analogRead(A2);
  UV2 = analogRead(A3);
  MS5611.read();
  tree = MS5611.getPressure();
  tree2 = MS5611.getTemperature();
  // altitude = ms5611.getAltitude(tree);
  
  String record = "";
  record += String(temp);
  record += ",";
  record += String(UV1);
  record += ",";
  record += String(UV2);
  record += ",";
  record += String(tree);
  record += ",";
  record += String(tree2);
  record += ",";
  record += String(count);
  record += ",";
  record += String(tf - ti);
  
  Serial.print("UV1: ");
  Serial.println(UV1);
  Serial.print("UV2: ");
  Serial.println(UV2);
  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Pressure : ");
  Serial.println(tree);
  Serial.print("Temp 2: ");
  Serial.println(tree2);
  Serial.print("Count: ");
  Serial.println(count);

  File df = SD.open("datalog.txt", FILE_WRITE);
  if (df) {
    df.println(record);
    df.close();
  } else {
    Serial.println("can't open file");  
  }
  
}
