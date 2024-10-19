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
		uint16_t _color;					/**< Color that is used to draw the Icon. The color is only used to draw this Icon. All other UIElements are using the default color again. */
		bool _wasColorSet;					/**< true, if the color was set in the constructor or during the first draw call; false, if the color wasn't set in the constructor and before the first draw call */
		uint16_t _iconWidth;				/**< Width of the icon. */
		uint16_t _iconHeight;				/**< Height of the icon. */
		
	public:

		/**
		 * Constructor of the Icon.
		 * @param iconWidth Drawing width of the Icon
 		 * @param iconHeight Drawing height of the Icon
		 * @param iconBits Pointer to the array containing the icon data. This must be stored in Progmem. Use the following declaration: `unsigned char icon_<iconName>_bits[] PROGMEM = { ... }`
		 */
		Icon(uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits);

		/**
		 * Constructor of the Icon.
		 * @param iconWidth Drawing width of the Icon
 		 * @param iconHeight Drawing height of the Icon
		 * @param iconBits Pointer to the array containing the icon data. This must be stored in Progmem. Use the following declaration: `unsigned char icon_<iconName>_bits[] PROGMEM = { ... }`
		 * @param color Color that is used to draw the Icon. The color is only used to draw this Icon. All other UIElements are using the default color again.
		 */
		Icon(uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits, uint16_t color);

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
		 * Constructor of the Icon.
		 * @param locX X Location of the upper left corner of the Icon 
		 * @param locY Y Location of the upper left corner of the Icon
		 * @param iconWidth Drawing width of the Icon
 		 * @param iconHeight Drawing height of the Icon
		 * @param iconBits Pointer to the array containing the icon data. This must be stored in Progmem. Use the following declaration: `unsigned char icon_<iconName>_bits[] PROGMEM = { ... }`
		 * @param color Color that is used to draw the Icon. The color is only used to draw this Icon. All other UIElements are using the default color again.
		 */
		Icon(uint16_t locX, uint16_t locY, uint16_t iconWidth, uint16_t iconHeight, const uint8_t* iconBits, uint16_t color);
		
		/**
		 * Method used for drawing of the Icon.
		 */
		virtual void Draw(bool redraw) override;

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

#endif /* ICON_H_ */