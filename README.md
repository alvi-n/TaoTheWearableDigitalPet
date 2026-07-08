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
