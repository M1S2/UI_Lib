/**
 * @file	ContainerStack.h
 * @brief	Containing a class for a container that is showing all items at a time stacked one after another.
 */ 

#ifndef CONTAINERSTACK_H_
#define CONTAINERSTACK_H_

#include "ContainerPage.h"

/**
 * Available stack orientations
 */
typedef enum StackOrientation
{
	STACK_ORIENTATION_VERTICAL,			/**< Stack the elements vertical */
	STACK_ORIENTATION_HORIZONTAL,		/**< Stack the elements horizontal */
}StackOrientation_t;

#define DEFAULT_MARGIN_BETWEEN_ELEMENTS	5	/**< Default margin between elements */

/**
 * class for a container that is showing all items at a time stacked one after another.
 * @tparam maxItems Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory.
 */
template <uint8_t maxItems>
class ContainerStack : public ContainerPage<maxItems>
{
	private:
		StackOrientation_t _stackOrientation;					/**< Stack orientation */
		uint16_t _marginBetweenElements;						/**< Margin between the last element and the next one */

	public:

		/**
		 * Constructor of the ContainerStack.
		 * @param stackOrientation Stack orientation
		 * @param marginBetweenElements Margin between the last element and the next one
		 */
		ContainerStack(StackOrientation_t stackOrientation = STACK_ORIENTATION_VERTICAL, uint16_t marginBetweenElements = DEFAULT_MARGIN_BETWEEN_ELEMENTS);

		/**
		 * Constructor of the ContainerStack.
		 * @param locX X Location of the upper left corner of the ContainerStack 
		 * @param locY Y Location of the upper left corner of the ContainerStack
		 * @param width Drawing width of the ContainerStack
		 * @param height Drawing height of the ContainerStack
		 * @param stackOrientation Stack orientation
		 * @param marginBetweenElements Margin between the last element and the next one
		 */
		ContainerStack(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, StackOrientation_t stackOrientation = STACK_ORIENTATION_VERTICAL, uint16_t marginBetweenElements = DEFAULT_MARGIN_BETWEEN_ELEMENTS);

		/**
		 * Method used for drawing of the ContainerStack.
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 */
		virtual void Draw(Adafruit_GFX* gfx) override;

		/**
		 * Add the item to the elements of this container and adapt the item location accordingly.
		 * @param item Pointer to the UIElement that should be added to the container. 
		 * @return true, if added; otherwise false (if container is full) 
		 */
		bool AddItem(UIElement* item);
};

/********************************************************************************************************************************************/

typedef ContainerStack<MAX_CONTAINER_ITEMS> ContainerStackDefault;		/**< Type definition for a ContainerStack using the default settings. */

#endif /* CONTAINERSTACK_H_ */