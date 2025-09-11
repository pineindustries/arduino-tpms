# arduino-tpms
Software and basic hardware description of an Arduino-based Tire Pressure Monitoring System (TPMS) receiver.

# TPMS Sensor
- [Schrader 433 MHz Sensor, Part Number: 29086](https://www.rockauto.com/en/moreinfo.php?pk=10672248)

# Receiver Hardware
- [Arduino Pro Mini 328 - 5V/16MHz](https://www.sparkfun.com/arduino-pro-mini-328-5v-16mhz.html)
- [SparkFun FTDI Basic Breakout - 5V](https://www.sparkfun.com/sparkfun-ftdi-basic-breakout-5v.html)
- [Adafruit TLV62569 3.3V Buck Converter Breakout](https://www.adafruit.com/product/4711)
- [CC1101 Wireless Module with SMA Antenna Wireless Transceiver Module 315/433/868/915MHZ](https://www.amazon.com/MELIFE-CC1101-Wireless-Antenna-Transceiver/dp/B0F7XGBX65/ref=sr_1_1_sspa?crid=3VAUSVKWO7J1X&dib=eyJ2IjoiMSJ9.Ovh6JCX6xBW0aTacHEU4RxG7PSH0UBUXchziX6vyQw4_DY5iAhh_MSQOmKswVYjT5r1ZJCHwADc6j0VEgHvz-1s8W-o8c0bFeuBBNveaHmrSf8UTznp3oRrks7hgKcWncGag6ZNWxp1ORjU6btcFzQsFeivNVp721h_hN2T_PWnJlWTl9T2nH3B5jUt3OfZ26bdkHzLnMNaU1nVzCoqnOfBOUy5avaYX1ICm2gvwGyw.QP6eXnBjD3qnp3Yrhjn-9gV6EULDfV0cjzyqNfdrjOk&dib_tag=se&keywords=CC1101&qid=1757543754&sprefix=cc1101%2Caps%2C243&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1)
- [Waveshare 2 Inch LCD Module](https://www.amazon.com/2inch-IPS-LCD-Display-Module/dp/B082GFTZQD/ref=sr_1_1?crid=TMSQEYATD13Y&dib=eyJ2IjoiMSJ9.CoCSQ0KUHCWGaeqdKZEKbMyFdIKdvxZb5S-CzjhGurBB2rVDdPcGm2T-MVZm5ZoOOuxO2ezvDc-okyEb0l13CAE36Cuq7S02e001oZLn1w5UyEePT5u_4uDdf-txvKP0p7euFOKnVNl9khJDFvAyTn50mothFp-bDsrxlK48LDmj60NL60tR_lRuOEEzzWtRTfrBSlnSq39BpMm0X8sVf6sXVDbig5LABfqxrD380us.nnx5qrxkt6qtYgvGoS2wNFfm9cIrhaaTqPi_mWPPFR8&dib_tag=se&keywords=waveshare%2B2inch&qid=1757543550&sprefix=waveshare%2B2innch%2Caps%2C205&sr=8-1&th=1)

# Test Equipment
- [VXDAS TPMS Relearn Tool](https://www.amazon.com/VXDAS-EL-50448-Pressure-Monitor-Activation/dp/B072BK693N/ref=sr_1_2?crid=11EC7BY1C9H3C&dib=eyJ2IjoiMSJ9.O3TlN-ZIJTKZtaIzFnlQsptp8FAK2ySqFb06MysUtvowwqnlw1zhfY-aHUK6yP4xbblbxTqbb-pmzJyQOodniWSdd4LuRACoMSw6UInOaEx9CGKwM582CBAYvGvDFKMP_eUAudGeofFRiNPzRfhOV4bA3PADKtJQB0gZK9c62AXdINLgGEb19V7GP34X0A5w6iw4CAKjQI6eZtRHjhzNeJPpUfwRR63clx74Nlhjd_4.ZDtKkkQNmBKfXmADQyvtHiWO7rAlLI2hv02auOh0vbs&dib_tag=se&keywords=tpms+test&qid=1757543912&sprefix=tpms+tes%2Caps%2C227&sr=8-2)

# Test Software
- [Texas Instruments SmartRF Studio 7](https://www.ti.com/tool/SMARTRFTM-STUDIO)

# Configure Arduino Software
- Edit tpms.ino
- 

# The Schrader TPMS Signal
![](/fig/signal.png)
Recorded using the [Nooelec RTL-SDR](https://www.amazon.com/Nooelec-RTL-SDR-SDR-100kHz-1-75GHz-Enclosure/dp/B01HA642SW/ref=sr_1_8?crid=AYZBZUIE7KMZ&dib=eyJ2IjoiMSJ9.sfbFJPG624K7g9NEHFje-VR12urW7PR8_DoFUjFh_J7yQX8cQgHa5ON6EM0iFGw1JVl7eBOLglMllG3J9aFaGfhCzEPVqPthgyl45UL8aoaRdDEF0CYBVHsrMdA84MnoT0dFXo0sZh0bm5ZXlHmxHU4bsyitbnJiTBIGQyXSgx7ImBglHYKzo__P4SgTwmAzymHZG9LzC7lxgOoUOw0xRdSENWxluiXf3jE0ptzCzvk.ic5znoyjSEfrqUMCeXKBMeouyr62M9HiimfnaWOXSxM&dib_tag=se&keywords=rtl-sdr&qid=1757548909&sprefix=rtl-sdr%2Caps%2C236&sr=8-8) with [SDRSharp](https://airspy.com/download/) and displayed with [Audacity](https://www.audacityteam.org/download/)
- Frequency: 433.92 MHz
- Modulation: ASK/OOK
- Packet Length: 88 Bits
- Packet Structure:\
  Byte: 0... 4... 8... 12\
        UUUU UUUA AAPT C\
     U: Unknown (Battery Status?)\
     A: Sensor ID Number\
     P: Pressure Byte\
     T: Temperature Byte\
     C: Checksum Byte\
