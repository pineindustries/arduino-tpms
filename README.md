<p align="center">
  <img src="/fig/lcd.jpg" width="500">
</p>

# arduino-tpms
An Arduino-based Tire Pressure Monitoring System (TPMS) receiver and LCD display.

# The Little Microcontroller Who Could
The Arduino is a small device for this project, so we leveraged most of the available on-board resources and implemented a few software tricks. The software, as written, requires approximately 95% of the Arduino's 32 kB of flash memory.  So, to display the car icon, we used a basic compression algorithm for the image data and allocated the image to the Arduino's PROGMEM thus avoiding the Arduino's flash memory.  We programmed the interrupt function, executed when a packet is received, to run as fast as possible, terminating as soon as possible on corrupt data, unknown sensor IDs, or when an update recently occurred. This allows the Arduino to return to an RX state quickly to process incoming packets. Packet data and status are kept in volatile memory thereby providing data integrity particularly when new data arrives as the Arduino is updating the LCD, a relatively slow process.

# The Texas Instruments CC1101 Sub-1 GHz Transceiver
We strongly recommend reading TI's documentation cover-to-cover including errata. See the ./ref folder for details. And TI's Smart RF Software is a must (see below).

# TPMS Sensor
- [Schrader 433 MHz Sensor, Part Number: 29086](https://www.rockauto.com/en/moreinfo.php?pk=10672248)\
  Other part numbers may work, as well.

# Receiver Hardware
- [Arduino Pro Mini 328 - 5V/16MHz](https://www.sparkfun.com/arduino-pro-mini-328-5v-16mhz.html)\
  In testing, the 3.3V/8MHz board appeared too slow to meet our processing requirements.
- [SparkFun FTDI Basic Breakout - 5V](https://www.sparkfun.com/sparkfun-ftdi-basic-breakout-5v.html)\
  Mostly required for testing/programming the Arduino. The final version should be hardwired to USB power without an FTDI breakout.
- [CC1101 Wireless Module with SMA Antenna Wireless Transceiver Module 315/433/868/915MHZ](https://www.amazon.com/MELIFE-CC1101-Wireless-Antenna-Transceiver/dp/B0F7XGBX65/ref=sr_1_1_sspa?crid=3VAUSVKWO7J1X&dib=eyJ2IjoiMSJ9.Ovh6JCX6xBW0aTacHEU4RxG7PSH0UBUXchziX6vyQw4_DY5iAhh_MSQOmKswVYjT5r1ZJCHwADc6j0VEgHvz-1s8W-o8c0bFeuBBNveaHmrSf8UTznp3oRrks7hgKcWncGag6ZNWxp1ORjU6btcFzQsFeivNVp721h_hN2T_PWnJlWTl9T2nH3B5jUt3OfZ26bdkHzLnMNaU1nVzCoqnOfBOUy5avaYX1ICm2gvwGyw.QP6eXnBjD3qnp3Yrhjn-9gV6EULDfV0cjzyqNfdrjOk&dib_tag=se&keywords=CC1101&qid=1757543754&sprefix=cc1101%2Caps%2C243&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1)
- [Adafruit TLV62569 3.3V Buck Converter Breakout](https://www.adafruit.com/product/4711)\
  Required to convert the Arduino's 5V VCC to support the CC1101 at 3.3V VCC.
- [Waveshare 2 Inch LCD Module](https://www.amazon.com/2inch-IPS-LCD-Display-Module/dp/B082GFTZQD/ref=sr_1_1?crid=TMSQEYATD13Y&dib=eyJ2IjoiMSJ9.CoCSQ0KUHCWGaeqdKZEKbMyFdIKdvxZb5S-CzjhGurBB2rVDdPcGm2T-MVZm5ZoOOuxO2ezvDc-okyEb0l13CAE36Cuq7S02e001oZLn1w5UyEePT5u_4uDdf-txvKP0p7euFOKnVNl9khJDFvAyTn50mothFp-bDsrxlK48LDmj60NL60tR_lRuOEEzzWtRTfrBSlnSq39BpMm0X8sVf6sXVDbig5LABfqxrD380us.nnx5qrxkt6qtYgvGoS2wNFfm9cIrhaaTqPi_mWPPFR8&dib_tag=se&keywords=waveshare%2B2inch&qid=1757543550&sprefix=waveshare%2B2innch%2Caps%2C205&sr=8-1&th=1)

### Hardware Pin Configuration
| Arduino Pin # | LCD Pin | CC1101 Pin |
| --- | --- | --- |
| 2   |     | GDO2 |
| 3   | RST |      |
| 4   | BL  |      |
| 5   | DC  |      |
| 7   | CS  |      |
| 10  |     | CSN  |
| 11  | DIN | MOSI |
| 12  |     | MISO |
| 13  | CLK | SCK  |

# EMI Shielding
<p align="left">
  <img src="/fig/case.jpg" width="500">
</p>
The receiver is susceptible to large amounts of Electromagnetic Interference (EMI). We tended to receive more packets by using a shielded case and by shielding each wire. Be sure to connect the shielding to ground.\

# Test Equipment
- [VXDAS TPMS Relearn Tool](https://www.amazon.com/VXDAS-EL-50448-Pressure-Monitor-Activation/dp/B072BK693N/ref=sr_1_2?crid=11EC7BY1C9H3C&dib=eyJ2IjoiMSJ9.O3TlN-ZIJTKZtaIzFnlQsptp8FAK2ySqFb06MysUtvowwqnlw1zhfY-aHUK6yP4xbblbxTqbb-pmzJyQOodniWSdd4LuRACoMSw6UInOaEx9CGKwM582CBAYvGvDFKMP_eUAudGeofFRiNPzRfhOV4bA3PADKtJQB0gZK9c62AXdINLgGEb19V7GP34X0A5w6iw4CAKjQI6eZtRHjhzNeJPpUfwRR63clx74Nlhjd_4.ZDtKkkQNmBKfXmADQyvtHiWO7rAlLI2hv02auOh0vbs&dib_tag=se&keywords=tpms+test&qid=1757543912&sprefix=tpms+tes%2Caps%2C227&sr=8-2)
- [Anti-Static ESD Mat](https://www.amazon.com/Electronics-HPFIX-Grounding-Soldering-Resistant/dp/B08FHQDKDH/ref=sr_1_11?crid=1O6I037HNMGYQ&dib=eyJ2IjoiMSJ9.rvp07dLAZXPh4hdvYVfJbwfGTvxY94Cr6yeWt8g0m2H_zLEV-Jo5SJMrSFmp3plzrp-r-l2w-5dzBhgDWFQfpvZ1R-ExcswkxzbZwrE2eE5WluNuXr6RUALenJdXKlOwn13e_MlE39tiZLmfllPUjd9cihvVjK6YSV0M_YtjGVEAyz1tge4oF7WSBo7SnEHXClUvEuPZDIPMYzLqDcMthzUgiKuMCoqhSKGD-c9ZCK8.KE2MiJktWotRyBuDTm8GqUh9dDzobbXHwTNoAFKRSVU&dib_tag=se&keywords=static%2Bmat&qid=1757552034&sprefix=static%2Bma%2Caps%2C204&sr=8-11&th=1)

# Test Software
- [Texas Instruments SmartRF Studio 7](https://www.ti.com/tool/SMARTRFTM-STUDIO)

# Development Environment
We love Microsoft's [VS Code](https://code.visualstudio.com/) with the [Platform.io Extension](https://platformio.org/install/ide?install=vscode). If you prefer the official [Arduino IDE](https://www.arduino.cc/en/software/), the Pro Mini may require the Legacy Version.

# The Schrader TPMS Signal and Raw Data
![](/fig/signal.png)
Recorded using the [Nooelec RTL-SDR](https://www.amazon.com/Nooelec-RTL-SDR-SDR-100kHz-1-75GHz-Enclosure/dp/B01HA642SW/ref=sr_1_8?crid=AYZBZUIE7KMZ&dib=eyJ2IjoiMSJ9.sfbFJPG624K7g9NEHFje-VR12urW7PR8_DoFUjFh_J7yQX8cQgHa5ON6EM0iFGw1JVl7eBOLglMllG3J9aFaGfhCzEPVqPthgyl45UL8aoaRdDEF0CYBVHsrMdA84MnoT0dFXo0sZh0bm5ZXlHmxHU4bsyitbnJiTBIGQyXSgx7ImBglHYKzo__P4SgTwmAzymHZG9LzC7lxgOoUOw0xRdSENWxluiXf3jE0ptzCzvk.ic5znoyjSEfrqUMCeXKBMeouyr62M9HiimfnaWOXSxM&dib_tag=se&keywords=rtl-sdr&qid=1757548909&sprefix=rtl-sdr%2Caps%2C236&sr=8-8) with [SDRSharp](https://airspy.com/download/) and displayed with [Audacity](https://www.audacityteam.org/download/)
- Frequency: 433.92 MHz
- Modulation: ASK/OOK
- Encoding: Manchester
- Packet Length: 88 Bits
- Packet Structure:
```
Byte: 0... 4... 8... 12
      UUUU UUUA AAPT C
   U: Unknown (Battery Status, Other Flags?)
   A: Sensor ID Number
   P: Pressure Byte
   T: Temperature Byte (°F = (T - 60) * 9/5 + 32)
   C: Checksum Byte
```

## Extracting Pressure in psi from Raw Data
```
psi = -0.5
        + floor( P/4 + 0.75 ) * 0.3
          + floor( (P + 1) % 2 + 2 * P/4 + 0.75 ) * 0.4
            + floor( P/4 + 0.25 ) * 0.5
```
  See ./ref/px_algorithm.xlsx

## Checksum Calculation
```math
\begin{flalign}
&C = \left( \sum_{i=0}^{11} b_i \right) \land 0xFF&&
\end{flalign}
```

# Thanks
[LSatan for the SmartRC CC1101 Driver Library](https://github.com/LSatan/SmartRC-CC1101-Driver-Lib)\
[merbanan's rtl-433 also helped debugging and testing](https://github.com/merbanan/rtl_433)
