/*
  demo to show functioning of touchscreen for a WT32-SCO1 display + ESP32
  using the FT6336U library https://github.com/aselectroworks/Arduino-FT6336U
*/

#include <TFT_eSPI.h> // Hardware-specific library
#include "FT6336U.h"
// pin definition for WT32-SCO1 display
#define I2C_SDA 18
#define I2C_SCL 19
#define RST_N_PIN 21
#define INT_N_PIN 34

// Invoke custom library with default width and height
TFT_eSPI tft = TFT_eSPI();
// WT32-SCO1 uses 18 & 19 for SDA & SCL i2c - see above pin definitions
FT6336U ft6336u(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN);


int tp_x = 0;
int tp_y = 0;

// Coordinates for drawing on screen
int x = 160;
int y = 240;

int padding = 0;
int sensor1EventTime;
int sensor2EventTime;
int interval;

void setup(void)
{
  Serial.begin(115200);
  delay(500);
  ft6336u.begin();

  Serial.print("Sketchname: GraphicTouchTest-WT32-SCO1 ");
  Serial.print("FT6336U Firmware Version: ");
  Serial.println(ft6336u.read_firmware_id());
  Serial.print("FT6336U Device Mode: ");
  Serial.println(ft6336u.read_device_mode());
  tft.begin();

  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(4);
  tft.drawString("TEST WT32-SCO1", x, y - 50, 4);
  delay(1000);
  tft.drawString("with FT6336U", x, y, 4);
  delay(1000);
  tft.drawString("12-02-2022", x, y + 50, 4);
  delay (1000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(4);
  tft.drawString("Start touching the screen", x, y - 50, 4);
  tft.drawString("Long press clears screen", x, y, 4);
  while (ft6336u.read_td_status() != 1) {
    // do nothing, waiting for a touschreen press
  }
  tft.fillScreen(TFT_BLACK);
  padding = tft.textWidth("999.9", 4); // get the width of the text in pixels
  tp_x = ft6336u.read_touch1_x();
  tp_y = ft6336u.read_touch1_y();
  y = 130;
  tft.drawString("TP-X : ", x-30, y, 4);
  tft.drawNumber(tp_x, x + 60, y, 4);
  y = 160;
  tft.drawString("TP-Y : ", x-30, y, 4);
  tft.drawNumber(tp_y, x + 60, y, 4);
}

void loop()
{
  if (digitalRead(INT_N_PIN) != -1) {
    Serial.println("in the loop for testing the touch !");
    while (ft6336u.read_td_status() != 0) {
      tft.setTextPadding(padding);
      Serial.print("FT6336U Touch Event/ID 1: ");
      Serial.println(ft6336u.read_touch1_event());
      Serial.print("TP-Y: (");
      Serial.println(ft6336u.read_touch1_x());
      Serial.print("TP_Y: (");
      Serial.println(ft6336u.read_touch1_y());

      tp_x = ft6336u.read_touch1_x();
      sensor1EventTime = millis();
      tp_y = ft6336u.read_touch1_y();
      while ( tp_x == ft6336u.read_touch1_x()) {
        sensor2EventTime = millis();
        if ((sensor2EventTime - sensor1EventTime) > 3000  && ft6336u.read_td_status() != 0) {
          tft.fillScreen(TFT_BLACK);
          tft.drawString("Start touching the screen", x, y - 50, 4);
          tft.drawString("Long press clears screen", x, y, 4);
          while (ft6336u.read_td_status() != 1) {
            // do nothing, waiting for a touschreen press
            sensor1EventTime = 0;
            sensor2EventTime = 0;
          }
          tft.fillScreen(TFT_BLACK);
        }
      }
      y = 130;
      tft.drawString("TP-X : ", x-30, y, 4);
      tft.drawNumber(tp_x, x + 60, y, 4);
      y = 160;
      tft.drawString("TP-Y : ", x-30, y, 4);
      tft.drawNumber(tp_y, x + 60, y, 4);

      drawCross(tp_x, tp_y, TFT_CYAN);
    }

  }
}

void drawCross(int xx, int yy, unsigned int color)
{
  tft.drawLine(xx - 5, yy, xx + 5, yy, color);
  tft.drawLine(xx, yy - 5, xx, yy + 5, color);
}
