#ifndef __UTILS_H__
#define __UTILS_H__

#include <Esp.h>
#include <HardwareSerial.h>

void printEspInfo()
{
    Serial.println(F("##################################"));
    Serial.println(F("ESP32 Information:"));
    Serial.printf("Internal Total Heap %d, Internal Used Heap %d, Internal Free Heap %d\n",
                  ESP.getHeapSize(), ESP.getHeapSize() - ESP.getFreeHeap(),
                  ESP.getFreeHeap());
    Serial.printf("Sketch Size %d, Free Sketch Space %d\n", ESP.getSketchSize(),
                  ESP.getFreeSketchSpace());
    Serial.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n",
                  ESP.getPsramSize(), ESP.getFreePsram());
    Serial.printf("Chip Model %s, ChipRevision %d, Cpu Freq %d, SDK Version %s\n",
                  ESP.getChipModel(), ESP.getChipRevision(), ESP.getCpuFreqMHz(),
                  ESP.getSdkVersion());
    Serial.printf("Flash Size %d, Flash Speed %d\n", ESP.getFlashChipSize(),
                  ESP.getFlashChipSpeed());
    Serial.println(F("##################################\n"));
}

#endif // __UTILS_H__
