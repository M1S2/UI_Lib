/**
 * @file	ContainerPage.h
 * @brief	Containing a class for a container that is showing all items at a time.
 */ 

#ifndef CONTAINERPAGE_H_
#define CONTAINERPAGE_H_

#include "Container.h"

/**
 * Struct used to store the relative coordinates of a item inside the page.
 */
struct PageItemConfig
{
	UIElement* item;		/**< Pointer to the item for which this configuration applies */
	uint16_t relativeX;		/**< Relative X coordinate inside the page where the item is placed */
	uint16_t relativeY;		/**< Relative Y coordinate inside the page where the item is placed */
};

/**
 * class for a container that is showing all items at a time.
 */
class ContainerPage : public Container
{
	private:
		PageItemConfig* _itemConfiguration;		/**< Configuration data for all items inside the page. */

	public:

		/**
		 * Constructor of the ContainerPage.
		 * @param maxNumItems Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory.
		 * @param locX X Location of the upper left corner of the ContainerPage 
		 * @param locY Y Location of the upper left corner of the ContainerPage
		 * @param width Drawing width of the ContainerPage
		 * @param height Drawing height of the ContainerPage
		  */
		ContainerPage(uint8_t maxNumItems = DEFAULT_MAX_CONTAINER_ITEMS, uint16_t locX = 0, uint16_t locY = 0, uint16_t width = 0, uint16_t height = 0);

		/**
		 * Method used for drawing of the ContainerPage.
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
		 * Process a touch input at the given point (x, y)
		 * @param x X-Coordinate of the touched point
		 * @param y Y-Coordinate of the touched point
		 * @param touchType Type of the touch
		 * @return true if the touch was processed; false if not.
		 */
		virtual bool TouchInput(uint16_t x, uint16_t y, TouchTypes touchType) override;

		/**
		 * This method selects the first UIElement in the container that is of UI_CONTROL type.
		 */
		void InitItems();

		/**
		 * Add the item to the elements of this container and adapt the save the current relative coordinates.
		 * @param item Pointer to the UIElement that should be added to the container. 
		 * @return true, if added; otherwise false (if container is full) 
		 */
		bool AddItem(UIElement* item);

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;

		/**
		 * Recalculate the UIElement layout (containers update the X- and Y-Location of all their items, all other items can do other layout update stuff)
		 */
		virtual void RecalculateLayout() override;
};

#endif /* CONTAINERPAGE_H_ */