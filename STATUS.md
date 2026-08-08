# zmk-config-lily58 — current state

Extract this folder's contents directly into the root of your
zmk-config-lily58 repo (merge with what's already there).

## Important: delete the old shared conf file
`config/lily58.conf` is now **replaced** by `config/lily58_left.conf`
and `config/lily58_right.conf`. Delete `config/lily58.conf` from your
repo — if it's still present, ZMK will use it instead of the new
per-side files (a shared `<shield>.conf` always takes priority over
`_left`/`_right` variants).

## Why the split
Left and right were sharing one `lily58.conf`, so widget-position
Kconfig options (like `CONFIG_NICE_OLED_WIDGET_BATTERY_CUSTOM_Y`) used
the same symbol on both halves. Repositioning things for the left
half's stacked layout accidentally shifted the right half's battery
text into its gem animation. Splitting into per-side files lets each
half be tuned independently.

## What's included
- `config/lily58_left.conf` — RGB underglow, `CONFIG_USB_DISPLAY_WAKE`,
  and the central-side zmk-nice-oled layout (BT profile dots, output
  icon, battery, layer, WPM number, WPM graph — see below for exact
  Y positions).
- `config/lily58_right.conf` — same shared base settings (RGB, USB
  wake, etc.), plus the peripheral-side layout (animated gem, Smart
  Battery), independent of the left half's positions.
- `config/lily58.keymap` — original keymap, plus the SPI3/WS2812
  devicetree block (pinctrl, &spi3, led_strip, chosen zmk,underglow)
  merged in directly since the built-in ZMK lily58 shield shadows any
  same-named file placed elsewhere, and `display-name` added to all
  three layers (needed for the layer widget to render at all).
- `config/west.yml` — pulls in mctechnology17/zmk-nice-oled (`main`).
- `build.yaml` — `lily58_left nice_oled`, `lily58_right nice_oled`,
  `settings_reset`.
- `zephyr/module.yml` (repo root) — makes the CI workflow's
  auto-injected `-DZMK_EXTRA_MODULES=<repo-root>` resolve correctly,
  building in `modules/usb_display_wake`.
- `modules/usb_display_wake/` — custom module keeping the display on
  while USB power is present, regardless of the idle timeout.
- `boards/shields/.gitkeep` — placeholder, unchanged.

## Custom status screen (zmk-nice-oled) — current layout

**Left (central)**, top to bottom:
- BT profile dots — `Y=14` (Y=2 rendered nothing, likely clipped by
  an unusable margin near the true top edge)
- Output icon (USB/BT type) — `Y=46`
- Battery — `Y=64`
- Layer indicator — `Y=76`
- WPM number — `Y=90`, `X=0` (left-aligned; two centering attempts,
  X=10 and X=16, both landed visibly off — reverted to left-aligned
  to match battery/layer rather than guess a third time)
- WPM graph — `Y=112` (was overlapping the number at a 6px gap;
  widened substantially)

The profile *number* next to the output icon is hidden at `Y=999`
(pushed off-canvas) since there's no documented Kconfig toggle to
disable it outright — a workaround, not a clean "off" switch.

**Right (peripheral)**: animated gem at `Y=60` (module default -18,
pushed down to clear the battery/output text above it), Smart Battery
animation, output/battery text left at module defaults.

**Both halves**: `CONFIG_LV_Z_MEM_POOL_SIZE=16384` (up from the
module's default 8192). Both the left WPM number and the right
battery percentage were rendering with corrupted glyphs (e.g. "120"
as "1Z0", "100%" as "1AA5") — matches a documented upstream ZMK issue
(zmkfirmware/zmk #3219) where too small an LVGL memory pool causes
exactly this once several widgets are active simultaneously.

None of this has been verified against real hardware beyond the two
photos so far — please send another one after this build.

## One manual cleanup step
If `boards/shields/lily58/boards/nice_nano_v2.overlay` still exists in
your repo from earlier troubleshooting, delete it — it's dead code,
silently shadowed by the built-in ZMK shield's own file of the same
name, and has no effect either way.

## Not included
Anything not touched in this conversation (e.g. your GitHub Actions
workflow file, LICENSE, top-level README) is left out on purpose, so
extracting this won't overwrite something we never looked at.
