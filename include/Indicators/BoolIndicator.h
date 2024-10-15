/**
 * @file	BoolIndicator.h
 * @brief	Containing a class for a boolean indicator that is only showing a boolean variable value.
 */ 

#ifndef BOOLINDICATOR_H_
#define BOOLINDICATOR_H_

#include "../Core/UIElement.h"

/**
 * Class for a boolean indicator that is only showing a boolean variable value.
 */
class BoolIndicator : public UIElement
{
	protected:
		bool* _valuePointer;		/**< Pointer to the boolean variable that is shown by this indicator. */
		bool _lastValueDraw;		/**< Last drawn bool value. Only if the _valueDraw differs from this value, the indicator is redrawn. */
		
	public:
	
		/**
		 * Constructor of the BooleanIndicator.
		 * @param valuePointer Pointer to the boolean variable that is shown by this indicator.
		 */
		BoolIndicator(bool* valuePointer);

		/**
		 * Constructor of the BooleanIndicator.
		 * @param locX X Location of the upper left corner of the BooleanIndicator 
		 * @param locY Y Location of the upper left corner of the BooleanIndicator
		 * @param valuePointer Pointer to the boolean variable that is shown by this indicator.
		 */
		BoolIndicator(uint16_t locX, uint16_t locY, bool* valuePointer);
		
		/**
		 * Method used for drawing of the BooleanIndicator.
		 */
		virtual void Draw(bool redraw) override;

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
};

#endif /* BOOLINDICATOR_H_ */