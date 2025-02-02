#include "MACCHINA_CONFIG.h"

#if defined(CFG_MACCHINA_A0) || defined(CFG_MACCHINA_ESP32_TEST)


#include "pt_device.h"
#include <esp32_can.h>
#include <FastLED.h>

pt_device* PT_DEVICE = new pt_device();

CRGB led[1];

void pt_device::init_device() {
    this->set_status_led(false); // Assume no connection on statup!
    digitalWrite(GPIO_NUM_13, LOW); // Power up the LED!
    // Add LED
    FastLED.addLeds<WS2812B, GPIO_NUM_2, GRB>(led, 1);
    FastLED.setBrightness(50); // Don't blind the user!
    Can0.setCANPins(GPIO_NUM_4, GPIO_NUM_5); // Set CAN Pins on startup
    this->set_status_led(false); // Assume no connection
    pinMode(GPIO_NUM_35, INPUT);
}

int pt_device::read_batt_mv() {
    // vin = (vOut(r1+r2)/r2)
    //
    // r1 = 10k
    // r2 = 2.2k
    float vout = ((float)analogRead(GPIO_NUM_35) * 3.3) / 4095.0;
    vout = ((vout * 1000.0) / (2200.0 / (10000.0 + 2200.0)));
    return vout + (vout*0.05); // A0 seems to underreport by ~5%
}

void set_led_colour(uint32_t rgb) {
    led[0].setColorCode(rgb);
    FastLED.show();
}

void pt_device::set_rgb_led(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t c = (r << 24 | g << 16 | b << 8);
    set_led_colour(c);
}

void pt_device::set_status_led(bool state) {
    if (state) {
        this->is_connected = true;
        set_led_colour(0x00FF00);
    } else {
        this->is_connected = false;
        set_led_colour(0xFF0000);
    }
}

void pt_device::set_rx_led(bool state){
    if (state) {
        set_led_colour(0x0000FF);
    } else {
        set_led_colour(0x00FF00);
    }
}

void pt_device::set_tx_led(bool state){
    if (state) {
        set_led_colour(0xFFFF00);
    } else {
        set_led_colour(0x00FF00);
    }
}

void pt_device::set_can_led(bool state){}
void pt_device::set_kline_led(bool state){}

#endif

