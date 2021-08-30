#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void setup() {
  Serial.begin(115200);
  Serial.println(F("test"));

  tft.begin();
  tft.setRotation(1);
}

void loop() {
  delay(2000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, 30);
  tft.setFreeFont(&Orbitron_Light_24);
  tft.println("Temp     Humidity");
  tft.drawLine(0, 35, 250, 35, TFT_BLUE);
  tft.setCursor(0, 60);
  tft.print("XX");tft.print(F("C"));
  tft.setCursor(130, 60);
  tft.print("YY");tft.print(F("%"));
}
