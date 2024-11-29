/**
 * @file	BoolIndicator.h
 * @brief	Containing a class for a boolean indicator that is only showing a boolean variable value.
 */ 

#ifndef BOOLINDICATOR_H_
#define BOOLINDICATOR_H_

#include "../Core/UIElement.h"

/**
 * Available boolean indicator styles.
 */
typedef enum BoolIndicatorStyles
{
	BOOLINDICATOR_STYLE_LED,			/**< The boolean indicator is styled like a LED. */
	BOOLINDICATOR_STYLE_CLASSIC,		/**< The boolean indicator is styled with a rectangle with ON/OFF text. */
}BoolIndicatorStyles_t;

#define DEFAULT_BOOLINDICATOR_LED_DIAMETER		20									/**< Default width and height for the bool indicator with led style. */
#define DEFAULT_BOOLINDICATOR_LED_SPACING		3									/**< Default spacing between the led indicator outline and the inner circle. */

/**
 * Class for a boolean indicator that is only showing a boolean variable value.
 */
class BoolIndicator : public UIElement
{
	protected:
		bool* _valuePointer;			/**< Pointer to the boolean variable that is shown by this indicator. */
		bool _lastValueDraw;			/**< Last drawn bool value. Only if the _valuePointer value differs from this value, the indicator is redrawn. */
		BoolIndicatorStyles_t _style;	/**< Style used by the boolean indicator. */
		
	public:

		/**
		 * Constructor of the BooleanIndicator.
		 * @param valuePointer Pointer to the boolean variable that is shown by this indicator.
 		 * @param style Style used by the boolean indicator.
		 * @param locX X Location of the upper left corner of the BooleanIndicator 
		 * @param locY Y Location of the upper left corner of the BooleanIndicator
		 */
		BoolIndicator(bool* valuePointer, BoolIndicatorStyles_t style = BOOLINDICATOR_STYLE_LED, uint16_t locX = 0, uint16_t locY = 0);
		
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