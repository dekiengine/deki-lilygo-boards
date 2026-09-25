# Deki LilyGO Boards

Build targets for LilyGO boards. Install the package and the boards appear in the Build panel's target list; choosing one copies it into your project with a boot scene that brings the display and touch up.

| Target | Board | Display | Touch |
| --- | --- | --- | --- |
| `lilygo_t_deck_plus` | T-Deck Plus | 320x240 ST7789, SPI | GT911, plus the keyboard, trackball and SD card |
| `lilygo_t_display_s3_amoled` | T-Display S3 AMOLED 1.91" | 536x240 RM67162 AMOLED, QSPI | CST816T (touch version) |

Both are ESP32-S3 boards with 16 MB flash and 8 MB octal PSRAM, built with ESP-IDF.

**Status: builds, not yet run on hardware.** Pins and panel settings come from LilyGO's own sources (listed under each board). If something is wrong on your board, fix it in your project's copy and please open an issue.

[Deki](https://dekiengine.com) is a modular C++ engine for embedded systems and desktop, with a visual editor. Every capability is a package, and a board is one too: this package has no code, only a description of each board and the scene that boots it.

## Install

Package Manager in the Deki Editor, or `DekiEditor --packages-add deki-lilygo-boards <project>`. It brings `deki-esp32-integration`, `deki-lovyangfx-integration`, `deki-input`, `deki-i2c`, `deki-gpio` and `deki-sdcard` with it. The keyboard and trackball are `deki-input` features that need the I2C and GPIO packages, which is why those two are listed.

Then pick the board in the Build panel, or:

```
DekiEditor --tool set_active_platform --args '{"platform_id":"lilygo_t_deck_plus"}' <project>
```

## The copy is yours

Choosing a board copies `platforms/<id>/` from this package into your project. Edit it there: the platform in the Build panel's editor, the boot scene like any scene.

When this package updates a board, the Build panel shows **update available**. **Update** takes the package's changes and keeps yours. **Restore defaults** goes back to the package's version. The command line has `update_platform` and `restore_platform`.

## T-Deck Plus

Boot order, as the boot scene lists it:

1. GPIO 10 high. It switches the rail that feeds the display, the touch controller and the keyboard.
2. GPIO 39 and GPIO 9 high. The SD card and the LoRa radio share the display's SPI bus; their chip selects are held high so they stay off it.
3. Display: ST7789, 240x320 shown in landscape, SPI2 at 40 MHz (MOSI 41, MISO 38, SCK 40, DC 11, CS 12), backlight on GPIO 42.
4. I2C bus: SDA 18, SCL 8.
5. Touch: GT911 at 0x5D, interrupt on GPIO 16.
6. Keyboard: the board's own keyboard controller, on the same I2C bus at 0x55. It is still starting at this point, so it is looked for over the first few seconds.
7. Trackball: GPIO 3, 15, 1, 2 for up, down, left, right, click on GPIO 0. As the arrow keys and Enter; set `mode` to `Pointer` in your copy to make it a mouse instead.
8. SD card: on the display's SPI bus, chip select GPIO 39, as `S:/`. A missing card is logged and the boot carries on (`required` is off). The game's assets are in the flash, so a game runs with no card in; a game too big for the flash can keep them on the card instead (Storage > Assets on: External storage), and then needs one.

Set up by this package: display, backlight, touch, keyboard, trackball, SD card. Keys arrive through `deki-input` like a desktop keyboard's: letters, digits, symbols, Enter, Backspace, Space, arrows.

Not set up: LoRa, speaker, microphone, GPS (UART on GPIO 43 and 44), battery reading (GPIO 4).

Sources: [T-Deck `utilities.h`](https://github.com/Xinyuan-LilyGO/T-Deck/blob/master/examples/UnitTest/utilities.h), and the LovyanGFX configuration Meshtastic runs on this board ([`LGFX_T_DECK.h`](https://github.com/meshtastic/device-ui/blob/master/include/graphics/LGFX/LGFX_T_DECK.h)).

## T-Display S3 AMOLED

Boot order:

1. GPIO 38 high. The panel has no supply until it is.
2. Display: RM67162, 240x536 shown in landscape, QSPI on SPI2 at 75 MHz (SCK 47, IO0 18, IO1 7, IO2 48, IO3 5, CS 6, reset 17). The frame buffer lives in PSRAM.
3. I2C bus: SDA 3, SCL 2.
4. Touch: CST816T, interrupt on GPIO 21.

On the version without touch, step 4 logs that nothing answered and the boot carries on. Remove the step from your copy to skip the wait.

An AMOLED has no backlight pin. Brightness is a panel command.

The board has no SD card slot, so the game's assets are always in its flash (about 13 MB of the 16 MB).

The RM67162 ignores a write whose start or size is odd along its short axis. `deki-lovyangfx-integration` widens partial screen updates to even rows on this panel, so there is nothing to do about it in a game.

Sources: [LilyGo-AMOLED-Series `LilyGo_AMOLED.h`](https://github.com/Xinyuan-LilyGO/LilyGo-AMOLED-Series/blob/master/src/LilyGo_AMOLED.h) (`BOARD_AMOLED_191`), and LovyanGFX's own `lgfx_user/Lilygo_T_Display_S3_AMOLED.hpp`.

## Both boards

The serial console is the ESP32-S3's own USB port (`CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG`). Neither board has a USB-UART bridge.

To flash a board that does not show up as a port: hold BOOT (the trackball click on the T-Deck), press RESET, release BOOT.

## Adding a board

One folder per board:

```
platforms/<id>/<id>.json    the platform
platforms/<id>/boot.scene   what brings its hardware up
```

Prefix the id with `lilygo_`. Two installed packages offering the same id is an error, not a choice the editor makes for you.

## License

Apache 2.0. See [LICENSE](LICENSE).
