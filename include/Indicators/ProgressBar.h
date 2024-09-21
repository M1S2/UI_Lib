/**
 * @file	ProgressBar.h
 * @brief	Containing a class for a progress bar indicator that is showing a horizontal progress bar.
 */ 

#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include "../Core/UIElement.h"

/**
 * Available progress bar origin positions.
 */
typedef enum ProgressBarOrigin
{
	PROGRESSBAR_ORIGIN_LEFT,		/**< Origin is on the left. All space from the minValue to the actualValue is filled. */
	PROGRESSBAR_ORIGIN_ZERO,		/**< Origin is at the zero point. The space from zero to the actualValue is filled (only negative or positive area). */
	PROGRESSBAR_ORIGIN_RIGHT		/**< Origin is on the right. All space from the maxValue to the actualValue is filled. */
}ProgressBarOrigin_t;

/**
 * Class for a progress bar indicator that is showing a horizontal progress bar.
 * @tparam T Type of numeric variable used by this indicator for drawing. This can be e.g. float or int.
 */
template <class T>
class ProgressBar : public UIElement
{
	private:
		T* _valuePointer;				/**< Pointer to the numeric variable that is used to draw this indicator. */
		T _valueDraw;					/**< This variable is updated from the _valuePointer on each draw of the first page. */
		T _minValue;					/**< Minimum value that can be shown by the progress bar. It is used to calculate the filled area. */
		T _maxValue;					/**< Maximum value that can be shown by the progress bar. It is used to calculate the filled area. */
		ProgressBarOrigin_t _origin;	/**< Origin position of this progress bar. See the ProgressBarOrigin enumeration for more details on the different options. */
		T _tickIncrement;				/**< Distance between tick lines drawn above the progress bar. Set this to 0 (or negative) to disable ticks. */

		uint16_t _originXCoord;	/**< X coordinate of the origin calculated on construction. */
		
		/**
		 * Calculate the X coordinate of the value on the progress bar.
		 * This takes the _minValue, _maxValue and Width into account.
		 * @param value value for which the X coordinate should be calculated.
		 * @return Calculated X coordinate
		 */
		uint16_t xCoordinateFromValue(T value);

	public:
	
		/**
		 * Constructor of the ProgressBar.
		 * @param width Drawing width of the ProgressBar
		 * @param height Drawing height of the ProgressBar
		 * @param valuePointer Pointer to the numeric variable that is used by this indicator.
		 * @param minValue Minimum value that can be shown by the progress bar. It is used to calculate the filled area.
		 * @param maxValue Maximum value that can be shown by the progress bar. It is used to calculate the filled area.
		 * @param origin Origin position of this progress bar. See the ProgressBarOrigin enumeration for more details on the different options.
		 * @param tickIncrement Distance between tick lines drawn above the progress bar. Set this to 0 (or negative) to disable ticks.
		 */
		ProgressBar(uint16_t width, uint16_t height, T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement);
		
		/**
		 * Constructor of the ProgressBar.
		 * @param locX X Location of the upper left corner of the ProgressBar 
		 * @param locY Y Location of the upper left corner of the ProgressBar
		 * @param width Drawing width of the ProgressBar
		 * @param height Drawing height of the ProgressBar
		 * @param valuePointer Pointer to the numeric variable that is used by this indicator.
		 * @param minValue Minimum value that can be shown by the progress bar. It is used to calculate the filled area.
		 * @param maxValue Maximum value that can be shown by the progress bar. It is used to calculate the filled area.
		 * @param origin Origin position of this progress bar. See the ProgressBarOrigin enumeration for more details on the different options.
		 * @param tickIncrement Distance between tick lines drawn above the progress bar. Set this to 0 (or negative) to disable ticks.
		 */
		ProgressBar(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement);
		
		/**
		 * Method used for drawing of the ProgressBar.
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 * @param wasScreenCleared This parameter indicates that the complete screen was cleared. Draw everything again.
		 */
		virtual void Draw(Adafruit_GFX* gfx, bool wasScreenCleared) override;
};

#endif /* PROGRESSBAR_H_ */