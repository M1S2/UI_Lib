/**
 * @file	ContainerStack.h
 * @brief	Containing a class for a container that is showing all items at a time stacked one after another.
 */ 

#ifndef CONTAINERSTACK_H_
#define CONTAINERSTACK_H_

#include "Container.h"

/**
 * Available stack layouts
 */
typedef enum StackLayout
{
	STACK_LAYOUT_VERTICAL_LEFT,				/**< Stack the elements vertical, left aligned */
	STACK_LAYOUT_VERTICAL_CENTER,			/**< Stack the elements vertical, center aligned */
	STACK_LAYOUT_VERTICAL_RIGHT,			/**< Stack the elements vertical, right aligned */
	STACK_LAYOUT_HORIZONTAL_TOP,			/**< Stack the elements horizontal, top aligned */
	STACK_LAYOUT_HORIZONTAL_CENTER,			/**< Stack the elements horizontal, center aligned */
	STACK_LAYOUT_HORIZONTAL_BOTTOM			/**< Stack the elements horizontal, bottom aligned */
}StackLayout_t;

/**
 * class for a container that is showing all items at a time stacked one after another.
 * @tparam maxItems Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory.
 */
template <uint8_t maxItems>
class ContainerStack : public Container<maxItems>
{
	private:
		StackLayout_t _stackLayout;							/**< Stack layout */

	public:

		/**
		 * Constructor of the ContainerStack.
		 * @param stackLayout Stack layout
		 */
		ContainerStack(StackLayout_t stackLayout = STACK_LAYOUT_VERTICAL_LEFT);

		/**
		 * Constructor of the ContainerStack.
		 * @param locX X Location of the upper left corner of the ContainerStack 
		 * @param locY Y Location of the upper left corner of the ContainerStack
		 * @param width Drawing width of the ContainerStack
		 * @param height Drawing height of the ContainerStack
		 * @param stackLayout Stack layout
		 */
		ContainerStack(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, StackLayout_t stackLayout = STACK_LAYOUT_VERTICAL_LEFT);

		/**
		 * Method used for drawing of the ContainerStack.
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
		 * This method selects the first UIElement in the container that is of UI_CONTROL type.
		 */
		void InitItems();

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;

		/**
		 * Recalculate the UIElement layout (containers update the X- and Y-Location of all their items, all other items can do other layout update stuff)
		 */
		virtual void RecalculateLayout() override;
};

/********************************************************************************************************************************************/

typedef ContainerStack<MAX_CONTAINER_ITEMS> ContainerStackDefault;		/**< Type definition for a ContainerStack using the default settings. */

#endif /* CONTAINERSTACK_H_ */