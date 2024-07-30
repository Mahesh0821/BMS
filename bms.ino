//#include<simpleDHT.h>
#include<DHT.h>
#include<LiquidCrystal_I2C.h>
#include<wire.h>
#define DHTPIN 7
#define DHtTYPE DHT11
#define MQ2pin 0
LiquidCrystal_I2C lcd(0x27,16,2); 
DHT dht(DHTPIN,DHtTYPE);


int temperature;
float sensorValue;
int buzzer=8;
int smokeAO=A0;

void setup()
{
  Serial.begin(9600);
  Serial.println("====temeperature====");
  dht.begin();
  lcd.init();
  lcd.backlight();
  Serial.print("MQ2 warning up !");
  pinMode(buzzer,OUTPUT);
  pinMode(smokeAO,INPUT);

}

void loop1()
{
  temperature=dht.readTemperature();
  if (temperature<=40)
  {
  Serial.print("temperature: ");
  Serial.print(temperature);
  Serial.print(" ° Celsius");
  lcd.setCursor(0,0);
  lcd.println(" Now Temperature: ");
  lcd.setCursor(0,1);
  lcd.print(temperature);
  lcd.print(" ° Celsius");

  }
  else if (temperature>50)
  {
  Serial.print("temperature: ");
  Serial.print(temperature);
  Serial.print(" ° Celsius");
  lcd.setCursor(0,0);
  lcd.println(" Now Temperature");
  lcd.setCursor(0,1);
  lcd.print(temperature);
  lcd.print(" ° Celsius");
  lcd.print("WARNING: HIGHTEMP PLEASE CHECK");
  lcd.clear();
  delay(2000);
  }
}

  void loop2()
  {
    sensorValue=analogRead(MQ2pin);
    Serial.print("concentration : ");
    Serial.print(sensorValue);
    if (sensorValue<=100)
    {
      Serial.println("NORMAL");
      noTone(buzzer);
    }
    else if(sensorValue<=200)
    {
      Serial.println("WARNINNG 1");
      tone(buzzer,1000,200);
    }
    else if(sensorValue<=300)
    {
      Serial.println("WARNING 2");
      tone(buzzer,2000,100);
    }
    else
    {
      Serial.println("DANGER");
      tone(buzzer,3000);
    }
    delay(2000);

  }
  void loop()
  {
    loop1();
    loop2();
  }
