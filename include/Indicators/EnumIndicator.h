/**
 * @file	EnumIndicator.h
 * @brief	Containing a class for an enum indicator that is only showing a enumeration variable value.
 */ 

#ifndef ENUMINDICATOR_H_
#define ENUMINDICATOR_H_

#include "../Core/UIElement.h"

/**
 * Class for an enum indicator that is only showing a enumeration variable value.
 * @tparam T Type of enumeration handled by this indicator
 */
template <class T>
class EnumIndicator : public UIElement
{
	protected:
		const char** _enumNames;	/**< Pointer to an array of char pointers containing the display names for all enumeration values. The number of elements in this array must be at least the number of enum values. */
		uint8_t _numEnumValues;		/**< Number of enumeration values. This number is set by the constructor of the indicator and must match the real number of enum values in T. */
		T* _valuePointer;			/**< Pointer to the enumeration variable that is shown by this indicator. */
		T _valueDraw;				/**< This variable is updated from the _valuePointer on each draw of the first page. */
	
	public:
	
		/**
		 * Constructor of the EnumIndicator.
		 * @param valuePointer Pointer to the enumeration variable that is shown by this indicator.
		 * @param enumNames Pointer to an array of char pointers containing the display names for all enumeration values. The number of elements in this array must be at least the number of enum values.
		 * @param numEnumValues Number of enumeration values. This number must match the real number of enum values in T.
		 */
		EnumIndicator(T* valuePointer, const char** enumNames, uint8_t numEnumValues);
		
		/**
		 * Constructor of the EnumIndicator.
		 * @param locX X Location of the upper left corner of the EnumIndicator 
		 * @param locY Y Location of the upper left corner of the EnumIndicator
		 * @param valuePointer Pointer to the enumeration variable that is shown by this indicator.
		 * @param enumNames Pointer to an array of char pointers containing the display names for all enumeration values. The number of elements in this array must be at least the number of enum values.
		 * @param numEnumValues Number of enumeration values. This number must match the real number of enum values in T.
		 */
		EnumIndicator(uint16_t locX, uint16_t locY, T* valuePointer, const char** enumNames, uint8_t numEnumValues);
		
		/**
		 * Method used for drawing of the EnumIndicator.
		 */
		virtual void Draw() override;

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

#endif /* ENUMINDICATOR_H_ */