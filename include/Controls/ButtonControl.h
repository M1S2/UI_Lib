/**
 * @file	ButtonControl.h
 * @brief	Containing a class for a button control that is showing a clickable button used to trigger some action.
 */ 

#ifndef BUTTONCONTROL_H_
#define BUTTONCONTROL_H_

#include "../Core/UIElement.h"

#define DEFAULT_BUTTON_STRING_LENGTH		25									/**< Default string length for the button text. */

/**
 * Class for a button control that is showing a clickable button used to trigger some action.
 */
class ButtonControl : public UIElement
{
	private:
		char* _buttonText;									/**< Character buffer holding the string drawn by the button as text. */

	public:
		void* ControlContext;								/**< Context pointer that is returned with the _onClick function pointer */
		void(*OnClick)(void* controlContext);				/**< Function pointer for _onClick event. This function is called when the button is clicked. */
		
		/**
		 * Constructor of the ButtonControl.
		 * @param buttonText String that is draw to the screen as button text.
		 * @param controlContext Context pointer that is returned with the _onClick function pointer
		 * @param onClick Function pointer for _onClick event. This function is called when the button is clicked.
		 * @param locX X Location of the upper left corner of the ButtonControl 
		 * @param locY Y Location of the upper left corner of the ButtonControl
		 * @param maxStringLength Maximum string length that the button text can hold. This is used as buffer length for the internal _buttonText character buffer.
		 */
		ButtonControl(const char* buttonText, void* controlContext = NULL, void(*onClick)(void* controlContext) = NULL, uint16_t locX = 0, uint16_t locY = 0, int maxStringLength = DEFAULT_BUTTON_STRING_LENGTH);

		/**
		 * Destructor of the ButtonControl
		 */
		~ButtonControl()
		{
			delete _buttonText;
			_buttonText = NULL;
		}

		/**
		 * Method used for drawing of the ButtonControl.
		 */
		virtual void Draw(bool redraw) override;

		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;

		/**
		 * Process a touch input at the given point (x, y)
		 * @param x X-Coordinate of the touched point
		 * @param y Y-Coordinate of the touched point
		 * @param touchType Type of the touch
		 * @return true if the touch was processed; false if not.
		 */
		virtual bool TouchInput(uint16_t x, uint16_t y, TouchTypes touchType) override;
		
		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

#endif /* BUTTONCONTROL_H_ */