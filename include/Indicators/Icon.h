/**
 * @file	Icon.h
 * @brief	Containing a class for an icon indicator that is drawing an icon to the screen.
 */ 

#ifndef ICON_H_
#define ICON_H_

#include "../Core/UIElement.h"

/**
 * Class for an icon indicator that is drawing an icon to the screen.
 */
class Icon : public UIElement
{
	private:
		const uint8_t* _iconBits;			/**< Pointer to the array containing the icon data. This must be stored in Progmem. Use the following declaration: `unsigned char icon_<iconName>_bits[] PROGMEM = { ... }` */
	
	public:
	
		/**
		 * Constructor of the Icon.
		 * @param locX X Location of the upper left corner of the Icon 
		 * @param locY Y Location of the upper left corner of the Icon
		 * @param iconWidth Drawing width of the Icon
 		 * @param iconHeight Drawing height of the Icon
		 * @param iconBits Pointer to the array containing the icon data. This must be stored in Progmem. Use the following declaration: `unsigned char icon_<iconName>_bits[] PROGMEM = { ... }`
		 */
		Icon(uint16_t locX, uint16_t locY, uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits);
		
		/**
		 * Method used for drawing of the Icon.
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(Adafruit_GFX* gfx, bool isFirstPage) override;
};

#endif /* ICON_H_ */