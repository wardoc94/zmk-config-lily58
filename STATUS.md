# zmk-config-lily58 — current state

Extract this folder's contents directly into the root of your
zmk-config-lily58 repo (merge with what's already there).

## Important: delete the old shared conf file
`config/lily58.conf` is now **replaced** by `config/lily58_left.conf`
and `config/lily58_right.conf`. Delete `config/lily58.conf` from your
repo if it's still present — a shared `<shield>.conf` always takes
priority over `_left`/`_right` variants, so leaving it in place would
silently ignore these two files.

## What changed this round
Reverted the "move BT dots to the top" experiment — it made the dots
disappear entirely and, before the left/right file split existed,
also pushed the right half's battery text into its gem animation via
a shared Kconfig symbol. Back to:
- **BT profile dots**: module default position (bottom of screen).
- **Profile number**: visible again (no longer hidden off-canvas).
- **Output icon / battery**: module default positions.
- **Layer indicator**: `Y=62`.
- **WPM number**: `X=16, Y=80` (centered attempt, not fully confirmed
  accurate — see note below).
- **WPM graph**: `Y=86`.
- **Right half gem**: reverted to module default position too, since
  the collision was most likely the left-side shared-symbol bug
  leaking across before the split existed, not a real default
  collision. Worth confirming with a photo.

## Why the left/right split still stays
Even though we're reverting the position experiment, the underlying
bug it exposed is real: one shared `lily58.conf` meant left-half
position changes could silently shift right-half widgets via shared
Kconfig symbols (e.g. `CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y`).
Keeping `lily58_left.conf` / `lily58_right.conf` separate prevents
that regardless of what positioning changes come next.

## What's included
- `config/lily58_left.conf` — RGB underglow, `CONFIG_USB_DISPLAY_WAKE`,
  and the central-side zmk-nice-oled layout described above.
- `config/lily58_right.conf` — same shared base settings, plus the
  peripheral-side layout (animated gem, Smart Battery), independent
  of the left half.
- `config/lily58.keymap` — original keymap plus the SPI3/WS2812
  devicetree block (pinctrl, &spi3, led_strip, chosen zmk,underglow)
  merged in directly, and `display-name` added to all three layers
  (needed for the layer widget to render at all — this is unrelated
  to the dots revert and still needed).
- `config/west.yml` — pulls in mctechnology17/zmk-nice-oled (`main`).
- `build.yaml` — `lily58_left nice_oled`, `lily58_right nice_oled`,
  `settings_reset`.
- `zephyr/module.yml` (repo root) — makes the CI workflow's
  auto-injected `-DZMK_EXTRA_MODULES=<repo-root>` resolve correctly,
  building in `modules/usb_display_wake`.
- `modules/usb_display_wake/` — custom module keeping the display on
  while USB power is present, regardless of the idle timeout.
- `boards/shields/.gitkeep` — placeholder, unchanged.

Both halves also have `CONFIG_LV_Z_MEM_POOL_SIZE=16384` (up from the
module's default 8192) — kept from the earlier fix for corrupted
glyph rendering on live-updating text (matches upstream ZMK issue
zmkfirmware/zmk #3219).

None of this is verified beyond the photos sent so far — please send
another one after this build to confirm the dots are back and nothing
else shifted.

## One manual cleanup step
If `boards/shields/lily58/boards/nice_nano_v2.overlay` still exists in
your repo from earlier troubleshooting, delete it — it's dead code,
silently shadowed by the built-in ZMK shield's own file of the same
name, and has no effect either way.

## Not included
Anything not touched in this conversation (e.g. your GitHub Actions
workflow file, LICENSE, top-level README) is left out on purpose, so
extracting this won't overwrite something we never looked at.
