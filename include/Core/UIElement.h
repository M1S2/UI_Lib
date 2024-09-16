/**
 * @file	UIElement.h
 * @brief	Containing the abstract base class for all user interface elements (controls, indicators, containers).
 */ 

#ifndef UIELEMENT_H_
#define UIELEMENT_H_

#include "UIElementType.h"
#include "Keys.h"
#include <stdint.h>
#include <Adafruit_GFX.h>

#include "Fonts/FreeSans12pt7b.h"

#define DEFAULT_UI_ELEMENT_WIDTH	70			/**< Default width for an UIElement */
#define DEFAULT_UI_ELEMENT_HEIGHT	20			/**< Default height for an UIElement */

// https://forum.arduino.cc/t/solved-rgb-in-rgb565-umwandeln-und-mit-0x-in-eine-variable-schreiben/1146991/2
#define RGB565(r, g, b) ((((r)& 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))

#define BACKGROUND_COLOR					RGB565(0, 0, 0)				/**< Background color (black) */
#define DEFAULT_UI_ELEMENT_COLOR			RGB565(0, 0xF7, 0)			/**< Default UIElement color (white) */	
#define DEFAULT_UI_ELEMENT_COLOR_CONTRAST	BACKGROUND_COLOR			/**< Default UIElement contrast color (black) */	

#define DEFAULT_FONT 						&FreeSans12pt7b
#define DEFAULT_FONT_OFFSET_Y_BASELINE		18

/**
 * Abstract base class for all user interface elements (controls, indicators, containers).
 */
class UIElement
{
	public:
		UIElementType Type;			/**< Element type (control, indicator, container) */
		uint16_t LocX;				/**< X Location of the upper left corner of the UIElement */
		uint16_t LocY;				/**< Y Location of the upper left corner of the UIElement */
		uint16_t Width;				/**< Drawing width of the UIElement */
		uint16_t Height;			/**< Drawing height of the UIElement */
		bool Visible;				/**< The UIElement is only drawn if the visibility is set to true */
		UIElement* Parent;			/**< Parent UIElement that this UIElement belongs to */
		UIElement* ActiveChild;		/**< Child element that is currently active (receiving all key inputs). This property is only set for elements that contain children (mostly containers). Simple controls or indicators should set this to NULL. */

		/**
		 * Constructor of the UIElement.
		 * @param type Element type (control, indicator, container)
		 */
		UIElement(UIElementType type)
		{
			Visible = true;
			Type = type;
			ActiveChild = NULL;
		}
	
		/**
		 * Constructor of the UIElement.
		 * @param locX X Location of the upper left corner of the UIElement
		 * @param locY Y Location of the upper left corner of the UIElement
		 * @param type Element type (control, indicator, container)
		 */
		UIElement(uint16_t locX, uint16_t locY, UIElementType type)
		{
			LocX = locX;
			LocY = locY;
			Width = DEFAULT_UI_ELEMENT_WIDTH;
			Height = DEFAULT_UI_ELEMENT_HEIGHT;
			Visible = true;
			Type = type;
			ActiveChild = NULL;
		}

		/**
		 * Virtual method used for drawing of the UIElement.
		 * This method must be overwritten in all derived classes.
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(Adafruit_GFX* gfx, bool isFirstPage) = 0;
		
		/**
		 * Process the given key.
		 * This method can be overwritten in all derived classes. If it's not overwritten, no keys are supported by the UIElement.
		 * If this element doesn't support the key type, the key is forwarded to the parent.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed.
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) { return false; }
};

#endif /* UIELEMENT_H_ */