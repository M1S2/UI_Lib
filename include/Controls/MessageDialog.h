/**
 * @file	MessageDialog.h
 * @brief	Containing a class for a message dialog with optional Ok and Cancel buttons.
 */ 

#ifndef MESSAGEDIALOG_H_
#define MESSAGEDIALOG_H_

#include "../Core/UIElement.h"
#include "../Containers/ContainerPage.h"
#include "../Indicators/Label.h"
#include "../Indicators/Icon.h"
#include "../Controls/ButtonControl.h"
#include "../Core/UI_Icons.h"

#define DEFAULT_MAX_MSG_STRING_LENGTH		55			/**< Default string length for the message dialog text. */
#define DEFAULT_MSG_BUTTON_HEIGHT			20			/**< Default height for the message box buttons */

#define MSG_INFO_COLOR		RGB565(0x00, 0x66, 0xCC)
#define MSG_WARNING_COLOR	RGB565(0xFF, 0xFF, 0x00)
#define MSG_ERR_COLOR		RGB565(0xFF, 0x00, 0x00)

/**
 * Available message dialog severity types.
 */
typedef enum MessageSeverity
{
	MSG_INFO,				/**< Message is an information. */
	MSG_WARNING,			/**< Message is a warning. */
	MSG_ERROR				/**< Message is an error. */
}MessageSeverity_t;

/**
 * Available message dialog buttons.
 */
typedef enum MessageButtons
{
	MSG_BTN_NONE,			/**< Dialog without any buttons. */
	MSG_BTN_OK,				/**< Dialog with one OK button. */
	MSG_BTN_OK_CANCEL		/**< Dialog with an OK and CANCEL button. */
}MessageButtons_t;

/**
 * Class for a message dialog with optional Ok and Cancel buttons.
 */
class MessageDialog : public UIElement
{
	private:
		uint16_t _maxMsgLength;								/**< Maximum length for the internally used string buffer of the Label of the MessageDialog. This is the maximum length that the message can be. */
		ContainerPage _page;								/**< Container page that is internally used to group and handle all elements of the message dialog. */
		Icon _severityIcon;									/**< Icon UIElement used to display the message severity. */
		MessageSeverity_t _severity;						/**< Message severity. */
		Label _message;										/**< Label UIElement used to display the dialog message. */
		ButtonControl _buttonOk;							/**< Optional Ok button. */
		ButtonControl _buttonCancel;						/**< Optional Cancel button. */

	public:
	
		/**
		 * Constructor of the MessageDialog.
		 * @param locX X Location of the upper left corner of the MessageDialog 
		 * @param locY Y Location of the upper left corner of the MessageDialog
		 * @param width Drawing width of the MessageDialog
		 * @param height Drawing height of the MessageDialog
		 * @param message Message string that is shown by this message dialog
		 * @param severity Message severity
		 * @param buttons Optional dialog buttons (None, Ok, Ok & Cancel)
		 * @param controlContext Context pointer that is returned with the onOkClick and onCancelClick function pointers
		 * @param onOkClick Function pointer for onOkClick event. This function is called when the Ok button is clicked.
		 * @param onCancelClick Function pointer for onCancelClick event. This function is called when the Cancel button is clicked.
		 * @param maxMsgLength Maximum length for the internally used string buffer of the Label of the MessageDialog. This is the maximum length that the message can be.
		 */
		MessageDialog(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, const char* message, MessageSeverity_t severity, MessageButtons_t buttons = MSG_BTN_OK, void* controlContext = NULL, void(*onOkClick)(void* controlContext) = NULL, void(*onCancelClick)(void* controlContext) = NULL, uint16_t maxMsgLength = DEFAULT_MAX_MSG_STRING_LENGTH);
		
		/**
		 * Method used for drawing of the MessageDialog.
		 */
		virtual void Draw(bool redraw) override;

		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK, KEYUP, KEYDOWN
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

#endif /* MESSAGEDIALOG_H_ */