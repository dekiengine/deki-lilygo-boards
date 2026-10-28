# Changelog

Notable changes to `deki-lilygo-boards`. Engine and editor changes are in the
[engine changelog](https://github.com/dekiengine/deki-engine/blob/master/CHANGELOG.md).

A package's `minEngine` names the engine version it needs. Before 1.0 a
breaking change bumps the minor across the editor, the engine and every
package together, so a package with no changes of its own is still released
alongside one that has them.

## 0.21.0

### Added
- The first T-Deck, `lilygo_t_deck`: the T-Deck Plus's boot scene without the
  GPS, whose pins are the T-Deck's Grove port.

### Changed
- `minEngine` 0.21.0: built against engine 0.21.0. A build of this package
  from before does not load and is rebuilt.

### Fixed
- Colours on T-Decks whose panel takes each pixel's bytes the other way round
  (dark colours pink, white green). LilyGO's start-up sequence, which the
  boards use, never set the panel's byte order, so it kept the one it powered
  up with; it is set now (RAMCTRL `B0 00 F0`). Tested on a first T-Deck.

## 0.20.0

### Added
- The T-Deck Plus is tear-free: its panel's TE line is not wired, so its
  display reads the panel's scanline instead (`scanlineSync`, answering on
  the data line, `readsOnMosi`). A new scene no longer shows as a diagonal
  wipe.
- Both boards say how big their screens are, `screenDiagonalInches`: 2.8 on
  the T-Deck Plus, 1.91 on the T-Display S3 AMOLED. Scenes use it to lay out
  for the AMOLED's small glass; an editor without it ignores the key.

### Changed
- `minEngine` 0.20.0: built against engine 0.20.0. A build of this package from before does not load and is rebuilt.
- No longer requires deki-lovyangfx-integration: both boards run on
  deki-display. A project whose own copy of a board still uses LovyanGFX
  components stops building until it takes **Update** on that copy (which
  moves it to deki-display) or adds deki-lovyangfx-integration itself.
- **T-Deck Plus: on deki-display, Deki's own display library, instead of
  LovyanGFX.** deki-spi's `SPIBusComponent` starts SPI2 (SCK 40, MOSI 41,
  MISO 38) before the display; the display, the SD card and the LoRa radio
  join that bus. The display is deki-display's `DisplayPanelComponent`,
  started with the T-Deck's own `initSequence` (the values of
  `ST7789TDeck`), and its touch is deki-display's `TouchPanelComponent`
  (GT911, INT on GPIO 16) on the I2C bus the keyboard shares. Measured on a
  T-Deck Plus with the frame rate uncapped: 63.2 fps in whole frames with
  10 KB more internal RAM free, and 61.0 in strips (LovyanGFX: 62.6 and
  60.2). The target requires deki-display. A project's own copy of the T-Deck
  target takes the change with **Update**.
- **T-Display S3 AMOLED: on deki-display too.** A quad SPI bus (deki-spi's
  `SPIBusComponent` with IO2 and IO3), the RM67162 over QSPI through
  `DisplayPanelComponent`, and the CST816T through `TouchPanelComponent`,
  with the same pins, rotation and touch bounds as before. The package
  still requires deki-lovyangfx-integration, so a project whose own copy of
  a board still uses its LovyanGFX components keeps building until it takes
  **Update**. With the panel's tearing-effect line (GPIO 9) as `tePin`, it no
  longer tears: frames start on the panel's refresh (59.6 Hz) and go in its
  own row order, drawn in strips of columns. 59.5 fps in strips.

### Fixed
- T-Deck Plus: the screen kept a fading ghost of what it showed before, such
  as the menu after opening a demo. LovyanGFX's ST7789 init set the panel's
  VCOM to 0x28 (1.1 V); the display is the `ST7789TDeck` panel, started with
  the voltages and gamma of LilyGO's own setup for the board and VCOM 0x19
  (0.725 V), with `rgbOrder` on. Requires deki-lovyangfx-integration with
  `ST7789TDeck`.

## 0.19.0

### Changed
- `minEngine` 0.19.0: built against engine 0.19.0, whose component metadata records the package each class is in. A build of this package from before does not load and is rebuilt.

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
