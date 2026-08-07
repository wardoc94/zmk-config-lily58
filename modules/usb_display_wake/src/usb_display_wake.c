/*
 * Copyright (c) 2026 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/activity_state_changed.h>
#include <zmk/usb.h>

LOG_MODULE_REGISTER(usb_display_wake, CONFIG_ZMK_LOG_LEVEL);

#if !DT_HAS_CHOSEN(zephyr_display)
#error "usb_display_wake requires a zephyr,display chosen devicetree node"
#endif

static const struct device *const display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

/*
 * ZMK's own display code blanks the screen in response to
 * zmk_activity_state_changed events (idle/sleep). We can't stop that
 * from happening, so instead we let it happen and then immediately
 * un-blank the display again if USB power is present. Running this a
 * few milliseconds later (via delayed work) rather than synchronously
 * in the event callback guarantees we run after ZMK's own listener has
 * already blanked the screen, so we reliably win the race.
 */
static void usb_display_wake_work_handler(struct k_work *work) {
    if (!device_is_ready(display_dev)) {
        return;
    }

    if (zmk_usb_get_conn_state() != ZMK_USB_CONN_NONE) {
        display_blanking_off(display_dev);
    }
}

static K_WORK_DELAYABLE_DEFINE(usb_display_wake_work, usb_display_wake_work_handler);

static int usb_display_wake_listener(const zmk_event_t *eh) {
    k_work_schedule(&usb_display_wake_work, K_MSEC(50));
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(usb_display_wake, usb_display_wake_listener);
ZMK_SUBSCRIPTION(usb_display_wake, zmk_activity_state_changed);
ZMK_SUBSCRIPTION(usb_display_wake, zmk_usb_conn_state_changed);
