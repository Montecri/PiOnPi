# PiOnPi
![ezgif-2-5d4288b5c5](https://user-images.githubusercontent.com/38574378/224581356-7ed40c9f-ac50-454b-8823-12422f6c3009.gif)


Infinitelly Calculating Digits of Pi on Rapsberry Pi Pico

| MAX7219 Module |      Pico     |
|----------------|---------------|
| VCC            | 3V3  (pin 36) |
| GND            | GND  (pin 38) |
| DIN            | GP19 (pin 25) |
| CS             | GP17 (pin 22) |
| CLK            | GP18 (pin 24) |

Makes use of:
- https://github.com/ozhantr/DigitLedDisplay/

C code from:
- https://bellard.org/pi/

Adapted to this use.

---
<p align="center"><img src="https://user-images.githubusercontent.com/38574378/132773469-08fb7b59-2f9d-4641-9665-c8d50d3904bc.png"><b>   ATTENTION   </b><img src="https://user-images.githubusercontent.com/38574378/132773469-08fb7b59-2f9d-4641-9665-c8d50d3904bc.png"></p> 

The code, as is, REQUIRES the target platform to support DOUBLE type to work (and works faster if the platform suports LONG LONG), if you try to port it to, for example, AVR, double will get demoted to float and break the code. Some work will be required to make it compatible. (Thanks to @dwillmore who pointed this out).

---

<p><img src="https://user-images.githubusercontent.com/38574378/224581754-acded585-7f55-4bdb-a56c-e0261c317b01.jpg" width="50%" height="50%"/></p>


https://www.linkedin.com/pulse/pi-on-pi-happy-pi-day-2023-2nd-generation-machine-cristiano-monteiro
