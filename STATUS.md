# zmk-config-lily58 — current state

Extract this folder's contents directly into the root of your
zmk-config-lily58 repo (merge with what's already there).

## What's included
- `config/lily58.conf` — RGB underglow (red, 10% brightness, full
  saturation, SPI3/P0.06 WS2812), `settings_reset` support,
  `CONFIG_USB_DISPLAY_WAKE=y`, and the `zmk-nice-oled` custom status
  screen config (see below).
- `config/lily58.keymap` — original keymap plus the SPI3/WS2812
  devicetree block (pinctrl, &spi3, led_strip, chosen zmk,underglow)
  merged in directly, since the built-in ZMK lily58 shield shadows any
  same-named file placed elsewhere.
- `config/west.yml` — now also pulls in mctechnology17/zmk-nice-oled
  as a module (`main` branch), alongside your existing zmk import.
- `build.yaml` — four build targets: `lily58_left nice_oled`,
  `lily58_right nice_oled`, `settings_reset`.
- `zephyr/module.yml` (repo root) — declares the whole repo as a valid
  Zephyr module, pointing at `modules/usb_display_wake` for its CMake
  and Kconfig. Your CI workflow already auto-injects
  `-DZMK_EXTRA_MODULES=<repo-root>` on every build; this file is what
  makes that auto-injected flag actually resolve to something valid,
  so the module's Kconfig option and source file get built in without
  needing any extra build.yaml flags.
- `modules/usb_display_wake/` — the custom module (CMakeLists.txt,
  Kconfig, src/usb_display_wake.c) that keeps the display on while USB
  power is present (charger, power bank, or data), regardless of the
  normal idle timeout.
- `boards/shields/.gitkeep` — placeholder, unchanged.

## Custom status screen (zmk-nice-oled)
Switched from the built-in status screen to
[zmk-nice-oled](https://github.com/mctechnology17/zmk-nice-oled).
Configured as:
- **Left (central)**: battery + BT profile/connection indicator (both
  shown by default, no toggle needed), WPM shown as a numeric readout
  plus a graph below it (speedometer/luna/bongo-cat off), layer
  indicator. HID lock and modifier indicators turned off since they
  weren't requested.
  - The layer indicator wasn't a position bug: ZMK's layer widget
    renders blank if a layer has no `display-name` set. Added
    `display-name = "Base"/"Lower"/"Adjust";` to the three layers in
    `lily58.keymap` to fix it.
  - Widened the vertical spacing between battery (Y=50), WPM number
    (Y=68), WPM graph (Y=88), and layer indicator (Y=118) since the
    first attempt at custom Y positions put the WPM number directly
    on top of the battery indicator. Still best-effort without seeing
    it on real hardware — send a photo if anything still overlaps and
    I'll adjust the exact pixel values.
- **Right (peripheral)**: battery + connection indicator via the
  animated "Smart Battery" widget, plus the animated gem (cat, the
  module's default animation, explicitly turned off in favor of gem).

The module's default widget coordinates are tuned for the exact
128×32 SSD1306 OLED this board uses (rendered rotated into a tall
strip), so no manual position tuning should be needed — but since this
can't be previewed without real hardware, some layout tweaks may still
be wanted once you see it running. All position options are
`CONFIG_NICE_OLED_WIDGET_*_CUSTOM_X` / `_Y` in `lily58.conf` if so.

## One manual cleanup step
If `boards/shields/lily58/boards/nice_nano_v2.overlay` still exists in
your repo from earlier troubleshooting, delete it — it's dead code,
silently shadowed by the built-in ZMK shield's own file of the same
name, and has no effect either way.

## Not included
Anything not touched in this conversation (e.g. your GitHub Actions
workflow file, LICENSE, top-level README) is left out on purpose, so
extracting this won't overwrite something we never looked at.
