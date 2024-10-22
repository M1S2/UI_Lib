/*
 * ContainerGrid.cpp
 */ 

#include "Containers/ContainerGrid.h"
#include "Core/UI_Manager.h"

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::ContainerGrid()
{
	this->Type = UI_CONTAINER;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::ContainerGrid(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::Draw(bool redraw)
{
	if (this->Visible)
	{
		redraw = redraw || !this->_lastDrawnVisible;
		this->_lastDrawnVisible = true;
		if(showGridCells && redraw)
		{
			UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);

			uint16_t currentY = this->LocY;
			UiManager.Gfx->drawFastHLine(this->LocX, currentY, this->Width, UiManager.ColorForeground);
			for(int r = 0; r < maxGridRows; r++)
			{
				if(_rowHeights[r] == 0) { break; }
				currentY += _rowHeights[r];
				UiManager.Gfx->drawFastHLine(this->LocX, currentY, this->Width, UiManager.ColorForeground);
			}

			uint16_t currentX = this->LocX;
			UiManager.Gfx->drawFastVLine(currentX, this->LocY, this->Height, UiManager.ColorForeground);
			for(int c = 0; c < maxGridColumns; c++)
			{
				if(_columnWidths[c] == 0) { break; }
				currentX += _columnWidths[c];
				UiManager.Gfx->drawFastVLine(currentX, this->LocY, this->Height, UiManager.ColorForeground);
			}
		}
		
		for (int i = 0; i < this->_numItems; i++)
		{
			this->_items[i]->Draw(redraw);
		}
	}
	else if(!this->Visible && this->_lastDrawnVisible)		// clear only when the Visible property changes from true to false
	{
		this->_lastDrawnVisible = false;
		UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::KeyInput(Keys_t key)
{
	switch (key)
	{
		case KEYUP:
			return this->NextControlItem();
		case KEYDOWN:
			return this->PreviousControlItem();
		default:
			return false;
	}
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::InitItems()
{
	this->_selectedItemIndex = 0;
	if (this->GetSelectedItem()->Type != UI_CONTROL || this->GetSelectedItem()->Visible == false)
	{
		this->NextControlItem();
	}
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::SetRowHeight(uint8_t rowIndex, uint16_t rowHeight)
{
	if(rowIndex >= maxGridRows) { return false; }

	_initialRowHeights[rowIndex] = rowHeight;
	return true;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::SetColumnWidth(uint8_t columnIndex, uint16_t columnWidth)
{
	if(columnIndex >= maxGridColumns) { return false; }

	_initialColumnWidths[columnIndex] = columnWidth;
	return true;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::AddItem(UIElement* item, uint8_t columnIndex, uint8_t rowIndex, GridCellAlignment_t cellAlignment, uint8_t columnSpan, uint8_t rowSpan)
{
	if(columnIndex >= maxGridColumns || rowIndex >= maxGridRows) { return false; }

	GridItemConfig itemConfig;
	itemConfig.item = item;
	itemConfig.columnIndex = columnIndex;
	itemConfig.rowIndex = rowIndex;
	itemConfig.cellAlignment = cellAlignment;
	itemConfig.columnSpan = columnSpan;
	itemConfig.rowSpan = rowSpan;
	_itemConfiguration[this->_numItems] = itemConfig;

	if(!Container<maxItems>::AddItem(item)) { return false; }

	return true;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::RecalculateDimensions()
{
	this->Width = 0;
	for(int i = 0; i <= maxGridColumns; i++)
	{
		if(_columnWidths[i] == 0) { break; }
		this->Width += _columnWidths[i];
	}

	this->Height = 0;
	for(int i = 0; i <= maxGridRows; i++)
	{
		if(_rowHeights[i] == 0) { break; }
		this->Height += _rowHeights[i];
	}
}

static int CompareItemConfigByColumnSpanAscending(const void* a, const void* b)
{
	// Sorting ascending by columnSpan
	return (((GridItemConfig*)a)->columnSpan - ((GridItemConfig*)b)->columnSpan);
}

static int CompareItemConfigByRowSpanAscending(const void* a, const void* b)
{
	// Sorting ascending by rowSpan
	return (((GridItemConfig*)a)->rowSpan - ((GridItemConfig*)b)->rowSpan);
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells, autoSizingFillLast>::RecalculateLayout()
{
	// clear _columnWidths and _rowHeights (set all to 0)
	memset(_columnWidths, 0, maxGridColumns * sizeof(_columnWidths[0]));
	memset(_rowHeights, 0, maxGridRows * sizeof(_rowHeights[0]));
	GridItemConfig tempItemConfiguration[maxItems];
	/************************************************************************************
	 * RECALCULATE COLUMN WIDTHS                                                        *
	 * sort the item configurations ascending by column span                            */
	memcpy(tempItemConfiguration, _itemConfiguration, maxItems * sizeof(GridItemConfig));
	qsort(tempItemConfiguration, maxItems, sizeof(GridItemConfig), CompareItemConfigByColumnSpanAscending);
	for(int i = 0; i < maxItems; i++)
	{
		GridItemConfig currentItemConfig = tempItemConfiguration[i];
		UIElement* currentItem = currentItemConfig.item;
		if(currentItem == NULL || currentItemConfig.columnSpan == 0) { continue; }
		currentItem->RecalculateDimensions();

		// First find the number of auto sized columns or the remaining size to distribute to the auto sized columns (only regard the columns that should be used for the current item; columnIndex, columnSpan)
		uint8_t numAutoSizeColumns = 0;
		int16_t remainingItemWidthToDistribute = currentItem->Width;
		for(int columnIndexAdapted = currentItemConfig.columnIndex; columnIndexAdapted < (currentItemConfig.columnIndex + currentItemConfig.columnSpan); columnIndexAdapted++)
		{
			if(_initialColumnWidths[columnIndexAdapted] == 0)
			{
			  numAutoSizeColumns++;
			}
			else
			{
			  // Fixed size columns don't need to be used for auto sizing
			  remainingItemWidthToDistribute -= _initialColumnWidths[columnIndexAdapted];
			  _columnWidths[columnIndexAdapted] = _initialColumnWidths[columnIndexAdapted];
			}
		}
		// Then size all columns that are configured for the current item
		for(int columnIndexAdapted = currentItemConfig.columnIndex; columnIndexAdapted < (currentItemConfig.columnIndex + currentItemConfig.columnSpan); columnIndexAdapted++)
		{
		  // Only auto size if the current column is set for auto sizing (size == 0) and there are auto sizing columns left and if there is remainingItemWidthToDistribute left
		  if(_initialColumnWidths[columnIndexAdapted] == 0 && numAutoSizeColumns > 0 && remainingItemWidthToDistribute > 0)
		  {
		    uint16_t calculatedColumnSize = 0;
		    if(autoSizingFillLast)
		    {
			    if(numAutoSizeColumns > 1)            // This isn't the last available column set to auto sizing -> use the current column width
			    {
			      calculatedColumnSize = _columnWidths[columnIndexAdapted];
			    }
			    else if(numAutoSizeColumns <= 1)      // This is the last available column set to auto sizing -> use the remaining size
			    {
			      calculatedColumnSize = remainingItemWidthToDistribute;
			    }
		    }
		    else
		    {
		      calculatedColumnSize = remainingItemWidthToDistribute / numAutoSizeColumns;
		    }
		    
		    if(calculatedColumnSize > _columnWidths[columnIndexAdapted])
		    {
		      _columnWidths[columnIndexAdapted] = calculatedColumnSize;
		      remainingItemWidthToDistribute -= calculatedColumnSize;
		    }
		    else
		    {
		      remainingItemWidthToDistribute -= _columnWidths[columnIndexAdapted];
		    }
		    numAutoSizeColumns--;
		  }
		}
	}
	
	/************************************************************************************
	 * RECALCULATE ROW HEIGHTS                                                          *
	 * sort the item configurations ascending by row span                               */
	memcpy(tempItemConfiguration, _itemConfiguration, maxItems * sizeof(GridItemConfig));
	qsort(tempItemConfiguration, maxItems, sizeof(GridItemConfig), CompareItemConfigByRowSpanAscending);
	for(int i = 0; i < maxItems; i++)
	{
		GridItemConfig currentItemConfig = tempItemConfiguration[i];
		UIElement* currentItem = currentItemConfig.item;
		if(currentItem == NULL || currentItemConfig.rowSpan == 0) { continue; }
		currentItem->RecalculateDimensions();

		// First find the number of auto sized rows or the remaining size to distribute to the auto sized rows (only regard the rows that should be used for the current item; rowIndex, rowSpan)
		uint8_t numAutoSizeRows = 0;
		int16_t remainingItemHeightToDistribute = currentItem->Height;
		for(int rowIndexAdapted = currentItemConfig.rowIndex; rowIndexAdapted < (currentItemConfig.rowIndex + currentItemConfig.rowSpan); rowIndexAdapted++)
		{
			if(_initialRowHeights[rowIndexAdapted] == 0)
			{
			  numAutoSizeRows++;
			}
			else
			{
			  // Fixed size rows don't need to be used for auto sizing
			  remainingItemHeightToDistribute -= _initialRowHeights[rowIndexAdapted];
			  _rowHeights[rowIndexAdapted] = _initialRowHeights[rowIndexAdapted];
			}
		}
		// Then size all rows that are configured for the current item
		for(int rowIndexAdapted = currentItemConfig.rowIndex; rowIndexAdapted < (currentItemConfig.rowIndex + currentItemConfig.rowSpan); rowIndexAdapted++)
		{
		  // Only auto size if the current row is set for auto sizing (size == 0) and there are auto sizing rows left and if there is remainingItemHeightToDistribute left
		  if(_initialRowHeights[rowIndexAdapted] == 0 && numAutoSizeRows > 0 && remainingItemHeightToDistribute > 0)
		  {
		    uint16_t calculatedRowSize = 0;
		    if(autoSizingFillLast)
		    {
			    if(numAutoSizeRows > 1)            // This isn't the last available row set to auto sizing -> use the current row height
			    {
			      calculatedRowSize = _rowHeights[rowIndexAdapted];
			    }
			    else if(numAutoSizeRows <= 1)      // This is the last available row set to auto sizing -> use the remaining size
			    {
			      calculatedRowSize = remainingItemHeightToDistribute;
			    }
		    }
		    else
		    {
		      calculatedRowSize = remainingItemHeightToDistribute / numAutoSizeRows;
		    }
		    
		    if(calculatedRowSize > _rowHeights[rowIndexAdapted])
		    {
		      _rowHeights[rowIndexAdapted] = calculatedRowSize;
		      remainingItemHeightToDistribute -= calculatedRowSize;
		    }
		    else
		    {
		      remainingItemHeightToDistribute -= _rowHeights[rowIndexAdapted];
		    }
		    numAutoSizeRows--;
		  }
		}
	}
	
	/************************************************************************************
	 * PLACE EACH ITEM INSIDE CONTAINER REGION                                          */
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		GridItemConfig currentItemConfig = _itemConfiguration[i];

		uint16_t columnStart = this->LocX, columnEnd = 0;
		for(int c = 0; c <= (currentItemConfig.columnIndex + currentItemConfig.columnSpan - 1); c++)
		{		
			if(c >= maxGridColumns - 1) { break; }
			
			if(c <= currentItemConfig.columnIndex)		// The start of the column wasn't reached yet.
			{
				if(c >= 1)
				{
					columnStart += _columnWidths[c - 1];
				}
				columnEnd = columnStart + _columnWidths[c];
			}
			else			// The column was reached. Take the column span in account here.
			{
				columnEnd += _columnWidths[c];
			}
		}

		uint16_t rowStart = this->LocY, rowEnd = 0;
		for(int r = 0; r <= (currentItemConfig.rowIndex + currentItemConfig.rowSpan - 1); r++)
		{
			if(r >= maxGridRows - 1) { break; }
			
			if(r <= currentItemConfig.rowIndex)		// The start of the row wasn't reached yet.
			{
				if(r >= 1)
				{
					rowStart += _rowHeights[r - 1];
				}
				rowEnd = rowStart + _rowHeights[r];
			}
			else			// The row was reached. Take the row span in account here.
			{
				rowEnd += _rowHeights[r];
			}
		}

		if(columnEnd < columnStart || rowEnd < rowStart)
		{
			continue;
		}
		else
		{
			switch (currentItemConfig.cellAlignment)
			{
				case GRID_CELL_ALIGNMENT_TOP_LEFT:
					currentItem->LocX = columnStart;
					currentItem->LocY = rowStart;
					break;
				case GRID_CELL_ALIGNMENT_LEFT:
					currentItem->LocX = columnStart;
					currentItem->LocY = rowStart + ((rowEnd - rowStart - currentItem->Height) / 2);
					break;
				case GRID_CELL_ALIGNMENT_BOTTOM_LEFT:
					currentItem->LocX = columnStart;
					currentItem->LocY = rowEnd - currentItem->Height;
					break;
				case GRID_CELL_ALIGNMENT_TOP:
					currentItem->LocX = columnStart + ((columnEnd - columnStart - currentItem->Width) / 2);
					currentItem->LocY = rowStart;
					break;
				case GRID_CELL_ALIGNMENT_MIDDLE:
					currentItem->LocX = columnStart + ((columnEnd - columnStart - currentItem->Width) / 2);
					currentItem->LocY = rowStart + ((rowEnd - rowStart - currentItem->Height) / 2);
					break;
				case GRID_CELL_ALIGNMENT_BOTTOM:
					currentItem->LocX = columnStart + ((columnEnd - columnStart - currentItem->Width) / 2);
					currentItem->LocY = rowEnd - currentItem->Height;
					break;
				case GRID_CELL_ALIGNMENT_TOP_RIGHT:
					currentItem->LocX = columnEnd - currentItem->Width;
					currentItem->LocY = rowStart;
					break;
				case GRID_CELL_ALIGNMENT_RIGHT:
					currentItem->LocX = columnEnd - currentItem->Width;
					currentItem->LocY = rowStart + ((rowEnd - rowStart - currentItem->Height) / 2);
					break;
				case GRID_CELL_ALIGNMENT_BOTTOM_RIGHT:
					currentItem->LocX = columnEnd - currentItem->Width;
					currentItem->LocY = rowEnd - currentItem->Height;
					break;
				default:
					break;
			}

			currentItem->RecalculateLayout();
			currentItem->RecalculateDimensions();
		}
	}
}