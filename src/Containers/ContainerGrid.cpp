/*
 * ContainerGrid.cpp
 */ 

#include "Containers/ContainerGrid.h"
#include "Core/UI_Manager.h"

ContainerGrid::ContainerGrid(uint8_t maxNumItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells, bool autoSizingFillLast, uint16_t locX, uint16_t locY, uint16_t width, uint16_t height) : Container(maxNumItems)
{
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
	_maxGridRows = maxGridRows;
	_maxGridColumns = maxGridColumns;
	_showGridCells = showGridCells;
	_autoSizingFillLast = autoSizingFillLast;
	_initialRowHeights = new uint16_t[maxGridRows]();
	_initialColumnWidths = new uint16_t[maxGridColumns]();
	_rowHeights = new uint16_t[maxGridRows]();
	_columnWidths = new uint16_t[maxGridColumns]();
	_itemConfiguration = new GridItemConfig[maxNumItems]();
}

void ContainerGrid::Draw(bool redraw)
{
	if (this->Visible)
	{
		redraw = redraw || !this->_lastDrawnVisible;
		this->_lastDrawnVisible = true;
		if(_showGridCells && redraw)
		{
			UiManager.Gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);

			uint16_t currentY = this->LocY;
			UiManager.Gfx->drawFastHLine(this->LocX, currentY, this->Width, UiManager.ColorForeground);
			for(int r = 0; r < _maxGridRows; r++)
			{
				if(_rowHeights[r] == 0) { break; }
				currentY += _rowHeights[r];
				UiManager.Gfx->drawFastHLine(this->LocX, currentY, this->Width, UiManager.ColorForeground);
			}

			uint16_t currentX = this->LocX;
			UiManager.Gfx->drawFastVLine(currentX, this->LocY, this->Height, UiManager.ColorForeground);
			for(int c = 0; c < _maxGridColumns; c++)
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

bool ContainerGrid::KeyInput(Keys_t key)
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

bool ContainerGrid::TouchInput(uint16_t x, uint16_t y, TouchTypes touchType)
{
	if(HitTest(x, y))
	{
		for (int i = 0; i < this->_numItems; i++)
		{
			bool touchResult = this->_items[i]->TouchInput(x, y, touchType);
			if(touchResult)
			{
				this->ActiveChild = this->_items[i];
				return true;
			}
		}
	}
	return false;
}

void ContainerGrid::InitItems()
{
	this->_selectedItemIndex = 0;
	if (this->GetSelectedItem()->Type != UI_CONTROL || this->GetSelectedItem()->Visible == false)
	{
		this->NextControlItem();
	}
}

bool ContainerGrid::SetRowHeight(uint8_t rowIndex, uint16_t rowHeight)
{
	if(rowIndex >= _maxGridRows) { return false; }

	_initialRowHeights[rowIndex] = rowHeight;
	return true;
}

bool ContainerGrid::SetColumnWidth(uint8_t columnIndex, uint16_t columnWidth)
{
	if(columnIndex >= _maxGridColumns) { return false; }

	_initialColumnWidths[columnIndex] = columnWidth;
	return true;
}

bool ContainerGrid::AddItem(UIElement* item, uint8_t columnIndex, uint8_t rowIndex, GridCellAlignment_t cellAlignment, uint8_t columnSpan, uint8_t rowSpan)
{
	if(columnIndex >= _maxGridColumns || rowIndex >= _maxGridRows) { return false; }

	GridItemConfig itemConfig;
	itemConfig.item = item;
	itemConfig.columnIndex = columnIndex;
	itemConfig.rowIndex = rowIndex;
	itemConfig.cellAlignment = cellAlignment;
	itemConfig.columnSpan = columnSpan;
	itemConfig.rowSpan = rowSpan;
	_itemConfiguration[this->_numItems] = itemConfig;

	if(!Container::AddItem(item)) { return false; }

	return true;
}

void ContainerGrid::RecalculateDimensions()
{
	this->Width = 0;
	for(int i = 0; i <= _maxGridColumns; i++)
	{
		if(_columnWidths[i] == 0) { break; }
		this->Width += _columnWidths[i];
	}

	this->Height = 0;
	for(int i = 0; i <= _maxGridRows; i++)
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

void ContainerGrid::RecalculateLayout()
{
	// clear _columnWidths and _rowHeights (set all to 0)
	memset(_columnWidths, 0, _maxGridColumns * sizeof(_columnWidths[0]));
	memset(_rowHeights, 0, _maxGridRows * sizeof(_rowHeights[0]));
	GridItemConfig tempItemConfiguration[_maxNumItems];
	/************************************************************************************
	 * RECALCULATE COLUMN WIDTHS                                                        *
	 * sort the item configurations ascending by column span                            */
	memcpy(tempItemConfiguration, _itemConfiguration, _maxNumItems * sizeof(GridItemConfig));
	qsort(tempItemConfiguration, _maxNumItems, sizeof(GridItemConfig), CompareItemConfigByColumnSpanAscending);
	for(int i = 0; i < _maxNumItems; i++)
	{
		GridItemConfig currentItemConfig = tempItemConfiguration[i];
		UIElement* currentItem = currentItemConfig.item;
		if(currentItem == NULL || currentItemConfig.columnSpan == 0) { continue; }
		currentItem->RecalculateDimensions();
		currentItem->RecalculateLayout();

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
		    if(_autoSizingFillLast)
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
	memcpy(tempItemConfiguration, _itemConfiguration, _maxNumItems * sizeof(GridItemConfig));
	qsort(tempItemConfiguration, _maxNumItems, sizeof(GridItemConfig), CompareItemConfigByRowSpanAscending);
	for(int i = 0; i < _maxNumItems; i++)
	{
		GridItemConfig currentItemConfig = tempItemConfiguration[i];
		UIElement* currentItem = currentItemConfig.item;
		if(currentItem == NULL || currentItemConfig.rowSpan == 0) { continue; }
		currentItem->RecalculateDimensions();
		currentItem->RecalculateLayout();

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
		    if(_autoSizingFillLast)
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
			if(c >= _maxGridColumns) { break; }
			
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
			if(r >= _maxGridRows) { break; }
			
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
	this->RecalculateDimensions();
}