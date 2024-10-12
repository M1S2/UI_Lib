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
			/*if(type != UI_CONTAINER)
			{
				Width = DEFAULT_UI_ELEMENT_WIDTH;
				Height = DEFAULT_UI_ELEMENT_HEIGHT;
			}*/
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
			/*if(type != UI_CONTAINER)
			{
				Width = DEFAULT_UI_ELEMENT_WIDTH;
				Height = DEFAULT_UI_ELEMENT_HEIGHT;
			}*/
			Visible = true;
			Type = type;
			ActiveChild = NULL;
		}

		/**
		 * Virtual method used for drawing of the UIElement.
		 * This method must be overwritten in all derived classes.
		 * Use the UiManager.Gfx object for drawing.
		 */
		virtual void Draw() = 0;
		
		/**
		 * Process the given key.
		 * This method can be overwritten in all derived classes. If it's not overwritten, no keys are supported by the UIElement.
		 * If this element doesn't support the key type, the key is forwarded to the parent.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed.
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) { return false; }

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() = 0;

		/**
		 * Recalculate the UIElement layout (containers update the X- and Y-Location of all their items, all other items can do other layout update stuff)
		 */
		virtual void RecalculateLayout() { }
};

#endif /* UIELEMENT_H_ */