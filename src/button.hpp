#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <esp32-hal-gpio.h>
#include <esp32-hal.h>
#include <sys/types.h>

struct ButtonState
{
    ulong bounceStart;
    uint debounceTime;
    int gpio;
    bool bouncing;
    bool down;

    ButtonState(int gpio, uint debounceTime, bool pull = true) : gpio(gpio),
                                                                 debounceTime(debounceTime)
    {
        pinMode(gpio, pull ? INPUT_PULLDOWN : INPUT);
    }

    void pollButton()
    {
        if (digitalRead(gpio) == LOW) {
            down = false;
            return;
        }

        ulong currTime = millis();

        if (!bouncing) {
            bouncing = true;
            bounceStart = currTime;
            return;
        }

        if (currTime - bounceStart > debounceTime) {
            down = true;
        }
    }
};

#endif // __BUTTON_H__
