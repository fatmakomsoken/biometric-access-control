#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C adresi 0x27 olabilir, değişebilir
SoftwareSerial mySerial(2, 3); // RX, TX pinleri
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int buzzer = 8;

void setup()
{
  pinMode(buzzer, OUTPUT);
  lcd.begin();
  lcd.backlight();

  Serial.begin(9600);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    lcd.print("Sensor Baglandi");
  } else {
    lcd.print("Sensor HATASI!");
    while (1);
  }

  delay(2000);
  lcd.clear();
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("Parmak okutunuz");

  int id = getFingerprintIDez();

  if (id >= 0) {
    lcd.clear();
    lcd.print("Giris Basarili");
    digitalWrite(buzzer, LOW);
  } else {
    lcd.clear();
    lcd.print("Erisim RED!");
    digitalWrite(buzzer, HIGH);
    delay(2000);
    digitalWrite(buzzer, LOW);
  }

  delay(3000);
  lcd.clear();
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerSearch();
  if (p != FINGERPRINT_OK) return -1;

  return finger.fingerID;
}
