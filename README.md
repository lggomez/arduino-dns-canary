# arduino-dns-canary
Arduino sketch for a dns canary device to test network connectivity. For this end it loops trough certain domains (configurable through code) establishing connections and priting an appoximate connection time, and the connection error (should any happen). No further requests are made so this basically tests the DNS resolution status of your internet connection. Useful if your devices rely heavily on ad-hoc network adapter configurations that make connection status diagnosis difficult (i.e: being behind seven proxies, VPNs, etcetera). Thus, this device acts as a simple network canary

Materials:
* Arduino Nano v3.0
* For the ethernet connectivity the Arduino stdlib is used, so a W5500 was picked for this task
* Display: HD44780U (or equivalent) 16x2 display with a PCF8574T I2C front board

## Schematic:
<p align="center">
  <img src="/canary_bb.png" width="500">
</p>

## Prototype:
In this case I was lazy and decided to bundle it with the shield due to the amount of connections, but the above schematics still hold

<p align="center">
  <img src="/proto1.jpg" width="500">
</p>

<p align="center">
  <img src="/proto2.jpg" width="500">
</p>
