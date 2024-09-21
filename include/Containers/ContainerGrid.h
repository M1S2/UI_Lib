/**
 * @file	ContainerGrid.h
 * @brief	Containing a class for a container that is showing all items at a time in a fixed grid layout. Therefore the location of the added items is changed.
 */ 

#ifndef CONTAINERGRID_H_
#define CONTAINERGRID_H_

#include "ContainerPage.h"

#define MAX_CONTAINER_GRID_ROWS		10		/**< Maximum number of rows, each container grid can hold. Lower this value if you don't need that much items to save memory. */
#define MAX_CONTAINER_GRID_COLUMNS	10		/**< Maximum number of columns, each container grid can hold. Lower this value if you don't need that much items to save memory. */

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
 * class for a container that is showing all items at a time in a fixed grid layout.
 * Therefore the location of the added items is changed.
 */
class ContainerGrid : public ContainerPage
{
	private:
		uint16_t _rowHeights[MAX_CONTAINER_GRID_ROWS];			/**< Heights for the individual rows. 0 if not used. */
		uint16_t _columnWidths[MAX_CONTAINER_GRID_COLUMNS];		/**< Widths for the individual columns. 0 if not used. */
		
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
		 * @param gfx Pointer to the Adafruit_GFX object used for LCD drawing.
		 * @param wasScreenCleared This parameter indicates that the complete screen was cleared. Draw everything again.
		 */
		virtual void Draw(Adafruit_GFX* gfx, bool wasScreenCleared) override;

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
};

#endif /* CONTAINERGRID_H_ */