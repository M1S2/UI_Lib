/**
 * @file	Container.h
 * @brief	Containing the base class for container elements.
 */ 

#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "../Core/UIElement.h"

#define MAX_CONTAINER_ITEMS		20		/**< Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory. */

/**
 * Base class for container elements.
 */
class Container : public UIElement
{
	protected:
		UIElement* _items[MAX_CONTAINER_ITEMS];		/**< Array holding pointers to all UIElement that are items of the container. */
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
		 * @param wasScreenCleared This parameter indicates that the complete screen was cleared. Draw everything again.
		 */
		virtual void Draw(Adafruit_GFX* gfx, bool wasScreenCleared) { }  //= 0;
			
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
		 * @return true, if added; otherwise false (if container is full) 
		 */
		bool AddItem(UIElement* item);
};

#endif /* CONTAINER_H_ */