# IGME470 Blog Post 3 - Variable Resistor
I set up a basic circuit with the tempature sensor [TMP36](https://www.adafruit.com/product/165?gclid=EAIaIQobChMIsdXDwLup2QIVCYrICh3CRAAwEAQYAyABEgLHNfD_BwE). Despite my best efforts, I was not able to get the serial reading to go above 215 (which is the tempature of my hands) or below 200 (blowing on it). It clearly doesn't fluctuate very much as that was roughly a change of 30 degrees F. It would take a much more signifigant temperature change go above 250 or below 100. According to Adafruit the tempature sensor only ranges between -50C and 125C at 0V output and 1.75V output respectivly. I can't think of any future project that could use such a range, so I do not plan on using it.
 
# IGME470 Blog Post 2 - Blink
For this project I created a game out of some LEDs and an arduino Nano. This game has 2 players fight for best of 5  in 1 dementional pong. Each player must press their button when the light is in one of their 2 red LEDs to send the light back. Each bounce increases the speed of the light untill someone fails to hit the button on time.
 
![Boxed Image](https://github.com/BrokenScience/IGME470/blob/master/Images/BlinkBox.jpg)
This particular image shows the box the components are in with the LEDs sticking out of the top of the box with the buttons out to the side.
 
![UnBoxed Image](https://github.com/BrokenScience/IGME470/blob/master/Images/BlinkUnBox.jpg)
This picture shows the wiring to the Arduino Nano in the bread boards as well as the resistors and button wiring. It doesn't look quite as glamorous as when concealed within the box.
 
The code to for the Arduino can be found [here](https://github.com/BrokenScience/IGME470/blob/master/Projects/Blink.ino)

### Future Improvements
- More LEDs. 9 is not enough in my opinion
- Smoother led transitions using the Analog output. It would look much cooler if the light was more continuous rather than discrete
- Clean up code. Code is a mess to look at. I would clean it up more if I had more time.
-  A nicer box. This one did not go as well as I had hoped.
- Tweak the values. More Speed Increase Faster? Longer Animations? Perhaps
- Learn to Photo More Better
 
 ### Plan vs Product
 Overall the product is nearly identical to my plan. I did want more LEDs however, and was limitied by the number of pins on the Arduino as well as space on the breadboard. I had hoped the buttons would be a bit bigger and more visible, but I did not have time to make custom ones and had to use the ones in the kit. Most of the animations in the product when a point is scored or the game is won were not part of the original plan. I liked how they turned out however and they fit quite nicely.
 
# IGME470 Blog Post 1 - Invisible Waves
[![Invisible Waves Image](https://i.vimeocdn.com/filter/overlay?src0=https%3A%2F%2Fi.vimeocdn.com%2Fvideo%2F665004012_1280x720.jpg&src1=https%3A%2F%2Ff.vimeocdn.com%2Fimages_v6%2Fshare%2Fplay_icon_overlay.png)](https://vimeo.com/241423111)\
This project takes a recording of sound from a mobile sound recorder and plays back the sound on aluminum foil wraped around a disk. The vibrations from the playback cause the aluminum foil to bend and fold leaving it shaped by the sounds that most people don't pay much attention to.
