/**
 * @file	VirtualKeys.h
 * @brief	Containing a class for a virtual keyboard.
 */ 

#ifndef VIRTUALKEYS_H_
#define VIRTUALKEYS_H_

#include "../Core/UIElement.h"
#include "../Containers/ContainerGrid.h"
#include "../Controls/ButtonControl.h"

/**
 * Class for a control that can be used to emulate key presses via touch inputs.
 * It displays one UI_Element that receives all emulated key presses and a matrix with many BoolControl elements to input key presses.
 */
class VirtualKeys : public UIElement
{
	private:
		ContainerGrid _grid;						/**< Grid used to combine all virtual keys. */
		ButtonControl _button0;						/**< Virtual keys button 0 */
		ButtonControl _button1;						/**< Virtual keys button 1 */
		ButtonControl _button2;						/**< Virtual keys button 2 */
		ButtonControl _button3;						/**< Virtual keys button 3 */
		ButtonControl _button4;						/**< Virtual keys button 4 */
		ButtonControl _button5;						/**< Virtual keys button 5 */
		ButtonControl _button6;						/**< Virtual keys button 6 */
		ButtonControl _button7;						/**< Virtual keys button 7 */
		ButtonControl _button8;						/**< Virtual keys button 8 */
		ButtonControl _button9;						/**< Virtual keys button 9 */
		ButtonControl _buttonLeft;					/**< Virtual keys button Left */
		ButtonControl _buttonRight;					/**< Virtual keys button Right */
		ButtonControl _buttonMilli;					/**< Virtual keys button Milli */
		ButtonControl _buttonKilo;					/**< Virtual keys button Kilo */
		ButtonControl _buttonX1;					/**< Virtual keys button x1 */
		ButtonControl _buttonMinus;					/**< Virtual keys button Minus */
		ButtonControl _buttonUp;					/**< Virtual keys button Up */
		ButtonControl _buttonOk;					/**< Virtual keys button Ok */
		ButtonControl _buttonDown;					/**< Virtual keys button Down */
		ButtonControl _buttonExitVirtualKeys;		/**< Virtual keys button Exit */
		static void _onButton0Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY0); }
		static void _onButton1Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY1); }
		static void _onButton2Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY2); }
		static void _onButton3Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY3); }
		static void _onButton4Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY4); }
		static void _onButton5Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY5); }
		static void _onButton6Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY6); }
		static void _onButton7Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY7); }
		static void _onButton8Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY8); }
		static void _onButton9Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEY9); }
		static void _onButtonLeftClick(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEYLEFT); }
		static void _onButtonRightClick(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEYRIGHT); }
		static void _onButtonMilliClick(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEYMILLI); }
		static void _onButtonKiloClick(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEYKILO); }
		static void _onButtonX1Click(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEYX1); }
		static void _onButtonMinusClick(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEYMINUS); }
		static void _onButtonUpClick(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEYUP); }
		static void _onButtonOkClick(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEYOK); }
		static void _onButtonDownClick(void* controlContext) { ((VirtualKeys*)controlContext)->_handleButtonPress(KEYDOWN); }
        static void _onButtonExitVirtualKeysClick(void* controlContext) { ((VirtualKeys*)controlContext)->ExitVirtualKeys(); }
		
		/**
		 * Method that is called whenever a button is pressed.
		 * @param key Pressed key 
		 */
		void _handleButtonPress(Keys_t key);
        UIElement* _targetElement;					/**< UI_Element that is edited by the virtual keys. */
        UIElement* _previousVisualTreeRoot;			/**< Previous visual tree root. This is used to change the visual tree root back on exit. */

	public:
        void(*OnVirtualKeyPressed)(Keys_t key);		/**< Function pointer for OnVirtualKeyPressed event */

		/**
		 * Constructor of the VirtualKeys.
		 * @param onVirtualKeyPressed Function pointer for OnVirtualKeyPressed event 
		 */
		VirtualKeys(void(*onVirtualKeyPressed)(Keys_t key) = NULL);
		
		/**
		 * Change the visual tree root to show the virtual keys with the given target element that will be edited.
		 * @param targetElement UI_Element that is edited by the virtual keys.
		 */
		void ShowVirtualKeys(UIElement* targetElement);
		
		/**
		 * Exit the virtual keys and change the visual tree root back to the _previousVisualTreeRoot
		 */
		void ExitVirtualKeys();
		
		/**
		 * Method used for drawing of the VirtualKeys.
		 */
		virtual void Draw(bool redraw) override;

		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYOK, KEYUP, KEYDOWN
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
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;

		/**
		 * Recalculate the UIElement layout (containers update the X- and Y-Location of all their items, all other items can do other layout update stuff)
		 */
		virtual void RecalculateLayout() override;
};

#endif /* VIRTUALKEYS_H_ */