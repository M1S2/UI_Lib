/**
 * @file	UI_Icons.h
 * @brief	Containing icons for the UI_Lib.
 */ 

#ifndef UI_ICONS_H_
#define UI_ICONS_H_

#include "../Core/UIElement.h"

#define icon_info_width 16									/**< Width of the info icon. */
#define icon_info_height 16									/**< Height of the info icon. */
extern const unsigned char icon_info_bits[] PROGMEM;	    /**< Data array of the info icon. */

#define icon_warning_width 16								/**< Width of the warning icon. */
#define icon_warning_height 16								/**< Height of the warning icon. */
extern const unsigned char icon_warning_bits[] PROGMEM;	    /**< Data array of the warning icon. */

#define icon_error_width 16									/**< Width of the error icon. */
#define icon_error_height 16								/**< Height of the error icon. */
extern const unsigned char icon_error_bits[] PROGMEM;	    /**< Data array of the error icon. */

#endif /* UI_ICONS_H_ */