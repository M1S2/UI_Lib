/**
 * @file	Label.h
 * @brief	Containing a class for an Label indicator that is drawing a string to the screen.
 */ 

#ifndef LABEL_H_
#define LABEL_H_

#include "../Core/UIElement.h"

#define DEFAULT_LABEL_STRING_LENGTH		50			/**< Default string length for a label. */

/**
 * Class for an Label indicator that is drawing a string to the screen.
 * @tparam StringLength Maximum string length that the Label can hold. This is used as buffer length for the internal character buffer.
 */
template <int StringLength>
class Label : public UIElement
{
	private:
		const GFXfont* _font;						/**< Font that is used to draw the Label. The font is only used to draw this Label. All other UIElements are using the default font again. */
		uint16_t _color;							/**< Text color that is used to draw the Label. The color is only used to draw this Label. All other UIElements are using the default color again. */

	public:
		char Text[StringLength];					/**< Character buffer holding the string drawn by the Label. */

		/**
		 * Constructor of the Label.
		 * @param locX X Location of the upper left corner of the Label 
		 * @param locY Y Location of the upper left corner of the Label
		 * @param text String that is draw to the screen by the label.
		 */
		Label(uint16_t locX, uint16_t locY, const char* text);
		
		/**
		 * Constructor of the Label including a font parameter.
		 * @param locX X Location of the upper left corner of the Label 
		 * @param locY Y Location of the upper left corner of the Label
		 * @param text String that is draw to the screen by the label.
		 * @param font Font that is used to draw the Label. The font is only used to draw this Label. All other UIElements are using the default font again.
		 * @param color Text color that is used to draw the Label. The color is only used to draw this Label. All other UIElements are using the default color again.
		 */
		Label(uint16_t locX, uint16_t locY, const char* text, const GFXfont* font, uint16_t color = DEFAULT_UI_ELEMENT_COLOR);
		
		/**
		 * Method used for drawing of the Label.
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(Adafruit_GFX* gfx, bool isFirstPage) override;
		
		/**
		 * Change the string that is drawn to the screen by this Label. 
		 * @param text New text string of the Label.
		 */
		void SetText(const char* text);
};

typedef Label<DEFAULT_LABEL_STRING_LENGTH> LabelDefault;		/**< Type definition for a label using the default string length. */

#endif /* LABEL_H_ */