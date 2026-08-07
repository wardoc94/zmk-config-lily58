# zmk-config-lily58 — current state

Extract this folder's contents directly into the root of your
zmk-config-lily58 repo (merge with what's already there).

## What's included
- `config/lily58.conf` — RGB underglow (red, 10% brightness, full
  saturation, SPI3/P0.06 WS2812), `settings_reset` support, and
  `CONFIG_USB_DISPLAY_WAKE=y` enabling the new display-wake module.
- `config/lily58.keymap` — original keymap plus the SPI3/WS2812
  devicetree block (pinctrl, &spi3, led_strip, chosen zmk,underglow)
  merged in directly, since the built-in ZMK lily58 shield shadows any
  same-named file placed elsewhere.
- `config/west.yml` — unchanged from your original.
- `build.yaml` — three build targets: `lily58_left`, `lily58_right`
  (both with `-DZMK_EXTRA_MODULES` pointing at the new module), and
  `settings_reset`.
- `modules/usb_display_wake/` — the custom module that keeps the
  display on while USB power is present (charger, power bank, or
  data), regardless of the normal idle timeout.
- `boards/shields/.gitkeep` — placeholder, unchanged.

## One manual cleanup step
If `boards/shields/lily58/boards/nice_nano_v2.overlay` still exists in
your repo from earlier troubleshooting, delete it — it's dead code,
silently shadowed by the built-in ZMK shield's own file of the same
name, and has no effect either way.

## Not included
Anything not touched in this conversation (e.g. your GitHub Actions
workflow file, LICENSE, top-level README) is left out on purpose, so
extracting this won't overwrite something we never looked at.
