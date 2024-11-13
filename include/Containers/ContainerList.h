/**
 * @file	ContainerList.h
 * @brief	Containing a class for a container that is showing one item at a time and supports scrolling.
 */ 

#ifndef CONTAINERLIST_H_
#define CONTAINERLIST_H_

#include "Container.h"

#define DEFAULT_CONTAINERLIST_SCROLLBAR_WIDTH		10		/**< Default width in pixel of the scroll bar on the right side of the container */
#define DEFAULT_CONTAINERLIST_SCROLLBAR_MARGIN		2		/**< Default margin in pixel between the scroll bar frame and the container list outline */

/**
 * Class for a container that is showing one item at a time and supports scrolling.
 */
class ContainerList : public Container
{
	private:
		uint8_t _lastDrawnItemIndex;		/**< Index of the last item */
		uint8_t _scrollBarWidth;			/**< Width in pixel of the scroll bar on the right side of the container */
		uint8_t _scrollBarMargin;			/**< Margin in pixel between the scroll bar frame and the container list outline */

	public:
		/**
		 * Constructor of the ContainerList.
		 * @param maxNumItems Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory.
		 * @param scrollBarWidth Width in pixel of the scroll bar on the right side of the container
 		 * @param scrollBarMargin Margin in pixel between the scroll bar frame and the container list outline
		 */
		ContainerList(uint8_t maxNumItems = DEFAULT_MAX_CONTAINER_ITEMS, uint8_t scrollBarWidth = DEFAULT_CONTAINERLIST_SCROLLBAR_WIDTH, uint8_t scrollBarMargin = DEFAULT_CONTAINERLIST_SCROLLBAR_MARGIN);

		/**
		 * Constructor of the ContainerList.
		 * @param locX X Location of the upper left corner of the ContainerList 
		 * @param locY Y Location of the upper left corner of the ContainerList
		 * @param width Drawing width of the ContainerList
		 * @param height Drawing height of the ContainerList
		 * @param maxNumItems Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory.
		 * @param scrollBarWidth Width in pixel of the scroll bar on the right side of the container
 		 * @param scrollBarMargin Margin in pixel between the scroll bar frame and the container list outline
		 */
		ContainerList(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, uint8_t maxNumItems = DEFAULT_MAX_CONTAINER_ITEMS, uint8_t scrollBarWidth = DEFAULT_CONTAINERLIST_SCROLLBAR_WIDTH, uint8_t scrollBarMargin = DEFAULT_CONTAINERLIST_SCROLLBAR_MARGIN);
		
		/**
		 * Method used for drawing of the ContainerList.
		 */
		virtual void Draw(bool redraw) override;
			
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYUP, KEYDOWN
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;

		/**
		 * Recalculate the UIElement layout (containers update the X- and Y-Location of all their items, all other items can do other layout update stuff)
		 */
		virtual void RecalculateLayout() override;
};

#endif /* CONTAINERLIST_H_ */