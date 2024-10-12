/**
 * @file	TabControl.h
 * @brief	Containing a class for a tab control that is showing a vertical or horizontal tab layout and can hold child elements.
 */ 

#ifndef TABCONTROL_H_
#define TABCONTROL_H_

#include "Container.h"

/**
 * Available tab positions.
 */
typedef enum TabPositions
{
	TAB_POSITION_LEFT,			/**< Tabs are on the left side */
	TAB_POSITION_TOP			/**< Tabs are on the top */
}TabPositions_t;

/**
 * Class for a tab control that is showing a tab layout and can hold child elements.
 * @tparam maxTabs Maximum number of tabs that are supported.
 * @tparam tabHeaderMargin Margin around the tab page tabs.
 * @tparam contentPadding Padding between the content region and the tab content
 */
template <uint8_t maxTabs, uint8_t tabHeaderMargin, uint8_t contentPadding>
class TabControl : public Container<maxTabs>
{
	private:
		UIElement* _headers[maxTabs];								/**< Array with all tab page tab header elements. */
		int _lastDrawnTabIndex;										/**< Index of the last drawn tab */
		TabPositions_t _tabPosition;								/**< Position of the tabs. */

		void* _controlContext;										/**< Context pointer that is returned with the _onSelectedTabChanged function pointer */
		void(*_onSelectedTabChanged)(void* controlContext);			/**< Function pointer for _onSelectedTabChanged event. This function is called when the selected tab is changed. */
		
	public:
		uint16_t TabRegionSize;										/**< Width or height of the Tabs in pixel. */

		/**
		 * Constructor of the TabControl.
		 * @param width Drawing width of the TabControl
		 * @param height Drawing height of the TabControl
		 * @param tabPosition Position of the tabs.
		 * @param controlContext Context pointer that is returned with the _onSelectedTabChanged function pointer
		 * @param onSelectedTabChanged Function pointer for _onSelectedTabChanged event. This function is called when the selected tab is changed.
		 */
		TabControl(uint16_t width, uint16_t height, TabPositions_t tabPosition = TAB_POSITION_LEFT, void* controlContext = NULL, void(*onSelectedTabChanged)(void* controlContext) = NULL);
	
		/**
		 * Constructor of the TabControl.
		 * @param locX X Location of the upper left corner of the TabControl 
		 * @param locY Y Location of the upper left corner of the TabControl
		 * @param width Drawing width of the TabControl
		 * @param height Drawing height of the TabControl
		 * @param tabPosition Position of the tabs.
		 * @param controlContext Context pointer that is returned with the _onSelectedTabChanged function pointer
		 * @param onSelectedTabChanged Function pointer for _onSelectedTabChanged event. This function is called when the selected tab is changed.
		 */
		TabControl(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height, TabPositions_t tabPosition = TAB_POSITION_LEFT, void* controlContext = NULL, void(*onSelectedTabChanged)(void* controlContext) = NULL);
	
		/**
		 * Method used for drawing of the TabControl.
		 */
		virtual void Draw() override;
		
		/**
		 * Process the given key.
		 * If this element doesn't support the key type, the key is forwarded to the parent. This is done until the key is processed or the root of the visual tree is reached.
		 * @param key Key that should be processed. Supported keys are: KEYLEFT, KEYRIGHT
s		 * @return true if the key was processed; false if not.
		 */
		virtual bool KeyInput(Keys_t key) override;
		
		/**
		 * Add a new tab to the tab control.
		 * @param header Tab page tab header UIElement.
		 * @param tabContent Pointer to the tab page content which can be of any UIElement.
		 * @return true, if added; otherwise false (if container is full) 
		 */
		bool AddItem(UIElement* header, UIElement* tabContent);
		
		/**
		 * Select the tab at the requested index (after range checking).
		 * @param index Index of the tab page that should be selected.
		 */
		void SelectTab(int index);
		
		/**
		 * Return the value of the selected tab index.
		 */
		int GetSelectedTabIndex();

		/**
		 * Get the size of the content region of the TabControl
		 * @param w Pointer used to give back the content region width
		 * @param h Pointer used to give back the content region height
		 */
		void GetContentRegionSize(uint16_t* w, uint16_t* h);

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

#define MAX_TABCONTROL_TABS			5		/**< Maximum number of tabs that are supported. */
#define TABCONTROL_TABHEADER_MARGIN	2		/**< Margin around the tab page tabs. */
#define TABCONTROL_CONTENT_PADDING	5		/**< Padding between the content region and the tab content */

typedef TabControl<MAX_TABCONTROL_TABS, TABCONTROL_TABHEADER_MARGIN, TABCONTROL_CONTENT_PADDING> TabControlDefault;		/**< Type definition for a TabControl using the default settings. */

#endif /* TABCONTROL_H_ */