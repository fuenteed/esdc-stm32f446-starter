# ESDC STM32F446RE Starter

Starter code for the **Embedded Systems Design Club** bare-metal challenges on the
**Nucleo-F446RE** board. No Arduino, no HAL: you write straight to the chip's registers,
and every file that runs on the chip is in this repo.

Works the same on **Windows, macOS and Linux**.

---

## Quick start

1. Install the tools ([below](#1-install-the-tools)). Do this **before** the meeting.
2. Get the code:
   ```bash
   git clone https://github.com/fuenteed/esdc-stm32f446-starter.git
   ```
   (No git? Click the green **Code** button on GitHub → **Download ZIP**, then unzip.)
3. In VS Code: **File → Open Folder…** and pick the `esdc-stm32f446-starter` folder
   (the one that contains `platformio.ini`).
4. Plug in the Nucleo with a USB cable.
5. Click **→ (Upload)** in the blue bar at the bottom of VS Code.
   The green LED on the board should start blinking. You're set up.

The first build downloads the ARM compiler and takes a few minutes. After that it takes seconds.

---

## 1. Install the tools

| | Windows | macOS | Linux |
|---|---|---|---|
| **VS Code** | [code.visualstudio.com](https://code.visualstudio.com) | same | same |
| **PlatformIO IDE** (VS Code extension) | Extensions tab → search "PlatformIO IDE" → Install | same | same |
| **Git** (optional) | [git-scm.com](https://git-scm.com) | `xcode-select --install` | `sudo apt install git` |
| **USB driver** | Usually automatic. If upload can't find the board, install ST's [STSW-LINK009](https://www.st.com/en/development-tools/stsw-link009.html) driver | Nothing needed | Install PlatformIO's udev rules (see [Troubleshooting](#troubleshooting)) |

After installing PlatformIO, **restart VS Code**. You'll see an alien-head icon in the left sidebar.

If you set up PlatformIO for meeting 1, you already have everything.

---

## 2. Build and upload

Each challenge is a separate PlatformIO **environment**. Pick which one you're working on
in the blue status bar at the bottom of VS Code:

```
 [✓ Build] [→ Upload] ... [ env:challenge0 ▾ ]   <- click this to switch challenge
```

- **✓ Build**: compile only. Use this to check for errors.
- **→ Upload**: compile and flash the board.

Prefer a terminal? These do the same thing on any OS:

```bash
pio run -e challenge1               # build challenge 1
pio run -e challenge1 -t upload     # build + flash challenge 1
```

---

## 3. The challenges

Work in pairs. Each challenge file starts with instructions, wiring, and a **DONE WHEN** line,
and has hints you can read one at a time if you're stuck.

| # | File (in `src/challenges/`) | You'll learn | Wiring |
|---|---|---|---|
| 0 | `challenge0_blink.c` | Check your setup (nothing to write) | None |
| 1 | `challenge1_port_a.c` | Bit masking: set, clear, toggle | 4 LEDs: A0, A1, A2, D12 |
| 2 | `challenge2_other_ports.c` | Finding registers in the reference manual, clocks | 2 LEDs: D4, D9 |
| 3 | `challenge3_button.c` | Reading input, edge detection | Same 4 LEDs + blue button |
| 4 | `challenge4_timer.c` | **Stretch:** a real hardware timer (TIM2) | None (onboard LED) |
| — | `playground.c` | Anything you want | Your call |

> **Challenge 2 won't compile until you fill in the blanks.** That's on purpose. The error
> message (`'FILL_ME_IN_...' undeclared`) tells you what's left to find.

### Bit masking cheat sheet

```c
REG |=  (1U << n);          // set bit n (turn ON), leave the rest alone
REG &= ~(1U << n);          // clear bit n (turn OFF), leave the rest alone
REG ^=  (1U << n);          // toggle bit n
if (REG & (1U << n)) { }    // test: is bit n a 1?
```

Never write `REG = (1U << n);` to turn one pin on. The plain `=` turns every other pin on that port off.

---

## 4. Wiring

Every LED is wired the same way:

```
 Nucleo pin ──► LED long leg (+)    LED short leg (–) ──► 220–330 Ω resistor ──► GND
```

- The **long leg** of the LED goes toward the pin. Backwards LEDs just stay dark (no damage).
- Any **GND** pin works. There are GND pins on the power header (next to 3V3, 5V) and next to D13.
- On a breadboard, the 5 holes in a short row are connected. The long rails down the sides are connected.

### Which board pins we use

The Nucleo's headers are labelled like an Arduino (A0, D12...). Here's what those labels are on the STM32:

| Board label | STM32 pin | Used in |
|---|---|---|
| A0 | PA0 | Challenges 1, 3 |
| A1 | PA1 | Challenges 1, 3 |
| A2 | **PA4** (not PA2!) | Challenges 1, 3 |
| D12 | PA6 | Challenges 1, 3 |
| D4 | PB5 | Challenge 2 |
| D9 | PC7 | Challenge 2 |
| D13 / green LED LD2 | PA5 | Everything |
| Blue USER button B1 | PC13 (reads 0 when pressed) | Challenges 3, 4 |

**Avoid these pins:** PA2/PA3 (D1/D0, used by the USB serial port), PA13/PA14 (debugger),
PC14/PC15 and PH0/PH1 (crystal oscillators).

Full pinout: ST user manual **UM1724** (Nucleo-64 boards), section "Extension connectors".

---

## 5. What's in this repo

```
esdc-stm32f446-starter/
├── platformio.ini            # build settings + one env per challenge
├── include/
│   ├── esdc.h                # include this; cheat sheet, delay_ms(), PIN_OUTPUT()
│   └── esdc_registers.h      # RCC and GPIOA register addresses, explained
├── src/
│   ├── challenges/           # <- your work goes here
│   ├── esdc_helpers.c        # delay_ms() using the SysTick timer
│   └── startup_stm32f446.c   # what runs before main() (worth reading later!)
├── linker/stm32f446re.ld     # where code and variables go in memory
└── Makefile                  # optional: build without PlatformIO
```

There's no hidden framework. When the board powers on, it runs `Reset_Handler()` in
`startup_stm32f446.c`, which sets up memory and calls your `main()`. That's the whole chain.

The chip runs on its internal **16 MHz** clock the whole time (we never change it).
Timer math in Challenge 4 depends on that number.

---

## Troubleshooting

**Upload fails / "no ST-LINK detected" / "LIBUSB_ERROR"**
- Try another USB cable. Many cables are charge-only and carry no data.
- Windows: install the [STSW-LINK009](https://www.st.com/en/development-tools/stsw-link009.html) driver and replug.
- Linux: install the udev rules, then unplug and replug:
  ```bash
  curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
  sudo udevadm control --reload-rules && sudo udevadm trigger
  ```
- Close anything else that might be using the board (STM32CubeIDE, Arduino IDE, another VS Code window).

**It builds and uploads, but my LED stays off**
1. Is the port's **clock** on? (`RCC_AHB1ENR` bit 0 = A, 1 = B, 2 = C.) This is the #1 cause.
2. Is the pin set to **output**? (`PIN_OUTPUT(GPIOx_MODER, pin)`)
3. Is the LED **backwards**? Flip it.
4. Is the LED on the **pin you think**? Check the table above. A2 is PA4!
5. Are the LED and resistor in the **same breadboard row**, and does the resistor reach **GND**?

**The pin's LED turned on, but another one turned off**
You used `=` instead of `|=` or `&= ~`. See the cheat sheet.

**Challenge 2 says `FILL_ME_IN... undeclared`**
Expected. Replace every `FILL_ME_IN` with the number from the reference manual.

**Button presses get missed or count twice** (Challenge 3)
Missed: you're checking the button only between long `delay_ms()` calls. Check it every 10 ms.
Counted twice: you're reacting to "is pressed" instead of "just got pressed" (edge detection, Step C).

**"Error: init mode failed" or the board seems stuck**
Hold the black **RESET** button, click Upload, and let go of RESET when the upload starts.

**Build: "env:challengeX not found"**
Make sure you opened the folder that contains `platformio.ini`, not a parent or child folder.

---

## Reference documents

All free from st.com (search the code):

- **RM0390**: STM32F446 Reference Manual. Every register. ("The holy grail.")
- **UM1724**: Nucleo-64 user manual. Board pinout, LEDs, button.
- **DS10693**: STM32F446 datasheet. Pin functions, electrical limits.
- **PM0214**: Cortex-M4 programming manual. SysTick, interrupts, the CPU core.

---

## Officers

- **Solutions** live on the `solutions` branch: `git checkout solutions`.
  Challenge 3's solution includes the "Stop the light" game, and Challenge 4's includes the reaction timer.
- **No PlatformIO?** If you have `arm-none-eabi-gcc` and OpenOCD installed:
  `make CHALLENGE=1` builds, `make CHALLENGE=1 flash` uploads.
- Every push is build-checked by GitHub Actions (see the **Actions** tab).
