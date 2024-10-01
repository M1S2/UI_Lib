/**
 * @file	ButtonControl.h
 * @brief	Containing a class for a button control that is showing a clickable button used to trigger some action.
 */ 

#ifndef BUTTONCONTROL_H_
#define BUTTONCONTROL_H_

#include "../Core/UIElement.h"

/**
 * Class for a button control that is showing a clickable button used to trigger some action.
 * @tparam StringLength Maximum string length that the button text can hold. This is used as buffer length for the internal _buttonText character buffer.
 */
template <int StringLength>
class ButtonControl : public UIElement
{
	private:
		char _buttonText[StringLength];						/**< Character buffer holding the string drawn by the button as text. */

		void* _controlContext;								/**< Context pointer that is returned with the _onClick function pointer */
		void(*_onClick)(void* controlContext);				/**< Function pointer for _onClick event. This function is called when the button is clicked. */

	public:
		
		/**
		 * Constructor of the ButtonControl.
		 * @param width Drawing width of the ButtonControl
		 * @param height Drawing height of the ButtonControl
		 * @param buttonText String that is draw to the screen as button text.
		 * @param controlContext Context pointer that is returned with the _onClick function pointer
		 * @param onClick Function pointer for _onClick event. This function is called when the button is clicked.
		 */
		ButtonControl(uint16_t width, uint16_t height, const char* buttonText, void* controlContext = NULL, void(*onClick)(void* controlContext) = NULL);

		/**
		 * Constructor of the ButtonControl.
		 * @param locX X Location of the upper left corner of the ButtonControl 
		 * @param locY Y Location of the upper left corner of the ButtonControl
		 * @param width Drawing width of the ButtonControl
		 * @param height Drawing height of the ButtonControl
		 * @param buttonText String that is draw to the screen as button text.
		 * @param controlContext Context pointer that is returned with the _onClick function pointer
		 * @param onClick Function pointer for _onClick event. This function is called when the button is clicked.
		 */
		ButtonControl(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, const char* buttonText, void* controlContext = NULL, void(*onClick)(void* controlContext) = NULL);

		/**
		 * Method used for drawing of the ButtonControl.
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 */
		virtual void Draw(Adafruit_GFX* gfx) override;

		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

/********************************************************************************************************************************************/

#define DEFAULT_BUTTON_STRING_LENGTH		25									/**< Default string length for the button text. */

typedef ButtonControl<DEFAULT_BUTTON_STRING_LENGTH> ButtonControlDefault;		/**< Type definition for a button using the default button text string length. */

#endif /* BUTTONCONTROL_H_ */