# ESP32 Deep Sleep Demo

You can watch a video walkthrough of this code [here](https://youtu.be/YOjgZUg_skU)

[![Demo Video](https://img.youtube.com/vi/YOjgZUg_skU/0.jpg)](https://www.youtube.com/watch?v=YOjgZUg_skU)

## Porting to another board

You will probably want to change this line:

```
#define BUTTON_GPIO GPIO_NUM_35
```

To match one of the buttons on your board.

## Deep sleep using a timer to wake up

Make sure this is uncommented:

```
#define DEEP_SLEEP_TIMER
```

When you push the button, the ESP32 will enter deep sleep for 30 seconds.

## Deep sleep using a single GPIO pin to wake up

Make sure this is uncommented:

```
#define DEEP_SLEEP_EXT0
```

The ESP32 will wake up when the button is pressed.

## Deep sleep using multiple GPIO pins to wake up

There are two options here:

`#define DEEP_SLEEP_EXT1_ANY_HIGH`

This will wake the ESP32 if either pins 12 or 15 go high.

Or:

`#define DEEP_SLEEP_EXT1_ALL_LOW`

This will wake the ESP32 when both pins 12 and 15 are taken low.

# Deep sleep using touch to wake up

Uncomment:

```
#define DEEP_SLEEP_TOUCH
```

The device will wake up when touch pin 9 is touched (GPIO 32). You can just connect a wire to pin 32 and when you touch the wire the device will wake up.
