# Tool to measure USB Keyboard-to-display latency

This tool allows you to measure the latency between a key-press on a USB keyboard and 
the pixels physically changing on your display.  

This works in three basic parts:

- [Board] Emit a key-press event and start a clock
- [PC] Detect key-press event and turn screen white
- [Board] Detect a significant increase in light and stop the clock

This is repeated (configurable) 10 times, and the median value is the result.


## PC

The helper program lives under `display`, is written in C + SDL and can build for Linux, Windows and MacOS.
This program is very simple and only displays a black window, which turns white while spacebar is pressed.


## Board

The code lives in `measure.ino`. The results are displayed on a SSD1306, but it can be replaced for any type of output mechanism.

Using the `Keyboard` Arduino library; simulate a press (and hold!) of the spacebar key until the light sensor goes over the threshold; 
once that happens, send a 'release' event for the spacebar.

The light measurement is very simple: read the value of a light-dependent resistor (LDR).

There is a (terrible) kicad schematic under `board/board`.
