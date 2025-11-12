#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>

// Dimensions de l'écran OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Création des objets
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_BMP280 bmp;   // Capteur de température et pression
BH1750 lightMeter;     // Capteur de luminosité

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== Wemos D1 mini + OLED + BMP280 + BH1750 ===");

  Wire.begin(D1, D2); // SDA = D1, SCL = D2

  // --- Initialisation de l'écran ---
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Erreur : ecran OLED non detecte !"));
    while (true);
  }

  // --- Initialisation du BMP280 ---
  bool bmpStatus = bmp.begin(0x76); // Adresse courante
  if (!bmpStatus) {
    Serial.println(F("BMP280 non detecte a 0x76, essai 0x77..."));
    if (!bmp.begin(0x77)) {
      Serial.println(F("Erreur : capteur BMP280 non trouve !"));
      while (true);
    }
  }

  // --- Initialisation du BH1750 ---
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("Erreur : capteur BH1750 non detecte !"));
    while (true);
  }

  // --- Message de démarrage ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Wemos D1 mini OK");
  display.println("BMP280 + BH1750");
  display.println("Initialisation...");
  display.display();
  delay(2000);
}

void loop() {
  // --- Lecture des deux capteurs ---
  float temperature = bmp.readTemperature();       // °C
  float pressure = bmp.readPressure() / 100.0;     // hPa
  float lux = lightMeter.readLightLevel();         // Lux

  // --- Affichage série ---
  Serial.print("Temp = ");
  Serial.print(temperature);
  Serial.print(" °C | Press = ");
  Serial.print(pressure);
  Serial.print(" hPa | Lux = ");
  Serial.print(lux);
  Serial.println(" lx");

  // --- Affichage sur OLED ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("BMP280 + BH1750");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 32);
  display.print("Press: ");
  display.print(pressure, 1);
  display.println(" hPa");

  display.setCursor(0, 44);
  display.print("Lumi: ");
  display.print(lux, 1);
  display.println(" lx");

  display.display();

  delay(1500);
}
