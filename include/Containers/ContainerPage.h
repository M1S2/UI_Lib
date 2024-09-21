/**
 * @file	ContainerPage.h
 * @brief	Containing a class for a container that is showing all items at a time.
 */ 

#ifndef CONTAINERPAGE_H_
#define CONTAINERPAGE_H_

#include "Container.h"

/**
 * class for a container that is showing all items at a time.
 */
class ContainerPage : public Container
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
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 * @param wasScreenCleared This parameter indicates that the complete screen was cleared. Draw everything again.
		 */
		virtual void Draw(Adafruit_GFX* gfx, bool wasScreenCleared) override;
		
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
		 * Select the next item in the container.
		 * @return true if there was a next item (if the selected item wasn't the last one).
		 */
		bool NextItem();
		
		/**
		 * Select the previous item in the container.
		 * @return true if there was a previous item (if the selected item wasn't the first one).
		 */
		bool PreviousItem();
		
		/**
		 * Select the next item in the container that is of UI_CONTROL type.
		 * @return true if there was a next control item (if the selected item wasn't the last one).
		 */
		bool NextControlItem();
		
		/**
		 * Select the previous item in the container that is of UI_CONTROL type.
		 * @return true if there was a previous control item (if the selected item wasn't the first one).
		 */
		bool PreviousControlItem();
};

#endif /* CONTAINERPAGE_H_ */