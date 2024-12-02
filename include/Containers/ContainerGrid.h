/**
 * @file	ContainerGrid.h
 * @brief	Containing a class for a container that is showing all items at a time in a fixed grid layout. Therefore the location of the added items is changed.
 */ 

#ifndef CONTAINERGRID_H_
#define CONTAINERGRID_H_

#include "Container.h"

#define DEFAULT_MAX_CONTAINERGRID_ROWS		10		/**< Default maximum number of rows, each container grid can hold. Lower this value if you don't need that much items to save memory. */
#define DEFAULT_MAX_CONTAINERGRID_COLUMNS	10		/**< Default maximum number of columns, each container grid can hold. Lower this value if you don't need that much items to save memory. */

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
	uint8_t columnSpan;						/**< Number of columns over which the item is placed (starting from column index) */
	uint8_t rowSpan;						/**< Number of rows over which the item is placed (starting from row index) */
};

/**
 * Class for a container that is showing all items at a time in a fixed grid layout.
 */
class ContainerGrid : public Container
{
	private:
		uint8_t _maxGridRows;							/**< Maximum number of rows, each container grid can hold. Lower this value if you don't need that much items to save memory. */
		uint8_t _maxGridColumns;						/**< Maximum number of columns, each container grid can hold. Lower this value if you don't need that much items to save memory. */
		bool _showGridCells;							/**< Enable this to draw the grid cells */
		bool _autoSizingFillLast;						/**< If true, keep all columns or rows as small as possible except the last one set to auto size; if false, evenly increase the size of all columns or rows set to auto size */
		uint16_t* _initialRowHeights;					/**< Heights for the individual rows. 0 if row sizes to the largest item. */
		uint16_t* _initialColumnWidths;					/**< Widths for the individual columns. 0 if column sizes to the largest item. */
		uint16_t* _rowHeights;							/**< Heights for the individual rows. They are recalculated by the RecalculateLayout() method. */
		uint16_t* _columnWidths;						/**< Widths for the individual columns. They are recalculated by the RecalculateLayout() method. */
		GridItemConfig* _itemConfiguration;				/**< Configuration data for all items inside the grid. */

	public:

		/**
		 * Constructor of the ContainerGrid.
		 * @param maxNumItems Maximum number of items, each container can hold. Lower this value if you don't need that much items to save memory.
		 * @param maxGridRows Maximum number of rows, each container grid can hold. Lower this value if you don't need that much items to save memory.
 		 * @param maxGridColumns Maximum number of columns, each container grid can hold. Lower this value if you don't need that much items to save memory.
 		 * @param showGridCells Enable this to draw the grid cells
 		 * @param autoSizingFillLast If true, keep all columns or rows as small as possible except the last one set to auto size; if false, evenly increase the size of all columns or rows set to auto size
		 * @param locX X Location of the upper left corner of the ContainerGrid 
		 * @param locY Y Location of the upper left corner of the ContainerGrid
		 * @param width Drawing width of the ContainerGrid
		 * @param height Drawing height of the ContainerGrid
		 */
		ContainerGrid(uint8_t maxNumItems = DEFAULT_MAX_CONTAINER_ITEMS, uint8_t maxGridRows = DEFAULT_MAX_CONTAINERGRID_ROWS, uint8_t maxGridColumns = DEFAULT_MAX_CONTAINERGRID_COLUMNS, bool showGridCells = false, bool autoSizingFillLast = true, uint16_t locX = 0, uint16_t locY = 0, uint16_t width = 0, uint16_t height = 0);

		/**
		 * Destructor of the ContainerGrid
		 */
		~ContainerGrid()
		{
			delete _initialRowHeights;
			delete _initialColumnWidths;
			delete _rowHeights;
			delete _columnWidths;
			delete _itemConfiguration;
			_initialRowHeights = NULL;
			_initialColumnWidths = NULL;
			_rowHeights = NULL;
			_columnWidths = NULL;
			_itemConfiguration = NULL;
		}

		/**
		 * Method used for drawing of the ContainerGrid.
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
		 * Set the height of a specific row. 0 if row sizes to the largest item.
		 * @param rowIndex Index of the row to set.
		 * @param rowHeight New height for the row.
		 * @return true, if set; otherwise false (if index is out of range) 
		 */
		bool SetRowHeight(uint8_t rowIndex, uint16_t rowHeight);

		/**
		 * Set the width of a specific column. 0 if column sizes to the largest item.
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
		 * @param columnSpan Number of columns over which the item is placed (starting from column index)
		 * @param rowSpan Number of rows over which the item is placed (starting from row index)
		 * @return true, if added; otherwise false (if container is full) 
		 */
		bool AddItem(UIElement* item, uint8_t columnIndex, uint8_t rowIndex, GridCellAlignment_t cellAlignment = GRID_CELL_ALIGNMENT_MIDDLE, uint8_t columnSpan = 1, uint8_t rowSpan = 1);

		/**
		 * Recalculate the Height and Width of the UIElement
		 */
		virtual void RecalculateDimensions() override;

		/**
		 * Recalculate the UIElement layout (containers update the X- and Y-Location of all their items, all other items can do other layout update stuff)
		 */
		virtual void RecalculateLayout() override;
};

#endif /* CONTAINERGRID_H_ */