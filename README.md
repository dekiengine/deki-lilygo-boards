# Deki LilyGO Boards

Build targets for LilyGO boards. Install the package and the boards appear in the Build panel's target list; choosing one copies it into your project with a boot scene that brings the display and touch up.

| Target | Board | Display | Touch |
| --- | --- | --- | --- |
| `lilygo_t_deck` | T-Deck | 320x240 ST7789, SPI, through deki-display | GT911 through deki-display, plus the keyboard, trackball and SD card |
| `lilygo_t_deck_plus` | T-Deck Plus | 320x240 ST7789, SPI, through deki-display | GT911 through deki-display, plus the keyboard, trackball and SD card |
| `lilygo_t_display_s3_amoled` | T-Display S3 AMOLED 1.91" | 536x240 RM67162 AMOLED, QSPI, through deki-display | CST816T (touch version), through deki-display |

Every board runs on Deki's own display library, deki-display. A project whose own copy of a board still uses LovyanGFX components needs **Update** on that copy, which moves it to deki-display, or `deki-lovyangfx-integration` added by hand.

All of them are ESP32-S3 boards with 16 MB flash and 8 MB octal PSRAM, built with ESP-IDF.

**Status: every board runs on hardware, touch included.** Pins and panel settings come from LilyGO's own sources (listed under each board). If something is wrong on your board, fix it in your project's copy and please open an issue.

[Deki](https://dekiengine.com) is a modular C++ engine for embedded systems and desktop, with a visual editor. Every capability is a package, and a board is one too: this package has no code, only a description of each board and the scene that boots it.

## Install

Package Manager in the Deki Editor, or `DekiEditor --packages-add deki-lilygo-boards <project>`. It brings `deki-esp32-integration`, `deki-display`, `deki-input`, `deki-i2c`, `deki-gpio` and `deki-sdcard` with it. The keyboard and trackball are `deki-input` features that need the I2C and GPIO packages, which is why those two are listed.

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
3. SPI bus: SPI2 (SCK 40, MOSI 41, MISO 38), started by deki-spi's `SPIBusComponent` before everything on it, so the display can send its pixels in 32 KB transfers. The display, the SD card and the LoRa radio share it.
4. Display: deki-display's `DisplayPanelComponent`. ST7789, 240x320 shown in landscape, at 80 MHz (DC 11, CS 12), backlight on GPIO 42. Its `initSequence` is LilyGO's own start-up for this panel (their TFT_eSPI setup for the board, INIT_SEQUENCE_2) with VCOM 0x19, tuned on a T-Deck Plus: the generic ST7789 start-up leaves a fading ghost of the last image. It also sets the panel's byte order (RAMCTRL, `B0 00 F0`): without it the panel keeps the one it powers up with, and some T-Decks power up the other way round, which shows dark colours pink and white green. Each frame is sent from the other core while the next is drawn (`asyncPresent`), from two frame copies in PSRAM. The panel's TE line is not wired, so frames are kept in step with its refresh by reading its scanline back (`scanlineSync`), on its data line at 10 MHz (`readsOnMosi`): no frame tears.
5. I2C bus: SDA 18, SCL 8.
6. Touch: deki-display's `TouchPanelComponent`. GT911 at 0x5D, interrupt on GPIO 16, read through the I2C bus the keyboard shares.
7. Keyboard: the board's own keyboard controller, on the same I2C bus at 0x55. It is still starting at this point, so it is looked for over the first few seconds.
8. Trackball: GPIO 3, 15, 1, 2 for up, down, left, right, click on GPIO 0. A pointer that glides like a ThinkPad TrackPoint (`glideMs` 120, `acceleration` 3), with the click as a mouse button; set `mode` to `Keys` in your copy for the arrow keys and Enter instead.
9. SD card: on the display's SPI bus, chip select GPIO 39, as `S:/`. A missing card is logged and the boot carries on (`required` is off). The game's assets are in the flash, so a game runs with no card in; a game too big for the flash can keep them on the card instead (Storage > Assets on: External storage), and then needs one.
10. GPS: UART 1, receiving on GPIO 44 and sending on 43. The board ships with an L76K (9600 baud) or a u-blox M10 (38400), so the speed is found by itself (`baudRate` 0).
11. LoRa: SX1262 on the display's SPI bus, CS 9, BUSY 13, RESET 17, DIO1 45, a 1.8 V TCXO on DIO3 and DIO2 switching the antenna. Set for Europe: 868.1 MHz, 14 dBm, 1% duty cycle. Change `frequencyHz` (and the power and duty cycle) in your copy for another region, such as 915 MHz in the Americas.

Set up by this package: display, backlight, touch, keyboard, trackball, SD card, GPS, LoRa. Keys arrive through `deki-input` like a desktop keyboard's: letters, digits, symbols, Enter, Backspace, Space, arrows.

Not set up: speaker, microphone, battery reading (GPIO 4).

Sources: [T-Deck `utilities.h`](https://github.com/Xinyuan-LilyGO/T-Deck/blob/master/examples/UnitTest/utilities.h), LilyGO's TFT_eSPI setup for the board (Setup210_LilyGo_T_Deck) for the panel's start-up, and the LovyanGFX configuration Meshtastic runs on this board ([`LGFX_T_DECK.h`](https://github.com/meshtastic/device-ui/blob/master/include/graphics/LGFX/LGFX_T_DECK.h)).

## T-Deck

The first T-Deck, before the Plus: the same board as the T-Deck Plus above, step for step, without the GPS. The Plus has its GPS on GPIO 43 and 44; on the T-Deck those pins are the Grove port, left free for you.

Colours that look wrong on a T-Deck of either kind (dark colours pink, white green) mean the panel reads each pixel's two bytes the other way round. The boot scene sets the panel's byte order to stop that; a panel that still shows it needs **Swap Bytes** on in the display's settings.

Tested on hardware by a user, on a first T-Deck, with the T-Deck Plus's boot scene.

## T-Display S3 AMOLED

Boot order:

1. GPIO 38 high. The panel has no supply until it is.
2. SPI bus: SPI2 as a quad bus (SCK 47, IO0 18, IO1 7, IO2 48, IO3 5), started by deki-spi's `SPIBusComponent`.
3. Display: deki-display's `DisplayPanelComponent`. RM67162 over QSPI, 240x536 shown in landscape, at 75 MHz (CS 6, reset 17). The frame buffer lives in PSRAM; each strip is sent from the other core while the next is drawn (`asyncPresent`). The panel's tearing-effect line, GPIO 9, is `tePin`: frames start on the panel's refresh and go in its own row order, so nothing tears, even swiping sideways.
4. I2C bus: SDA 3, SCL 2.
5. Touch: deki-display's `TouchPanelComponent`. CST816T at 0x15, interrupt on GPIO 21. It sleeps, and does not answer on I2C, until it is first touched, so it is set up then; on the version without touch nothing ever answers, and the boot is not slowed by it.

An AMOLED has no backlight pin. Brightness is a panel command (0x51), which deki-display sends when the engine turns the screen off and on.

The board has no SD card slot, so the game's assets are always in its flash (about 13 MB of the 16 MB).

The RM67162 ignores a write whose start or size is odd along its short axis. deki-display widens partial screen updates to even rows on this panel, so there is nothing to do about it in a game.

Sources: [LilyGo-AMOLED-Series `LilyGo_AMOLED.h`](https://github.com/Xinyuan-LilyGO/LilyGo-AMOLED-Series/blob/master/src/LilyGo_AMOLED.h) (`BOARD_AMOLED_191`), and LovyanGFX's own `lgfx_user/Lilygo_T_Display_S3_AMOLED.hpp`.

## Every board

The serial console is the ESP32-S3's own USB port (`CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG`). None of them has a USB-UART bridge.

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
