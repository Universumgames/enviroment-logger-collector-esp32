#pragma once
#include "config.hpp"
#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

class EditableTFTString
{
public:
    EditableTFTString() {}
    EditableTFTString(int x, int y, String permanent, String value, TFT_eSPI *tft, uint32_t bgColor = TFT_BLACK, uint32_t fontColor = TFT_WHITE);
    void setPermanentString(String permanent);
    void setBgColor(uint32_t color);
    void setFontColor(uint32_t color);
    void drawAllNew();
    void updateString(String newString);
    void setTFT(TFT_eSPI *tft);
    void setXY(int x, int y);
    void clear();
    void clearAll();

private:
    int x = 0;
    int y = 0;
    int startX = 0;
    int old_width = 0;
    uint32_t bgColor = TFT_BLACK;
    uint32_t fontColor = TFT_WHITE;
    String permanent = "";
    String currentValue = "";
    TFT_eSPI *tft;
};