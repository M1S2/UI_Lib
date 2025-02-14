# UI_Lib Changelog

## [v2.0.3]

- Added requirements for dynamic memory allocation to documentation (#7), decreased size for virtual key buttons
- Changed driver for XPT2046 touch controller in examples (to reduce the number of SPI transfers)

## [v2.0.2]

- Fixed small TouchInput bug (some controls were not selected on touch)
- Using touch IRQ pin in touch_demo example

## [v2.0.1]

- Small bugfix for NumericIndicator (make locX and locY optional)
- Corrected some images, updated XPT2046_Touchscreen_TT to v1.8.1 (used by examples)

## [v2.0.0]

- Added touch support for all UIElements
- Added VirtualKeys Control
- Changed focus frame to edge markers
- Added focus frame Color
- Changed button frames to full rectangle
- Improved doxygen docu and Readme
- Added more styles for boolean indicators and controls
- Changed API for most UIElements to use the locX and locY at the end of the constructor
- Renamed TabControl to ContainerTabs

## [v1.0.0]

- Initial commit

## [v0.8.0]

- Preliminary test version
