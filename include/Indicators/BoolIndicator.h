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
		bool _valueDraw;			/**< This variable is updated from the _valuePointer on each draw of the first page. */

	public:
	
		/**
		 * Constructor of the BooleanIndicator.
		 * @param locX X Location of the upper left corner of the BooleanIndicator 
		 * @param locY Y Location of the upper left corner of the BooleanIndicator
		 * @param valuePointer Pointer to the boolean variable that is shown by this indicator.
		 */
		BoolIndicator(uint16_t locX, uint16_t locY, bool* valuePointer);
		
		/**
		 * Method used for drawing of the BooleanIndicator.
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 * @param isFirstPage This parameter should be only set to true on the first iteration of the u8g_lib picture loop. It is used internally by the controls and indicators to update some variables on each redraw.
		 */
		virtual void Draw(Adafruit_GFX* gfx, bool isFirstPage) override;
};

#endif /* BOOLINDICATOR_H_ */