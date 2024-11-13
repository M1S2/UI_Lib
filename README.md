# UI_Lib
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/m1s2/library/UI_LIB.svg)](https://registry.platformio.org/libraries/m1s2/UI_LIB)
[![GitHub Release Version](https://img.shields.io/github/v/release/M1S2/UI_LIB)](https://github.com/M1S2/UI_LIB/releases/latest)
[![GitHub License](https://img.shields.io/github/license/M1S2/UI_LIB)](https://github.com/M1S2/UI_LIB/blob/master/LICENSE.md)

This is a user interface library for graphical LCDs. 
It offers many different controls and indicators that can be nested depending on the element types. 
This Readme gives an overview of the available components, the usage of the library and some implementation details.

The library uses a Visual Tree concept that is similar to the C# WPF UI organization. 
There is always one single tree root element that has nested children. 
There are UI elements that can have multiple children (e.g. pages) or elements that are "leaf" elements not supporting nested children (e.g. numeric controls).

![UI_Lib_Test boolean page](https://github.com/M1S2/UI_Lib/blob/master/Doc/Images/UI_Lib_Test_BooleanPage.jpg)

More example images can be found in the [Doc/Images folder](https://github.com/M1S2/UI_Lib/blob/master/Doc/Images).

![Visual Tree](https://github.com/M1S2/UI_Lib/blob/master/Doc/Images/VisualTree.jpg)


For more informations visit the detailed documentation: https://m1s2.github.io/UI_Lib

## Release creation process
Do the following steps to create a release:
1. Make sure that everything is committed to the master branch and builds without errors.
2. Update the Changelog and commit it to the master.
3. Checkout a new release branch (adapt the version accordingly): `git checkout -b release/v0.0.0`
4. Update the version in the library.json file: Replace the "[NO_RELEASE]" string by the version number (e.g. "0.0.0")
5. Create a new commit (adapt the version accordingly): `git add .` and `git commit -m "Created release v0.0.0"`
6. Tag the release commit (adapt the version accordingly): `git tag v0.0.0`
7. Open the PlatformIO Core CLI and run the following command: `pio pkg publish` (Check and acknowledge with "y" if asked)
8. Push everything: `git push` and `git push --tags`