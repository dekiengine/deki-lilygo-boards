# Changelog

Notable changes to `deki-lilygo-boards`. Engine and editor changes are in the
[engine changelog](https://github.com/dekiengine/deki-engine/blob/master/CHANGELOG.md).

A package's `minEngine` names the engine version it needs. Before 1.0 a
breaking change bumps the minor across the editor, the engine and every
package together, so a package with no changes of its own is still released
alongside one that has them.

## Unreleased

### Added
- `lilygo_t_deck_plus_dekidisplay`, a temporary T-Deck Plus target for
  moving the board to Deki's own display library. For now it is
  `lilygo_t_deck_plus` with deki-spi's `SPIBusComponent` starting SPI2
  (SCK 40, MOSI 41, MISO 38) before the display; the display, the SD card and
  the LoRa radio join that bus. The display is deki-display's
  `DisplayPanelComponent`, started with the T-Deck's own `initSequence` (the
  values of `ST7789TDeck`); with no LovyanGFX in the target it has no touch
  yet. Requires deki-display.

### Fixed
- T-Deck Plus: the screen kept a fading ghost of what it showed before, such
  as the menu after opening a demo. LovyanGFX's ST7789 init set the panel's
  VCOM to 0x28 (1.1 V); the display is the `ST7789TDeck` panel, started with
  the voltages and gamma of LilyGO's own setup for the board and VCOM 0x19
  (0.725 V), with `rgbOrder` on. Requires deki-lovyangfx-integration with
  `ST7789TDeck`.

## 0.18.2

### Added
- T-Deck Plus: the GPS (UART on GPIO 43/44, speed found by itself, as the
  board ships with an L76K or a u-blox M10) and the SX1262 LoRa radio (868.1
  MHz, 14 dBm, 1% duty cycle: Europe's rules; change them for other
  regions). Requires deki-gps and deki-lora.
- T-Deck Plus: the keyboard's backlight can be set (`hasBacklight`).
- T-Deck Plus: the trackball is a pointer that glides like a ThinkPad
  TrackPoint, and its click is a mouse click. It was arrow keys.

### Changed
- T-Deck Plus: 60 fps. The display's SPI runs at 80 MHz (it was 40), and
  each frame is sent from the other core while the next is drawn
  (`asyncPresent`): sending a frame took 31 ms, now 16, and no longer adds
  to the drawing. The demo's scenes went from about 16 fps to 62.

### Fixed
- T-Deck Plus: a touch landed away from the finger, further off towards the
  top of the screen. The GT911's range was given as 320x240, the landscape
  screen, but LovyanGFX takes it along the panel's own 240x320 and rotates
  after, so X was stretched by 4/3 and Y squeezed by 3/4. It is 240x320 now.
  A project that adopted the board takes it with **Update** in the Build panel.

## 0.18.0

### Changed
- `minEngine` 0.18.0. Reflection ABI 21: the package must be rebuilt.
- The boot scenes are saved in the current scene format, as the editor writes
  them, so an adopted board matches its package copy.

## 0.17.2

### Changed
- Both boards keep the game's assets in their flash (internal storage), so a
  T-Deck Plus runs a game with no SD card in it; before, it read them from
  the card only. The T-Deck provides `external-storage`, so a game too big
  for the flash can keep its assets on the card instead (Storage > Assets on).
  The T-Display S3 AMOLED, which has no card, can now run a game at all.

## 0.17.1

### Added
- **T-Deck Plus keyboard.** The boot scene gains a keyboard step
  (`DekiInput::I2CKeyboardComponent`, I2C 0x55).
- **T-Deck Plus trackball** (`DekiInput::TrackballComponent`), as the
  arrow keys and Enter; switch it to a pointer in your copy.
- **T-Deck Plus SD card**, on the display's SPI bus. Not required: without a
  card the boot carries on and the assets on it do not load.

### Changed
- Both boot scenes drive their power and chip-select pins with deki-gpio's
  `GpioPinSetup`, which replaced `DekiEsp32::ESP32PinSetup` (scenes naming
  the old one still load).

A project that already uses a board is offered the update in the Build panel.

## 0.17.0

### Added
- **LilyGO T-Deck Plus** (`lilygo_t_deck_plus`): 320x240 ST7789 over SPI,
  GT911 touch, the peripheral power rail and the shared SPI bus's other chip
  selects handled at boot.
- **LilyGO T-Display S3 AMOLED 1.91"** (`lilygo_t_display_s3_amoled`): 536x240
  RM67162 AMOLED over QSPI, CST816T touch, panel power at boot.

Both build. Neither has been run on hardware yet.
