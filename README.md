# Tao - A Palm-Sized, Sensor-Driven Dynamic Digital Pet ⭐

Tao is a highly responsive, accessible companion device that uses an **accelerometer** much like on phones that detect orientation to switch from portrait to landscape mode. It also utilizes a **temperature sensor** and **haptic motors** that vibrate, which all triggers unique face animations.

Over the past few months, I've been seeing this unique hardware accessory called ["Starboy" by CREATURE](https://hesjustalittleguy.com/). It features a palm-sized digital pet that has a star-shaped form factor and a round screen in the middle with very expressive eyes. As you can probably tell by the title description, this project was heavily inspired by this. I was immediately captivated by the concept, but with the original product selling for around $150 to $300, I thought it felt like a great open-source, diy project.
> _"STARBOY perceives its environment through a camera, microphone, temperature sensor, and accelerometer. bring him into a loud environment and he gets anxious. Leave him in the cold and he shivers. Shake him and he gets dizzy and mad at you. give him a thumbs up to check the battery level, or flick him off if you feel like making him sad."_

![overall](https://github.com/alvi-n/TaoTheWearableDigitalPet/blob/main/Screenshots/overall.png)

## Hardware
- Seeed XIAO ESP32-S3
- Round GC9A01 SPI LCD
- LSM6DSL accelerometer (I2C)
- PCT2075D Temperature sensor (I2C)
- DRV2605L haptic driver (I2C, shared bus)
- 3.7V 300mAh LiPo battery

| Schematic | PCB | Render |
| ----- | ---- | ---- |
| !["Schematic"](https://github.com/alvi-n/TaoTheWearableDigitalPet/blob/main/Screenshots/schematic.png) | ![PCB](https://github.com/alvi-n/TaoTheWearableDigitalPet/blob/main/Screenshots/pcb.png) | ![PCB](https://github.com/alvi-n/TaoTheWearableDigitalPet/blob/main/Screenshots/pcbwithart.png)


## What It Does
- Two animated apple-seed eyes are Tao's entire face 
  - (Táo means apple in Vietnamese🍎)
  - `eyes.h` computes each eye as a 10-point polygon and changes its scale, spacing, and rotation toward a specifc shape at ~30fps for smooth transitions
- An LSM6DSL accelerometer notes any change in motion in real time
  - `mood.h` reads the change in acceleration magnitude between frames and sorts it into still / moving / shake states
  - This is very similar to how your phone can detect if your screen is turned so you could watch videos fullscreen
- A haptic motor creates different unique haptic patterns depending on what Tao is going through
- Uses an ESP32-S3 development board to make power management easier

## Future Plans
- Add additional states for more interactions
- Use a deburring tool to round out all the edges
### If done again...
- NFC Chip - Originally this was supposed to have one but making a custom antenna was a diffcult process in such a limited space
- No modules - this means all power managment will have to be designed from scratch and same with the screen module
- Add more inputs like a microphone, camera, photoresistor, etc

BOM
| item | quantity | Link |
| --- | --- | --- |
| LSM6DSLTR | 1 | https://a.co/d/095roJXs |
| DRV2605LYZFR | 1 | https://www.lcsc.com/product-detail/C527464.html?s_z=n_q_drv2605l&spm=wm.fly.bg.0.xh&lcsc_vid=TwVeBQVfTwcMXgJUT1daV1VSE1RZVQIAT1cIBlMEQgIxVlNeRlZaV1RQR1ZfUTsOAxUeFF5JWBYZEEoKFBINSQcJGk4NBhADEA4cHktXR1FaSQwSGg0%3D |
| 10kΩ Resistor | 2 | 
| 100nF Capacitors | 4 |
| Pin Header 1x7 2.54mm | 1 | https://www.aliexpress.us/item/3256804295713253.html?spm=a2g0o.productlist.main.2.37dd5e3dMBryvp&algo_pvid=373884e1-f59c-4b72-a437-bb5c888fe552&algo_exp_id=373884e1-f59c-4b72-a437-bb5c888fe552-1&pdp_ext_f=%7B%22order%22%3A%227427%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21USD%213.16%213.16%21%21%213.16%213.16%21%402101ea8c17831339121663052ef6b1%2112000049978631702%21sea%21US%216414118479%21X%211%210%21n_tag%3A-29919%3Bd%3A39b1dfd6%3Bm03_new_user%3A-29895&curPageLogUid=j9He0oLnZBlu&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005004482028005%7C_p_origin_prod%3A#nav-specification
| JST PH 2-pin 2.0mm | 1 | 
| Seeed XIAO ESP32-S3 | 1 | https://www.seeedstudio.com/XIAO-ESP32S3-p-5627.html |
| 3.7V 300mAh 502035 LiPo | 1 | https://a.co/d/0aBGCeCJ | 
| Round LCD Module GC9A01 | 1 | https://www.aliexpress.us/item/3256804295713253.html?spm=a2g0o.productlist.main.2.37dd5e3dMBryvp&algo_pvid=373884e1-f59c-4b72-a437-bb5c888fe552&algo_exp_id=373884e1-f59c-4b72-a437-bb5c888fe552-1&pdp_ext_f=%7B%22order%22%3A%227427%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21USD%213.16%213.16%21%21%213.16%213.16%21%402101ea8c17831339121663052ef6b1%2112000049978631702%21sea%21US%216414118479%21X%211%210%21n_tag%3A-29919%3Bd%3A39b1dfd6%3Bm03_new_user%3A-29895&curPageLogUid=j9He0oLnZBlu&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005004482028005%7C_p_origin_prod%3A#nav-specification |
| LRA Haptic Motor| 1 | https://www.adafruit.com/product/1201 | 
| PCT2075D Temperature Sensor| 1 | https://www.lcsc.com/product-detail/C126672.html?s_z=n_q_LSM6DSL&spm=wm.fly.bg.0.xh&lcsc_vid=E1MMAQdWQQNXX11WRARZAgJTEVVaUFcEQQBZUQcARVYxVlNeRldYUFFfQFZZUTsOAxUeFF5JWBYZEEoKFBINSQcJGk4eFQsCAgIaSgADAwAHC0slQ1NaV11VWQkaCgg%3D |
| Plastic Case | 3D - Printed |

> [!NOTE]
> Estimated Total Price: ~$30.58 USD
> The Resistors and Capacitors will be ordered with JLCPCB
