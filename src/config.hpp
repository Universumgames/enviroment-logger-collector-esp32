#pragma once

#define CCS811_ADDR 0x5A
#define SEALEVELPRESSURE_HPA (1013.25)

#define UPLINK_SERVER_ADDRESS "env.raspi.universum.net" //(my own local dns entry for an raspberry pi)
#define UPLINK_SERVER_PORT 80
#define POST_ENDPOINT "/api/env/iot"

//#define ILI9341_DRIVER // Large Display (that i've had on hand)

//#define ST7735_DRIVER //smaller display (that i've had on hand)

#define noDisplay