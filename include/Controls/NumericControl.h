/**
 * @file	NumericControl.h
 * @brief	Containing a class for a numeric control that is showing a numeric variable value and offers the possibility to edit the value.
 */ 

#ifndef NUMERICCONTROL_H_
#define NUMERICCONTROL_H_

#include "../Indicators/NumericIndicator.h"
#include "VirtualKeys.h"

/**
 * Class for a numeric control that is showing a numeric variable value and offers the possibility to edit the value.
 * @tparam T Type of numeric variable handled by this control. This can be e.g. float or int.
 */
template <class T>
class NumericControl : public NumericIndicator<T>
{
	private:
		T _minValue;									/**< Minimum value that can be handled by this numeric control. */

		bool _lastDrawnEditMode;						/**< The EditMode that was last drawn. Used to detect changes in the edit mode. */
		int8_t _lastDrawnCurrentDigitPosition;			/**< The CurrentDigitPosition that was last drawn. Used to detect changes in the current digit position. */

		/**
		 * Limit the value to be between _minValue and _maxValue.
		 * @param value Value to coerce.
		 * @return Coerced value.
		 */
		T coerceValue(T value);
		
		/**
		 * Extract the digit at the requested position from the number.
		 * @param number Number from which to extract one digit.
		 * @param position position in the number from which to extract the digit. Position 0 is are 1er. Negative positions are fractionals. Positive positions are non-fractionals.
		 * @return Extracted digit between 0 and 9
		 * @see https://www.quora.com/How-can-you-mathematically-extract-a-single-digit-from-a-number
		 */
		uint8_t extractDigit(T number, int8_t position);
		
		void* _controlContext;							/**< Context pointer that is returned with the _onValueChanged function pointer */
		void(*_onValueChanged)(void* controlContext);	/**< Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed. */
		
		VirtualKeys* _virtualKeys;						/**< Pointer to a VirtualKeys UIElement. This can be used to edit the NumericControl via touch. */

	public:
		int8_t CurrentDigitPosition;					/**< Position of the currently controlled digit. Range from (-_numFractionalDigits) to (_numDigits - _numFractionalDigits - 1) */

		/**
		 * Constructor of the NumericControl.
		 * @param valuePointer Pointer to the numeric variable that is shown and manipulated by this control.
		 * @param baseUnit Base unit that is appended to the calculated prefix. E.g. "V" for voltage values. To use the "%" sign as unit, you have to use "%%" as string.
		 * @param minValue Minimum value that can be handled by this numeric control.
		 * @param maxValue Maximum value that can be shown by this numeric control. It is used to determine the number of non-fractional digits.
		 * @param numFractionalDigits Number of fractional digits that are shown by this control. E.g. 1.234 V has 3 fractional digits.
		 * @param controlContext Context pointer that is returned with the _onValueChanged function pointer
		 * @param onValueChanged Function pointer for _onValueChanged event. This function is called when the value of the valuePointer is changed.
		 * @param virtualKeys Pointer to a VirtualKeys UIElement. This can be used to edit the NumericControl via touch.
		 * @param locX X Location of the upper left corner of the NumericControl 
		 * @param locY Y Location of the upper left corner of the NumericControl
		 * @param maxStringBufferLength Length for the internally used _stringDrawBuffer. Make sure this is large enough to hold all character displayed by this element + 1 termination character '\0' (e.g. "1234.56mV" needs at least a length of 10; 9 characters + 1 termination character).
		 */
		NumericControl(T* valuePointer, const char* baseUnit, T minValue, T maxValue, int numFractionalDigits, void* controlContext = NULL, void(*onValueChanged)(void* controlContext) = NULL, VirtualKeys* virtualKeys = NULL, uint16_t locX = 0, uint16_t locY = 0, uint8_t maxStringBufferLength = DEFAULT_NUMERIC_INDICATOR_STRING_LENGTH);

		/**
		 * Method used for drawing of the NumericControl.
		 */
		virtual void Draw(bool redraw) override;
		
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK, KEYLEFT, KEYRIGHT, KEYUP, KEYDOWN, KEYKILO, KEYMILLI, KEYX1, KEYMINUS, KEY0 .. KEY9
s		 * @return true if the key was processed; false if not.
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
		 * Kilo key is used to change the prefix to kilo.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyKilo();
		
		/**
		 * Milli key is used to change the prefix to milli.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyMilli();
		
		/**
		 * Minus key is used to toggle the sign of the controlled value.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyMinus();
		
		/**
		 * X1 key is used to change the prefix to none.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyX1();
		
		/**
		 * Numeric keys (KEY0 .. KEY9) change the digit at the current cursor position.
		 * @return True if the key was handled, otherwise False.
		 */
		bool KeyNumeric(Keys_t key);
		
		/**
		 * Up key is used to increment the digit at the current cursor position.
		 * @return True if the key was handled, otherwise False.
		 */
		bool ValueUp();
		
		/**
		 * Down key is used to decrement the digit at the current cursor position.
		 * @return True if the key was handled, otherwise False.
		 */
		bool ValueDown();
		
		/**
		 * Left key is used to move the cursor position left.
		 * @return True if the key was handled, otherwise False.
		 */
		bool CursorLeft();

		/**
		 * Right key is used to move the cursor position right.
		 * @return True if the key was handled, otherwise False.
		 */
		bool CursorRight();
		
		/**
		 * Toggle the control between display and edit mode.
		 * Some keys are only supported in edit mode.
		 */
		void ToggleEditMode();

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

#endif /* NUMERICCONTROL_H_ */