/**
 * @file	Label.h
 * @brief	Containing a class for an Label indicator that is drawing a string to the screen.
 */ 

#ifndef LABEL_H_
#define LABEL_H_

#include "../Core/UIElement.h"
#include "../Core/UI_Manager.h"

#define DEFAULT_LABEL_STRING_LENGTH		50							/**< Default string length for a label. */
#define LABEL_COLOR_NOTSET				RGB565(0x2B, 0x23, 0x10)	/**< Define one color that is used to indicate that the user doesn't set the color */

/**
 * Class for an Label indicator that is drawing a string to the screen.
 */
class Label : public UIElement
{
	private:
		uint16_t _maxStringLength;					/**< Maximum string length that the Label can hold. This is used as buffer length for the internal character buffer. */
		const GFXfont* _font;						/**< Font that is used to draw the Label. The font is only used to draw this Label. All other UIElements are using the default font again. */
		uint16_t _color;							/**< Text color that is used to draw the Label. The color is only used to draw this Label. All other UIElements are using the default color again. Use values < 0 to use the UiManager.ColorForeground */

	public:
		char* Text;									/**< Character buffer holding the string drawn by the Label. */
		bool WasTextChangedSinceLastDraw;			/**< Flag indicating if the Text was changed since the last draw. This is set to true by the SetText() method and back to false after drawing. */

		/**
		 * Constructor of the Label.
		 * @param text String that is draw to the screen by the label.
		 * @param color Text color that is used to draw the Label. The color is only used to draw this Label. All other UIElements are using the default color again.
		 * @param font Font that is used to draw the Label. The font is only used to draw this Label. All other UIElements are using the default font again.
		 * @param locX X Location of the upper left corner of the Label 
		 * @param locY Y Location of the upper left corner of the Label
		 * @param maxStringLength Maximum string length that the Label can hold. This is used as buffer length for the internal character buffer.
		 */
		Label(const char* text, uint16_t color = LABEL_COLOR_NOTSET, const GFXfont* font = NULL, uint16_t locX = 0, uint16_t locY = 0, uint16_t maxStringLength = DEFAULT_LABEL_STRING_LENGTH);

		/**
		 * Destructor of the Label
		 */
		~Label()
		{
			delete Text;
			Text = NULL;
		}

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

#endif /* LABEL_H_ */