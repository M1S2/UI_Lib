/**
 * @file	BoolControl.h
 * @brief	Containing a class for a boolean control that is showing a boolean variable value and offers the possibility to toggle the value.
 */ 

#ifndef BOOLCONTROL_H_
#define BOOLCONTROL_H_

#include "../Indicators/BoolIndicator.h"

/**
 * Available boolean control styles.
 */
typedef enum BoolControlStyles
{
	BOOLCONTROL_STYLE_TOGGLESWITCH,		/**< The boolean control is styled like a toggle switch. */
	BOOLCONTROL_STYLE_CHECKBOX,			/**< The boolean control is styled like a check box. */
	BOOLCONTROL_STYLE_CLASSIC,			/**< The boolean control is styled with a rectangle with ON/OFF text. */
}BoolControlStyles_t;

#define DEFAULT_BOOLCONTROL_TOGGLESWITCH_WIDTH			40		/**< Default width for the toggle switch. */
#define DEFAULT_BOOLCONTROL_TOGGLESWITCH_HEIGHT			20		/**< Default height for the toggle switch. */
#define DEFAULT_BOOLCONTROL_TOGGLESWITCH_SLIDER_SPACING	3		/**< Default spacing between the toggle switch outline and the slider circle. */

#define DEFAULT_BOOLCONTROL_CHECKBOX_SIZE				20		/**< Default width and height for the checkbox. */
#define DEFAULT_BOOLCONTROL_CHECKBOX_CHECK_SPACING		3		/**< Default spacing between the checkbox outline and the checkbox mark. */

/**
 * Class for a boolean control that is showing a boolean variable value and offers the possibility to toggle the value.
 */
class BoolControl : public UIElement
{	
	protected:
		bool* _valuePointer;							/**< Pointer to the boolean variable that is shown by this control. */
		bool _lastValueDraw;							/**< Last drawn bool value. Only if the _valuePointer value differs from this value, the control is redrawn. */
		BoolControlStyles_t _style;						/**< Style used by the boolean control. */
		
	public:
		void* ControlContext;							/**< Context pointer that is returned with the OnValueChanged function pointer */
		void(*OnValueChanged)(void* controlContext);	/**< Function pointer for OnValueChanged event. This function is called when the value of the valuePointer is changed. */
		
		/**
		 * Constructor of the BoolControl.
		 * @param valuePointer Pointer to the boolean variable that is shown and manipulated by this control.
		 * @param controlContext Context pointer that is returned with the OnValueChanged function pointer
		 * @param onValueChanged Function pointer for OnValueChanged event. This function is called when the value of the valuePointer is changed.
		 * @param style Style used by the boolean control.
		 * @param locX X Location of the upper left corner of the BoolControl 
		 * @param locY Y Location of the upper left corner of the BoolControl
		 */
		BoolControl(bool* valuePointer, void* controlContext = NULL, void(*onValueChanged)(void* controlContext) = NULL, BoolControlStyles_t style = BOOLCONTROL_STYLE_TOGGLESWITCH, uint16_t locX = 0, uint16_t locY = 0);
		
		/**
		 * Method used for drawing of the BoolControl.
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
		 * Toggle the value of the boolean variable referenced by the valuePointer.
		 */
		void ToggleValue();
		
		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

#endif /* BOOLCONTROL_H_ */