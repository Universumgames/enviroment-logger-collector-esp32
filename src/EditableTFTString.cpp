#include "EditableTFTString.hpp"

EditableTFTString::EditableTFTString(int x, int y, String permanent, String value, TFT_eSPI *tft, uint32_t bgColor, uint32_t fontColor)
{
    this->x = x;
    this->y = y;
    this->tft = tft;
    this->permanent = permanent;
    this->currentValue = value;
    this->bgColor = bgColor;
    this->fontColor = fontColor;
}

void EditableTFTString::setPermanentString(String permanent)
{
    this->permanent = permanent;
    drawAllNew();
}

void EditableTFTString::setBgColor(uint32_t color)
{
    this->bgColor = color;
}

void EditableTFTString::setFontColor(uint32_t color)
{
    this->fontColor = color;
}

void EditableTFTString::drawAllNew()
{
    clearAll();
    tft->setTextColor(fontColor);
    this->startX = tft->drawString(permanent, x, y) + x;
    this->old_width = tft->drawString(currentValue, startX, y) + tft->textWidth("0");
}

void EditableTFTString::updateString(String newString)
{
    if (newString != currentValue)
    {
        this->currentValue = newString;
        clear();
        tft->setTextColor(fontColor);
        this->old_width = tft->drawString(currentValue, startX, y) + tft->textWidth("0");
    }
}

void EditableTFTString::setTFT(TFT_eSPI *tft)
{
    this->tft = tft;
}

void EditableTFTString::clear()
{
    tft->fillRect(startX, y, old_width, tft->fontHeight(), bgColor);
}

void EditableTFTString::clearAll()
{
    tft->fillRect(x, y, startX + old_width, tft->fontHeight(), bgColor);
}

void EditableTFTString::setXY(int x, int y)
{
    clearAll();
    this->x = x;
    this->y = y;
    drawAllNew();
}