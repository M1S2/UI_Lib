/**
 * @file	ContainerList.h
 * @brief	Containing a class for a container that is showing one item at a time and supports scrolling.
 */ 

#ifndef CONTAINERLIST_H_
#define CONTAINERLIST_H_

#include "Container.h"

#define SCROLLBAR_WIDTH			6		/**< Width in pixel of the scroll bar on the right side of the container */
#define SCROLLBAR_MARGIN		2		/**< Margin in pixel between the scroll bar frame and the scroll bar slider */

/**
 * Class for a container that is showing one item at a time and supports scrolling.
 */
class ContainerList : public Container
{
	private:
		int _lastDrawnItemIndex;		/**< Index of the last item */

	public:
		/**
		 * Constructor of the ContainerList.
		 */
		ContainerList();

		/**
		 * Constructor of the ContainerList.
		 * @param locX X Location of the upper left corner of the ContainerList 
		 * @param locY Y Location of the upper left corner of the ContainerList
		 * @param width Drawing width of the ContainerList
		 * @param height Drawing height of the ContainerList
		 */
		ContainerList(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height);
		
		/**
		 * Method used for drawing of the ContainerList.
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 */
		virtual void Draw(Adafruit_GFX* gfx) override;
			
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
};

#endif /* CONTAINERLIST_H_ */