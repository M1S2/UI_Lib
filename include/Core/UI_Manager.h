/**
 * @file	UI_Manager.h
 * @brief	Containing a class that is used to handle the drawing and key handling of all UI_Elements.
 */ 

#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include "UIElement.h"

#define UI_LIB_DEFAULT_COLOR_BACKGROUND				RGB565(0x00, 0x00, 0x00)			/**< Default Background color (black) */
#define UI_LIB_DEFAULT_COLOR_FOREGROUND				RGB565(0xFF, 0xFF, 0xFF)			/**< Default UIElement foreground color (white) */	
#define UI_LIB_DEFAULT_COLOR_FOREGROUND_EDIT_MODE	UI_LIB_DEFAULT_COLOR_BACKGROUND		/**< Default UIElement foreground color in edit mode (black). When some controls are in edit mode, the background is drawn in the foreground color and the text is drawn with this color. */	
#define UI_LIB_DEFAULT_FONT 						&FreeSans12pt7b						/**< Default font used for all UIElements. */
#define UI_LIB_DEFAULT_ELEMENT_MARGIN				1									/**< Space without content around every UIElement. This is part of the UIElement size. */
#define UI_LIB_DEFAULT_ELEMENT_PADDING				2									/**< Space between the outline of every UIElement and the internal content. This is part of the UIElement size. */

/**
 * Class that is used to handle the drawing and key handling of all UI_Elements.
 */
class UI_Manager
{
	private:
		UIElement* _visualTreeRoot;					/**< Root element of the visual tree. This element and all children are drawn to the screen. */
		UIElement* _focusElement;					/**< Element that has the focus (it is highlited and receives all key inputs). */
		UIElement* _lastDrawnFocusElement;			/**< Last drawn focus element. */
		
		UIElement* _lastElementInEditMode = NULL;

		/**
		 * Traverse down the visual tree until an element without a child is reached and focus this element.
		 */
		void setFocusToLeaf();

		/** Constructor of the UI_Manager */
		UI_Manager();

	public:
		uint16_t ColorBackground;					/**< Background color (black) */
		uint16_t ColorForeground;					/**< UIElement color (green) */	
		uint16_t ColorForegroundEditMode;			/**< Default UIElement foreground color in edit mode (black). When some controls are in edit mode, the background is drawn in the foreground color and the text is drawn with this color. */	
		const GFXfont* Font;						/**< Default font used when no other font is assigned */
		uint16_t FontHeight;						/**< Maximum height of the Font (height of the string "Ag"). This parameter is recalculated when the SetFont() method is used. */
		uint16_t ElementMargin;						/**< Space without content around every UIElement. This is part of the UIElement size. */
		uint16_t ElementPadding;					/**< Space between the outline of every UIElement and the internal content. This is part of the UIElement size. */
		bool CompleteRedrawRequested;				/**< True, if the tree root was currently changed. This is set to false on the first draw then. */
		Adafruit_GFX* Gfx;							/**< Graphics object */
		
		/**
		 * Get the singleton instance of the UI_Manager class.
		 * Usage:
		 * UI_Manager UiManager = UI_Manager::getInstance();
		 */
		static UI_Manager& getInstance()
        {
            static UI_Manager instance; 	// Instantiated on first use.
            return instance;
        }

		/**
		 * Initialize the UI_Manager.
		 * This method sets the font and foreground color of the graphic library.
		 */
		void Init(Adafruit_GFX* gfx);

		/**
		 * Set all colors.
		 * @param colorBackground Background color
		 * @param colorForeground UIElement color
		 * @param colorForegroundEditMode UIElement foreground color in edit mode (black). When some controls are in edit mode, the background is drawn in the foreground color and the text is drawn with this color.
		 */
		void SetColors(uint16_t colorBackground, uint16_t colorForeground, uint16_t colorForegroundEditMode);

		/**
		 * Set the Font property and recalculate the FontHeight based on this font
		 * @param font New font to use for all UIElements
		 */
		void SetFont(const GFXfont* font);

		/**
		 * Draw the complete visual tree (_visualTreeRoot and all of its children).
		 */
		void Draw();

		/**
		 * Change the root element of the visual tree. 
		 * This changes the _visualTreeRoot variable and recalculates the _focusElement.
		 * @param visualTreeRoot UIElement that becomes the new root element of the visual tree. 
		 */
		void ChangeVisualTreeRoot(UIElement* visualTreeRoot);
		
		/**
		 * Forward the given key to the _focusElement.
		 * If the element type of the _focusElement doesn't support the key type, the key is forwarded to the parent of the _focusElement.
		 * This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed.
		 * @return true if the key was processed; false if no control in the visual tree has processed the key.
		 */
		bool KeyInput(Keys_t key);

		/**
		 * Process a touch input at the given point (x, y)
		 * @param x X-Coordinate of the touched point
		 * @param y Y-Coordinate of the touched point
		 * @return true if the touch was processed; false if not.
		 */
		bool TouchInput(uint16_t x, uint16_t y);

		/**
		 * Update the IsInEditMode property of the given UIElement to the given newEditModeState.
		 * This method makes sure that the IsInEditMode property of the UIElement that was previously modified using this method is deactivated.
		 * So only one UIElement can have the IsInEditMode property active at a time.
		 * @param element UIElement for which the IsInEditMode property is modified.
		 * @param newEditModeState New Value for the IsInEditMode property.
		 */
		void UpdateIsInEditModeElement(UIElement* element, bool newEditModeState);
};

extern UI_Manager UiManager;			/**< Access object for the singleton instance of the UI_Manager */

#endif /* UI_MANAGER_H_ */