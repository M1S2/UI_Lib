/**
 * @file	Container.h
 * @brief	Containing the base class for container elements.
 */ 

#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "../Core/UIElement.h"

/**
 * Base class for container elements.
 * @tparam maxItems Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory.
 */
template <uint8_t maxItems>
class Container : public UIElement
{
	protected:
		UIElement* _items[maxItems];				/**< Array holding pointers to all UIElement that are items of the container. */
		int _numItems;								/**< Number of items in the container (number of valid items in the _items array). */
		int _selectedItemIndex;						/**< Index of the selected container item. */

	public:
		/**
		 * Constructor of the Container.
		 */
		Container();
		
		/**
		 * Virtual method used for drawing of the container UIElement.
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 */
		virtual void Draw(Adafruit_GFX* gfx) { }  //= 0;
			
		/**
		 * Process the given key.
		 * This method can be overwritten in all derived classes. If it's not overwritten, no keys are supported by the container UIElement.
		 * If this element doesn't support the key type, the key is forwarded to the parent.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed.
		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) { return false; }  // = 0;
		
		/**
		 * Get the selected item.
		 * This is the UIElement in the _items array at the _selectedItemIndex.
		 * @return Pointer to the selected UIElement in the container. 
		 */
		UIElement* GetSelectedItem();
		
		/**
		 * Add a new element to the container and activate it as active child.
		 * @param item Pointer to the UIElement that should be added to the container.
		 * @param stretchWidthEnabled If true, the items Width is changed to fill the parent container (if the item is a UI_CONTAINER and the item Width is 0)
		 * @param strechHeightEnabled If true, the items Height is changed to fill the parent container (if the item is a UI_CONTAINER and the item Height is 0)
		 * @return true, if added; otherwise false (if container is full) 
		 */
		bool AddItem(UIElement* item, bool stretchWidthEnabled = true, bool strechHeightEnabled = true);

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

		/**
		 * Get a bounding box around all items in the Container
		 * @param x X coordinate of the upper left corner of the bounding box
		 * @param y Y coordinate of the upper left corner of the bounding box
		 * @param w Width of the bounding box
		 * @param h Height of the bounding box
		 */
		void GetItemsBoundingBox(uint16_t* x, uint16_t* y, uint16_t* w, uint16_t* h);
};

#define MAX_CONTAINER_ITEMS		20		/**< Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory. */


#endif /* CONTAINER_H_ */