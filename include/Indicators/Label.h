/**
 * @file	Label.h
 * @brief	Containing a class for an Label indicator that is drawing a string to the screen.
 */ 

#ifndef LABEL_H_
#define LABEL_H_

#include "../Core/UIElement.h"
#include "../Core/UI_Manager.h"

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
		bool _wasColorSet;							/**< true, if the color was set in the constructor or during the first draw call; false, if the color wasn't set in the constructor and before the first draw call */

	public:
		char Text[StringLength];					/**< Character buffer holding the string drawn by the Label. */

		/**
		 * Constructor of the Label.
		 * @param text String that is draw to the screen by the label.
		 */
		Label(const char* text);

		/**
		 * Constructor of the Label.
		 * @param locX X Location of the upper left corner of the Label 
		 * @param locY Y Location of the upper left corner of the Label
		 * @param text String that is draw to the screen by the label.
		 */
		Label(uint16_t locX, uint16_t locY, const char* text);
		
		/**
		 * Constructor of the Label.
		 * @param locX X Location of the upper left corner of the Label 
		 * @param locY Y Location of the upper left corner of the Label
		 * @param text String that is draw to the screen by the label.
		 * @param color Text color that is used to draw the Label. The color is only used to draw this Label. All other UIElements are using the default color again.
		 */
		Label(uint16_t locX, uint16_t locY, const char* text, uint16_t color);

		/**
		 * Constructor of the Label including a font parameter.
		 * @param text String that is draw to the screen by the label.
		 * @param font Font that is used to draw the Label. The font is only used to draw this Label. All other UIElements are using the default font again.
		 * @param color Text color that is used to draw the Label. The color is only used to draw this Label. All other UIElements are using the default color again.
		 */
		Label(const char* text, const GFXfont* font, uint16_t color);

		/**
		 * Constructor of the Label.
		 * @param text String that is draw to the screen by the label.
		 * @param color Text color that is used to draw the Label. The color is only used to draw this Label. All other UIElements are using the default color again.
		 */
		Label(const char* text, uint16_t color);

		/**
		 * Constructor of the Label including a font parameter.
		 * @param locX X Location of the upper left corner of the Label 
		 * @param locY Y Location of the upper left corner of the Label
		 * @param text String that is draw to the screen by the label.
		 * @param font Font that is used to draw the Label. The font is only used to draw this Label. All other UIElements are using the default font again.
		 * @param color Text color that is used to draw the Label. The color is only used to draw this Label. All other UIElements are using the default color again.
		 */
		Label(uint16_t locX, uint16_t locY, const char* text, const GFXfont* font, uint16_t color);
		
		/**
		 * Method used for drawing of the Label.
		 */
		virtual void Draw(bool redraw) override;
		
		/**
		 * Change the string that is drawn to the screen by this Label. 
		 * @param text New text string of the Label.
		 */
		void SetText(const char* text);

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

/********************************************************************************************************************************************/

#define DEFAULT_LABEL_STRING_LENGTH		50						/**< Default string length for a label. */

typedef Label<DEFAULT_LABEL_STRING_LENGTH> LabelDefault;		/**< Type definition for a label using the default string length. */

#endif /* LABEL_H_ */