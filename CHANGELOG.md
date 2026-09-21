# Changelog

Notable changes to `deki-lilygo-boards`. Engine and editor changes are in the
[engine changelog](https://github.com/dekiengine/deki-engine/blob/master/CHANGELOG.md).

A package's `minEngine` names the engine version it needs. Before 1.0 a
breaking change bumps the minor across the editor, the engine and every
package together, so a package with no changes of its own is still released
alongside one that has them.

## Unreleased

### Added
- **LilyGO T-Deck Plus** (`lilygo_t_deck_plus`): 320x240 ST7789 over SPI,
  GT911 touch, the peripheral power rail and the shared SPI bus's other chip
  selects handled at boot.
- **LilyGO T-Display S3 AMOLED 1.91"** (`lilygo_t_display_s3_amoled`): 536x240
  RM67162 AMOLED over QSPI, CST816T touch, panel power at boot.

Both build. Neither has been run on hardware yet.
