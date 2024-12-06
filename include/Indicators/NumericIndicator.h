/**
 * @file	NumericIndicator.h
 * @brief	Containing a class for a numeric indicator that is only showing a numeric variable value.
 */ 

#ifndef NUMERICINDICATOR_H_
#define NUMERICINDICATOR_H_

#include "../Core/UIElement.h"

#define DEFAULT_NUMERIC_INDICATOR_STRING_LENGTH		15						/**< Default max string length for a numeric indicator. */

/**
 * Class for a numeric indicator that is only showing a numeric variable value.
 * @tparam T Type of numeric variable handled by this indicator. This can be e.g. float or int.
 */
template <class T>
class NumericIndicator : public UIElement
{
	private:
		/** Calculate the _displayValue and the _unitPrefix from the given value. */
		void calculateDisplayValue(float value);

		/**
		 * Calculate the number of non-fractional digits of the given number.
		 * @see https://stackoverflow.com/questions/1489830/efficient-way-to-determine-number-of-digits-in-an-integer
		 */
		uint8_t numNonFractionalDigits(T number);
		
		const char* _unitPrefix;						/**< Current display prefix character ("m", "k", "M") */
		char* _stringDrawBuffer;						/**< Buffer holding the string that is drawn to the screen. This is only recalculated on the firstPage and if the value has changed. `char stringBufferLen = _numDigits + 1 + strlen(_unitPrefix) + strlen(_baseUnit) + 1;` */
		
	protected:
		const char* _baseUnit;							/**< Base unit that is appended to the calculated prefix. E.g. "V" for voltage values. To use the "%" sign as unit, you have to use "%%" as string. */
		T* _valuePointer;								/**< Pointer to the numeric variable that is shown by this indicator. */
		T _lastValueDraw;								/**< Last drawn numeric value. Only if the _valueDraw differs from this value, the _stringDrawBuffer is recalculated and the indicator redrawn. */
		T _maxValue;									/**< Maximum value that can be shown by this numeric indicator. It is used to determine the number of non-fractional digits. */
		uint8_t _numFractionalDigits;					/**< Number of fractional digits that are shown by this indicator. E.g. 1.234 V has 3 fractional digits. */
		uint8_t _numDigits;								/**< Number of digits calculated from the maxValue (_numFractionalDigits + numNonFractionalDigits). */

		float _displayValue;							/**< Value that is displayed by Draw(). E.g. 999.760 if control value is 999760 Hz. */
		int8_t _unitPrefixPower;						/**< Current display prefix power (m = -3, k = 3, M = 6) */

	public:
	
		/**
		 * Constructor of the NumericIndicator.
		 * @param valuePointer Pointer to the numeric variable that is shown by this indicator.
		 * @param baseUnit Base unit that is appended to the calculated prefix. E.g. "V" for voltage values. To use the "%" sign as unit, you have to use "%%" as string.
		 * @param maxValue Maximum value that can be shown by this numeric indicator. It is used to determine the number of non-fractional digits.
		 * @param numFractionalDigits Number of fractional digits that are shown by this indicator. E.g. 1.234 V has 3 fractional digits.
 		 * @param locX X Location of the upper left corner of the NumericIndicator 
		 * @param locY Y Location of the upper left corner of the NumericIndicator
		 * @param maxStringBufferLength Length for the internally used _stringDrawBuffer. Make sure this is large enough to hold all character displayed by this element + 1 termination character '\0' (e.g. "1234.56mV" needs at least a length of 10; 9 characters + 1 termination character).
		 */
		NumericIndicator(T* valuePointer, const char* baseUnit, T maxValue, unsigned char numFractionalDigits, uint16_t locX = 0, uint16_t locY = 0, uint8_t maxStringBufferLength = DEFAULT_NUMERIC_INDICATOR_STRING_LENGTH);
		
		/**
		 * Destructor of the NumericIndicator
		 */
		~NumericIndicator()
		{
			delete _stringDrawBuffer;
			_stringDrawBuffer = NULL;
		}

		/**
		 * Method used for drawing of the NumericIndicator.
		 */
		virtual void Draw(bool redraw) override;

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

#endif /* NUMERICINDICATOR_H_ */