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

#define PROGRESSBAR_LONG_TICK_LENGTH     	6		/**< Length of the tick lines used at all values dividable by 10  */
#define PROGRESSBAR_MIDDLE_TICK_LENGTH 		4		/**< Length of the tick lines used at all values dividable by 5  */
#define PROGRESSBAR_SHORT_TICK_LENGTH   	2		/**< Length of the tick lines used at all values dividable by 1  */

/**
 * Class for a progress bar indicator that is showing a horizontal progress bar.
 * @tparam T Type of numeric variable used by this indicator for drawing. This can be e.g. float or int.
 */
template <class T>
class ProgressBar : public UIElement
{
	private:
		T* _valuePointer;				/**< Pointer to the numeric variable that is used to draw this indicator. */
		T _lastValueDraw;				/**< This variable is updated from the _valuePointer when the value changed. */
		T _minValue;					/**< Minimum value that can be shown by the progress bar. It is used to calculate the filled area. */
		T _maxValue;					/**< Maximum value that can be shown by the progress bar. It is used to calculate the filled area. */
		ProgressBarOrigin_t _origin;	/**< Origin position of this progress bar. See the ProgressBarOrigin enumeration for more details on the different options. */
		T _tickIncrement;				/**< Distance between tick lines drawn above the progress bar. Set this to 0 (or negative) to disable ticks. */
		uint16_t _originXCoord;			/**< X coordinate of the origin calculated on layout reconstruction. */
		
		uint16_t _minValueTextWidth;	/**< Text width of the minimum value text */
		uint16_t _maxValueTextWidth;	/**< Text width of the maximum value text */

		/**
		 * Calculate the X coordinate of the value on the progress bar.
		 * This takes the _minValue, _maxValue and ProgressbarWidth into account.
		 * @param value value for which the X coordinate should be calculated.
		 * @return Calculated X coordinate. This is inside the progressbar region.
		 */
		uint16_t xCoordinateFromValue(T value);

	public:
		uint16_t ProgressbarWidth;		/**< Drawing width of the ProgressBar (without min and max texts and tick markers) */
		uint16_t ProgressbarHeight;		/**< Drawing height of the ProgressBar (without min and max texts and tick markers) */
		
		/**
		 * Constructor of the ProgressBar.
		 * @param valuePointer Pointer to the numeric variable that is used by this indicator.
		 * @param minValue Minimum value that can be shown by the progress bar. It is used to calculate the filled area.
		 * @param maxValue Maximum value that can be shown by the progress bar. It is used to calculate the filled area.
		 * @param origin Origin position of this progress bar. See the ProgressBarOrigin enumeration for more details on the different options.
		 * @param tickIncrement Distance between tick lines drawn above the progress bar. Set this to 0 (or negative) to disable ticks.
		 * @param progressbarWidth Drawing width of the ProgressBar (without min and max texts and tick markers)
		 * @param progressbarHeight Drawing height of the ProgressBar (without min and max texts and tick markers)
		 */
		ProgressBar(T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement, uint16_t progressbarWidth = 80, uint16_t progressbarHeight = 10);
		
		/**
		 * Constructor of the ProgressBar.
		 * @param locX X Location of the upper left corner of the ProgressBar 
		* @param locY Y Location of the upper left corner of the ProgressBar
		* @param valuePointer Pointer to the numeric variable that is used by this indicator.
		* @param minValue Minimum value that can be shown by the progress bar. It is used to calculate the filled area.
		* @param maxValue Maximum value that can be shown by the progress bar. It is used to calculate the filled area.
		* @param origin Origin position of this progress bar. See the ProgressBarOrigin enumeration for more details on the different options.
		* @param tickIncrement Distance between tick lines drawn above the progress bar. Set this to 0 (or negative) to disable ticks.
		* @param progressbarWidth Drawing width of the ProgressBar (without min and max texts and tick markers)
		* @param progressbarHeight Drawing height of the ProgressBar (without min and max texts and tick markers)
		*/
		ProgressBar(uint16_t locX, uint16_t locY, T* valuePointer, T minValue, T maxValue, ProgressBarOrigin_t origin, T tickIncrement, uint16_t progressbarWidth = 80, uint16_t progressbarHeight = 10);
		
		/**
		 * Method used for drawing of the ProgressBar.
		 */
		virtual void Draw() override;
		
		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;
		
		/**
		 * Recalculate the UIElement layout (containers update the X- and Y-Location of all their items, all other items can do other layout update stuff)
		 */
		virtual void RecalculateLayout() override;
};


#endif /* PROGRESSBAR_H_ */