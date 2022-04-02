# Arduino Code of "Máquina de Ensaio de Tração" Version 1.0 

Application developed using Arduino IDE 1.8.1, for ESP32 microcontroler.

*This was my **first** Microcontroler, and my **first** Arduino project ever, so dont mind the badly written code.*

## Libraries used:

- Arduino Core for ESP32, by 
Espresif. [Source](https://github.com/espressif/arduino-esp32)
- EspDRO, by MGX3D. [Source](https://github.com/MGX3D/EspDRO)
- HX711 library, by Bodge. [Source](https://github.com/bogde/HX711)
- ESPAsyncWebServer, by Me-no-dev. [Source](https://github.com/me-no-dev/ESPAsyncWebServer)
- Arduino ESP32 filesystem uploader, by Me-no-dev. [Source](https://github.com/me-no-dev/arduino-esp32fs-plugin)

## Notes

```
- The code was developed in 2019, so there may be some incompatibilities using newer libraries.

- EspDRO was not included as a libray, just the basic code to read the digital calipher was used.

- The Version 1 code is the one presented, and is fully working.

- The Full Code contains all the functions wich the original Machine was supposed to execute.
```

# Known Issues

    - This code contains Memory Management and Watchdog issues on core 1.

    - Webserver instable due to watchdog reset. Hardware watchdog for the loop function in core 1 needs to be deactivated.

    - Task "Paquimetro" not working on the full code.

    - No organization of the code, no classes created, tasks poorly distributed between cores.