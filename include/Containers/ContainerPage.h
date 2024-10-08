/**
 * @file	ContainerPage.h
 * @brief	Containing a class for a container that is showing all items at a time.
 */ 

#ifndef CONTAINERPAGE_H_
#define CONTAINERPAGE_H_

#include "Container.h"

/**
 * class for a container that is showing all items at a time.
 * @tparam maxItems Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory.
 */
template <uint8_t maxItems>
class ContainerPage : public Container<maxItems>
{
	public:
		/**
		 * Constructor of the ContainerPage.
		 */
		ContainerPage();
		
		/**
		 * Constructor of the ContainerPage.
		 * @param locX X Location of the upper left corner of the ContainerPage 
		 * @param locY Y Location of the upper left corner of the ContainerPage
		 * @param width Drawing width of the ContainerPage
		 * @param height Drawing height of the ContainerPage
		 */
		ContainerPage(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height);

		/**
		 * Method used for drawing of the ContainerPage.
		 */
		virtual void Draw() override;
		
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYUP, KEYDOWN
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;
		
		/**
		 * This method selects the first UIElement in the container that is of UI_CONTROL type.
		 */
		void InitItems();

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;

		/**
		 * Recalculate the X- and Y-Location of all items in the container
		 */
		virtual void RecalculateItemLocations() override;
};

/********************************************************************************************************************************************/

typedef ContainerPage<MAX_CONTAINER_ITEMS> ContainerPageDefault;		/**< Type definition for a ContainerPage using the default settings. */

#endif /* CONTAINERPAGE_H_ */