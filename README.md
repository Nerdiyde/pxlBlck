# pxlBlck - An IOT/SmartHome notification system
![](https://nerdiy.de/wp-content/uploads/2020/11/www.nerdiy.de-20200717-184353-scaled.jpg)

pxlBlck is a system of hardware and software to set up an event indication system for your SmartHome. Various software and hardware options are available. Thanks to the integration in an EspEasy plug-in, the pxlBlck's can be automated without external automation software. In combination with external automation software (e.g. NodeRed, MQTT server, etc ...), various notifications such as a doorbell-, social media (e.g. WhatsApp, Facebook, ...)-, call notifications on mobile devices and landlines are possible.

**You can find more information about the software and detailed HowTo's in the following articles:**

- [pxlBlck – An IOT/SmartHome information system](https://nerdiy.de/en/pxlblck/)
- [pxlBlck – Install and configure the pxlBlck plugin](https://nerdiy.de/en/howto-pxlblck-das-pxlblck-plugin-installieren-und-konfigurieren/)
- [pxlBlck – Commands to configure the pxlBlck](https://nerdiy.de/en/howto-pxlblck-befehle-zur-konfiguration-des-pxlblck/)
- [pxlBlck – Configure and display animations](https://nerdiy.de/en/howto-pxlblck-animationen-konfigurieren-und-anzeigen/)
- [pxlBlck – Configure watch faces and screensavers](https://nerdiy.de/en/howto-pxlblck-ziffernblaetter-und-bildschirmschoner-konfigurieren/)
- [pxlBlck – Design icons, transfer them to the pxlBlck and display them](https://nerdiy.de/en/howto-pxlblck-icons-designen-auf-den-pxlblck-uebertragen-und-anzeigen/)

**An overview of the different hardware-plattforms is available here:**
- https://nerdiy.de/tag/pxlblckplattform/

### Known bugs
 (checked=done)
 - [x] Bug: Outgoing icon animation "fly out to left" crashes on ESP32
 - [ ] Improve: replace "paramX" by "pxlBlckUtils_parseString()"
 - [ ] Bug: Color handling (in webmenu) is mixed up at using non standard "GRB" led-configuration
 
### Planned features
 - Predefined color palletes

### Trademarks
All third-party trademarks are the property of their respective owners. More infos here: https://nerdiy.de/en/warenzeichen/

### License
Unless otherwise stated, all works presented here and on Nerdiy.de that are not based on software/code are subject to the CC BY-NC-SA 4.0 license (attribution – non-commercial – dissemination under the same conditions 4.0 international).
You can find additional infos here: https://nerdiy.de/en/lizenz/

### Videos
https://www.youtube.com/watch?v=v85Cp-WSeHQ&list=PLBdOzBJ6T0Q4NGt_zj3ziZtrls4c4zjq4

### Credits:
 - ESPEasy:  A big thank you to the guys who started, maintain and in general care about whats going on with ESPEasy. This plugin wouldn't have been possible without them.
   See more about it here: https://www.letscontrolit.com/wiki/index.php/ESPEasy
 - Adafruit: A big thank you to the awesome ladies and gentleman at adafruit.com who made many awesome and important libraries and products.
   If you want to thank/support them, buy one(or more) of their products on www.adafruit.com.
 - RingClock functionality: The RingClock functionality is roughly inspired by the ESPEasy-Plugin: Plugin 070: NeoPixel ring clock