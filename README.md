# EE4370GroupProject

This repo is for the EE/CE 4370 Group 1 project: an ESP32 Intruder Alarm system. The system
alerts the user whenever their door opens by sending an email. Sounds simple, right?

## How it works
The intruder alarm system works by using a reed switch and a magnet. The read switch
is made with two pieces of metal that overlap each other. They can be open or closed,
depending on what configuration you buy. These pieces of metal are thin enough to be
affected by a magnet. Whenever a strong enough magnetic force is applied to the
reed switch, the two pieces of metal will open or close. We use the ESP32 to sense
this change in state.
Next, we have an LED indicator. Whenever the reed switch changes state, we will
light up an LED and make sure that it has some sort of current-limiting resistor
such as a 220 or 330 Ohm one.
What if you don't want an email all the time, though? Well, why not a button that
arms the device and one that disarms it? Done. We add two buttons and have
the ESP32 detect and debounce those buttons.
Lastly, the project allows a time frame to disarm the device. Any time the device
changes state (door opening or closing), the user will have a set amount of time
to press a disarm button so you are not sent an email. How do you know you can
still disarm it? There is a red LED that lights up to show state transition!
## Design Challenges
When designing out intruder alarm, we tried to do everything one-by one, making sure
that everything worked by itself as we added more complexity. It was kind of a cyclic
cycle, actually.

The first thing the board needed, however, was a deep sleep or low power mode. Our first
iteration of this was a light sleep mode, but that still had too much current draw
if we were to have this run on an external battery. Why have a product that requires
you to look after it so often?

This was a challenge to get it into deep sleep mode, as this offered us some of the best
battery life. Whenever the device woke up from a deep sleep mode, the memory contents
would be wiped clean. Then we looked closer at how deep sleep worked and found that
it still uses RTC peripherals such as RTC memory. This was incredibly important because
it allowed use to use flags that did not change upon a reset.

From there, it came down to algorithms in our code and using polls to decide what to do.
We then set priorities by adding polls in order of most to least importance. We then
ran into further trouble with disarming and arming the buttons properly, though.
Thinking about assembly and wishing for a jump instruction, though, Michael remembered
that he could use labels and the goto keyword, which helped fix our code.
Yes, goto an be obfuscating, but it helped readability and avoided some hazards in our
code here.

The last problem we had was the LED. During deep sleep mode, the LED was still dimly lit.
Obviously not a good idea for battery life. This may have been caused by the internal
pullup resistor in the ESP32. So... To fix that, we used a resistive network and an
NPN transistor to shunt current away from the LED. And there we have it: an electronic
switch for the LED + a current limiter.

Our last concern was how to get the device running on a battery. Should we use LiFePO4,
Li ion, LiPO, or what? What about an LDO voltage regulator? What was necessary? We only
had a month to work on the project, afterall.
Turns out that we could just use a 3.3V breakout board with a 9V battery, but a lot of
research did help understand what was and wasn't dangerous to the board. That, and what
was inefficient and wasting power. But as far as getting the board off of the 5V USB
power, this was a quick and dirty solution.
## Future plans
As far as future plans go, Michael has an AI-Thinker ESP32 Cam Module that we can
use to take pictures and then send over email with. The problem is positioning.
How do we get a proper picture? What if we use a relay and separate boards?
Maybe a long wire to the reed switch? Thoughts for another time.

The other thing is to give the board a better power source. We could make a PCB for
the board so that we can use SMD components and get a new ESP32 chip, or we could
use the PCB as a custom power source.

