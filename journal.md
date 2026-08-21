# Digital Pet PCB — Build Log

## No modules, everything on a PCB. *(1 hr)*

I've been seeing these digital pet accessories, similar to a Tamagotchi, on my feed a lot and the company selling these were listing them for ~$300 and I thought "I could definitely make this myself". These things interact with the environment with some basic sensors which gives the face a signal to change its animation.

Turns out it's wayy harder than I thought to make myself, considering my only hardware project I've made was a macropad.

There is a lot of pieces and I obviously haven't built anything like this ever. Instead of adding modules that hook up to a breadboard, I realized that all of those parts can be added to the PCB itself, and it all starts with the chip.

I looked for the best chip for each of the different features and looked at the schematic for each. The only chip with a datasheet that actually made sense was for the battery charger, the rest... it took a bit more work.

Anyways tomorrow, I'm planning on trying to finish the schematics, so make sure to follow!

<p align="center">
  <img width="600" alt="image" src="https://github.com/user-attachments/assets/f47e9e5f-f162-4aa2-ac23-8309ee1fe169" />
</p>

---

## Tao Now Has an Accelerometer, NFC Reader, and a Temperature Sensor *(2 hr 19 min)*

`all powered by the xiao esp-32 s3`

- I never realized how powerful **development boards** are. Last session, I took an hour trying to figure out how to make a buck converter, battery charger, and an external USB-C connector — turns out the XIAO board does all of that for me.
- Now instead of focusing my time on the power management side of my project, which was really confusing for me, I could now focus on adding additional **fun** elements to my project such as a haptic motor and temperature sensor.
- Overall this project has been pretty fun so far — next step is working on the PCB layout and thinking about the shape of my digital pet.

<p align="center">
  <img width="600" alt="image" src="https://github.com/user-attachments/assets/de2001df-aa9c-4317-8721-08848286373d" />
</p>

---

## A little bit of this and a little bit of that. *(51 min)*

Today I started on my actual PCB design and turns out it's harder than I thought. My schematic is like a maze, some lines in my schematic going two directions. Figuring out the best way to put everything is definitely a challenge.

- Tomorrow, I'll start working on the edge cuts first and try to get 50% of the PCB wired.

<p align="center">
  <img width="450" alt="image" src="https://github.com/user-attachments/assets/6c5d8711-9c77-404c-8066-ddabb32066b5" />
</p>

---

## Outlining my PCB *(30 min)*

Not much was done in these 30 minutes, just imported a drawing of the shape of the pet.

<p align="center">
  <img width="380" alt="image" src="https://github.com/user-attachments/assets/4a246714-6c72-4490-9823-4db3c6f207c5" />
  <img width="380" alt="image" src="https://github.com/user-attachments/assets/2009e6c7-d29b-4615-adf3-8c38647d2c3b" />
</p>

---

## PCB routing - continued *(1 hr 21 min)*

So far I believe I've set up the temperature sensor, motor driver, and accelerometer well — and by that, I mean just the capacitors and resistors.

I'm planning on putting the ESP32 on the back side where I'll also keep the battery so it doesn't interfere with the LCD screen. This has been quite fun so far.

<p align="center">
  <img width="450" alt="image" src="https://github.com/user-attachments/assets/ad1df60e-02ff-4a2c-ad63-972cbe4a6f1a" />
</p>

---

## Honestly, I do not think I can get past the cut *(3 hr 6 min)*

Spent too long trying to figure out how to make my own NFC chip antenna, and for the time I'm trying to rush this project, I don't think I can.

- I think I will end up using a module with header pins for it instead of making it myself. It's really devastating, but for the sake of maintaining my enjoyment throughout this project I'm going to lower my scope.

<p align="center">
  <img width="600" alt="image" src="https://github.com/user-attachments/assets/f2608897-f092-40e3-9143-71c97c90b4e0" />
</p>

---

## Taking out the NFC, Retracing Everything *(1 hr 58 min)*

This took a little too long. Turns out since I didn't have an LCD screen footprint, I didn't really know where the middle of the screen would be relative to the rest of the PCB. When I put everything into my 3D modeling software (Shapr3D), I realized it would be better if the screen was directly in the middle of the PCB by making it vertical — which ultimately means I have to retrace almost everything.

<p align="center">
  <img width="500" alt="image" src="https://github.com/user-attachments/assets/3ad6804b-9b57-4ea8-ab43-b7ffdd55490d" />
</p>

---

## Time skip - I finished all the CAD and code for the project and I think it's about time to submit! *(1 hr 15 min)*

- I added a Charizard silkscreen design and it looks pretty cool.
- The case has a pretty big back for the battery, but I guess it's not too noticeable.
- The main thing I'm not too sure about is the schematic and PCB wiring, but hopefully if the design comes back for a redo I can do that fast.
- This was a really fun project and I'm surprised myself — I was able to remain consistent with this project.

<p align="center">
  <img width="450" alt="pcbwithart" src="https://github.com/user-attachments/assets/a5253b1d-27e9-41b2-be35-65abd492e481" />
  <img width="450" alt="fit" src="https://github.com/user-attachments/assets/2083554b-6917-4791-a13d-fe297f07fe02" />
</p>
