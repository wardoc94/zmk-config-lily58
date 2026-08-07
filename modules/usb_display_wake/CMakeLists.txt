if(CONFIG_USB_DISPLAY_WAKE)
  zephyr_library()
  zephyr_library_sources(src/usb_display_wake.c)
endif()
