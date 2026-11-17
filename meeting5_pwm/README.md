# Meeting 5: PWM with Arduino

This meeting is about the **idea** of PWM (pulse width modulation): how a pin that can only be
ON or OFF makes an LED look dim, or fade, by switching very fast. No registers today. We use the
**Arduino framework** on the Nucleo, so you write the same `analogWrite()` code Arduino users do.

This folder is its **own PlatformIO project** (it has its own `platformio.ini`), separate from the
bare-metal challenges one folder up.

---

## Quick start

1. You need VS Code + the PlatformIO extension from the main [README](../README.md#1-install-the-tools).
2. In VS Code: **File → Open Folder…** and pick **`meeting5_pwm`** (the folder that contains this
   README and a `platformio.ini`). Not the main repo folder.
3. Plug in the Nucleo.
4. In the blue bar at the bottom, pick `env:challenge9`, then click **→ (Upload)**.

> **Do step 4 before the meeting.** The first build downloads the Arduino framework for STM32
> (a few hundred MB) and compiles it, which takes several minutes. After that it takes seconds.

Prefer a terminal? From inside this folder:

```bash
pio run -e challenge9 -t upload
```

Getting new challenges: `git pull` in the main repo, same as always.

---

## The challenges

Work in pairs. Each file starts with instructions, wiring and a **DONE WHEN** line.

| # | File (in `src/`) | You'll learn | Wiring |
|---|---|---|---|
| 9 | `challenge9_pwm_basics.cpp` | Duty cycle = brightness, frequency = flicker | LED on D12 |
| 10 | `challenge10_pwm_fade.cpp` | Fading with a loop; why a straight ramp looks uneven | LEDs on D12, A1 |
| 11 | `challenge11_pwm_timers.cpp` | One frequency per timer, one duty per channel | LEDs on A0, A1, D12 |
| 12 | `challenge12_pwm_scanner.cpp` | **Stretch:** a scanner light with a fading tail | LEDs on A0, A1, D12 |
| — | `playground.cpp` | Anything you want | Your call |

## Wiring

Use the same LEDs as the earlier meetings. Every LED is wired the same way:

```
 Nucleo pin ──► LED long leg (+)    LED short leg (–) ──► 220–330 Ω resistor ──► GND
```

PWM works on **A0, A1 and D12**. Those already have LEDs from meetings 1 to 4. (A2 has one too,
but it can't do PWM. See below.)

---

## PWM cheat sheet

```
 value  25   ┌┐        ┌┐        ┌┐         frequency = on/off cycles per second (Hz)
             ┘└────────┘└────────┘└──       duty cycle = share of each cycle the pin is ON
 value 128   ┌───┐     ┌───┐     ┌───┐      your eye only sees the AVERAGE brightness
             ┘   └─────┘   └─────┘   └─
 value 230   ┌────────┐┌────────┐┌───
             ┘        └┘        └┘
```

```cpp
analogWrite(D12, 128);          // brightness 0 to 255.  0 = off, 255 = fully on, 128 = about half
analogWriteFrequency(1000);     // PWM cycles per second (default 1000). Call it BEFORE analogWrite
analogWriteResolution(12);      // optional: analogWrite now takes 0 to 4095 instead of 0 to 255
```

- You don't need `pinMode()` before `analogWrite()`: it sets the pin up the first time you use it.
- **duty cycle = value ÷ 255.** `analogWrite(pin, 25)` is about 10 %.

## Which pins can do PWM?

A pin can only do PWM if a hardware **timer** is wired to it. The Nucleo labels its headers like an
Arduino, but only some pins have a timer:

| Board label | Chip pin | Timer, channel |
|---|---|---|
| **A0** | PA0 | TIM2, channel 1 |
| **A1** | PA1 | TIM2, channel 2 |
| **D12** | PA6 | TIM3, channel 1 |
| D9 | PC7 | TIM3, channel 2 |
| D10 | PB6 | TIM4, channel 1 |

Three things to know:

1. **Pins on the same timer share one frequency** (A0 and A1 are both on TIM2), but each channel
   has its own duty. Pins on different timers are independent. Challenge 11 is all about this.
2. **Two pins that need the same timer channel can't both be used.** For example A1 and D3 are both
   TIM2 channel 2, and D12 and D5 are both TIM3 channel 1. Pick one.
3. **D13 (the green LED) and A2 are DAC pins, not PWM pins.** On this board, `analogWrite()` on a
   pin that has a DAC uses the DAC, which outputs a steady voltage, not PWM. That's why the lesson
   uses D12, A0 and A1 and not the onboard LED. (An LED needs about 2 V to light at all, so a steady
   voltage looks very different from PWM. Challenge 11 has a bonus about this.)

Pin names like `D12` and `A1` are the labels printed on the board next to the headers.

## How does it work underneath?

`analogWrite()` starts a **timer** (the same kind of counter you used in Meeting 4: PSC, ARR,
CNT) and gives the pin a **compare value**. While the counter is below the compare value the pin is
high, and above it the pin is low:

```
 count:   0 1 2 3 4 5 6 7 8 9 ... ARR  0 1 2 3 ...
 pin:     ┌───────────┐                ┌───────────
          ┘           └────────────────┘
          count < compare -> HIGH      count >= compare -> LOW
```

- **ARR** sets the period, so it sets the **frequency**.
- The **compare value** sets how long the pin stays high, so it sets the **duty cycle**.

One timer has one ARR, so one frequency. Each channel has its own compare value, so its own duty.
That's the whole reason for rule 1 above.

---

## Troubleshooting

**The LED stays dark or full on**
1. Is it on A0, A1 or D12? Other pins (D13, A2...) don't do PWM here.
2. Is `analogWrite` getting a value from 0 to 255? (That's the range at the default 8-bit resolution.)
3. LED backwards, or resistor not reaching GND? Same checks as the other meetings.

**Changing the frequency changed another pin too** (Challenge 11)
Expected. `analogWriteFrequency()` is one setting for the whole program, and `analogWrite()` applies
it to that pin's timer every time you call it. Pins on the same timer follow along.

**The first build takes forever, or fails to download**
It's downloading the Arduino framework. Check your internet, and give it several minutes. If it
fails, run the build again.

**Build: "env:challengeX not found"**
Make sure you opened the `meeting5_pwm` folder (the one that contains this README's
`platformio.ini`), not the main repo folder.

**Upload fails / "no ST-LINK detected"**
Same fixes as the main [README](../README.md#troubleshooting): try another USB cable, close other
programs using the board, hold RESET while you start the upload.
