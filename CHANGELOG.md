# Changelog

Notable changes to `deki-lilygo-boards`. Engine and editor changes are in the
[engine changelog](https://github.com/dekiengine/deki-engine/blob/master/CHANGELOG.md).

A package's `minEngine` names the engine version it needs. Before 1.0 a
breaking change bumps the minor across the editor, the engine and every
package together, so a package with no changes of its own is still released
alongside one that has them.

## Unreleased

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
