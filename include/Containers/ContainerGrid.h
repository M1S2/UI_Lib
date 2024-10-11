/**
 * @file	ContainerGrid.h
 * @brief	Containing a class for a container that is showing all items at a time in a fixed grid layout. Therefore the location of the added items is changed.
 */ 

#ifndef CONTAINERGRID_H_
#define CONTAINERGRID_H_

#include "Container.h"

#define DEBUG_SHOW_CONTAINER_GRID_CELLS		/**< Enable this define to draw the grid cells */

/**
 * Available alignment in cell.
 */
typedef enum GridCellAlignment
{
	GRID_CELL_ALIGNMENT_TOP_LEFT,			/**< Alignment top left */
	GRID_CELL_ALIGNMENT_LEFT,				/**< Alignment left */
	GRID_CELL_ALIGNMENT_BOTTOM_LEFT,		/**< Alignment bottom left */
	GRID_CELL_ALIGNMENT_TOP,				/**< Alignment top */
	GRID_CELL_ALIGNMENT_MIDDLE,				/**< Alignment middle */
	GRID_CELL_ALIGNMENT_BOTTOM,				/**< Alignment bottom */
	GRID_CELL_ALIGNMENT_TOP_RIGHT,			/**< Alignment top right */
	GRID_CELL_ALIGNMENT_RIGHT,				/**< Alignment right */
	GRID_CELL_ALIGNMENT_BOTTOM_RIGHT,		/**< Alignment bottom right */
}GridCellAlignment_t;

/**
 * Struct used to store the column, row and alignment of a item inside the grid.
 */
struct GridItemConfig
{
	UIElement* item;						/**< Pointer to the item for which this configuration applies */
	uint8_t columnIndex;					/**< Column inside the grid where the item is placed */
	uint8_t rowIndex;						/**< Row inside the grid where the item is placed */
	GridCellAlignment_t cellAlignment;		/**< Alignment of the item inside the grid cell */
};

/**
 * Class for a container that is showing all items at a time in a fixed grid layout.
 * @tparam maxItems Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory.
 * @tparam maxGridRows Maximum number of rows, each container grid can hold. Lower this value if you don't need that much items to save memory.
 * @tparam maxGridColumns Maximum number of columns, each container grid can hold. Lower this value if you don't need that much items to save memory.
 */
template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns>
class ContainerGrid : public Container<maxItems>
{
	private:
		uint16_t _rowHeights[maxGridRows];				/**< Heights for the individual rows. 0 if not used. */
		uint16_t _columnWidths[maxGridColumns];			/**< Widths for the individual columns. 0 if not used. */
		GridItemConfig _itemConfiguration[maxItems];	/**< Configuration data for all items inside the grid. */

	public:

		/**
		 * Constructor of the ContainerGrid.
		 */
		ContainerGrid();

		/**
		 * Constructor of the ContainerGrid.
		 * @param locX X Location of the upper left corner of the ContainerGrid 
		 * @param locY Y Location of the upper left corner of the ContainerGrid
		 * @param width Drawing width of the ContainerGrid
		 * @param height Drawing height of the ContainerGrid
		 */
		ContainerGrid(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height);

		/**
		 * Method used for drawing of the ContainerGrid.
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
		 * Set the height of a specific row.
		 * @param rowIndex Index of the row to set.
		 * @param rowHeight New height for the row.
		 * @return true, if set; otherwise false (if index is out of range) 
		 */
		bool SetRowHeight(uint8_t rowIndex, uint16_t rowHeight);

		/**
		 * Set the width of a specific column.
		 * @param columnIndex Index of the column to set.
		 * @param columnWidth New width for the column.
		 * @return true, if set; otherwise false (if index is out of range) 
		 */
		bool SetColumnWidth(uint8_t columnIndex, uint16_t columnWidth);

		/**
		 * Add the item to the elements of this container and adapt the item location accordingly.
		 * @param item Pointer to the UIElement that should be added to the container. 
		 * @param columnIndex Index of the column to insert the item into.
		 * @param rowIndex Index of the row to insert the item into.
		 * @param cellAlignment Alignment for the cell.
		 * @return true, if added; otherwise false (if container is full) 
		 */
		bool AddItem(UIElement* item, uint8_t columnIndex, uint8_t rowIndex, GridCellAlignment_t cellAlignment);

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

#define MAX_CONTAINER_GRID_ROWS		10		/**< Maximum number of rows, each container grid can hold. Lower this value if you don't need that much items to save memory. */
#define MAX_CONTAINER_GRID_COLUMNS	10		/**< Maximum number of columns, each container grid can hold. Lower this value if you don't need that much items to save memory. */

typedef ContainerGrid<MAX_CONTAINER_ITEMS, MAX_CONTAINER_GRID_ROWS, MAX_CONTAINER_GRID_COLUMNS> ContainerGridDefault;		/**< Type definition for a ContainerGrid using the default settings. */

#endif /* CONTAINERGRID_H_ */