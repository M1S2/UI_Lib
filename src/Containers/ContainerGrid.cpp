/*
 * ContainerGrid.cpp
 */ 

#include "Containers/ContainerGrid.h"
#include "Core/UI_Manager.h"

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::ContainerGrid()
{
	this->Type = UI_CONTAINER;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::ContainerGrid(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::Draw(bool redraw)
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

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::KeyInput(Keys_t key)
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

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::InitItems()
{
	this->_selectedItemIndex = 0;
	if (this->GetSelectedItem()->Type != UI_CONTROL || this->GetSelectedItem()->Visible == false)
	{
		this->NextControlItem();
	}
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::SetRowHeight(uint8_t rowIndex, uint16_t rowHeight)
{
	if(rowIndex >= maxGridRows) { return false; }

	_initialRowHeights[rowIndex] = rowHeight;
	return true;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::SetColumnWidth(uint8_t columnIndex, uint16_t columnWidth)
{
	if(columnIndex >= maxGridColumns) { return false; }

	_initialColumnWidths[columnIndex] = columnWidth;
	return true;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::AddItem(UIElement* item, uint8_t columnIndex, uint8_t rowIndex, GridCellAlignment_t cellAlignment)
{
	if(columnIndex >= maxGridColumns || rowIndex >= maxGridRows) { return false; }

	GridItemConfig itemConfig;
	itemConfig.item = item;
	itemConfig.columnIndex = columnIndex;
	itemConfig.rowIndex = rowIndex;
	itemConfig.cellAlignment = cellAlignment;
	_itemConfiguration[this->_numItems] = itemConfig;

	if(!Container<maxItems>::AddItem(item)) { return false; }

	return true;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::RecalculateDimensions()
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

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns, bool showGridCells>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns, showGridCells>::RecalculateLayout()
{
	// clear _columnWidths and _rowHeights (set all to 0)
	memset(_columnWidths, 0, maxGridColumns * sizeof(_columnWidths[0]));
	memset(_rowHeights, 0, maxGridRows * sizeof(_rowHeights[0]));
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		GridItemConfig currentItemConfig = _itemConfiguration[i];
		
		if(_initialColumnWidths[currentItemConfig.columnIndex] == 0)
		{
			currentItem->RecalculateDimensions();
			if(currentItem->Width > _columnWidths[currentItemConfig.columnIndex])
			{
				_columnWidths[currentItemConfig.columnIndex] = currentItem->Width;
			}
		}
		else
		{
			_columnWidths[currentItemConfig.columnIndex] = _initialColumnWidths[currentItemConfig.columnIndex];
		}
		if(_initialRowHeights[currentItemConfig.rowIndex] == 0)
		{
			currentItem->RecalculateDimensions();
			if(currentItem->Height > _rowHeights[currentItemConfig.rowIndex])
			{
				_rowHeights[currentItemConfig.rowIndex] = currentItem->Height;
			}
		}
		else
		{
			_rowHeights[currentItemConfig.rowIndex] = _initialRowHeights[currentItemConfig.rowIndex];
		}
	}

	// Place each item inside container region
	for(int i = 0; i < this->_numItems; i++)
	{
		UIElement* currentItem = this->_items[i];
		GridItemConfig currentItemConfig = _itemConfiguration[i];

		uint16_t columnStart = this->LocX, columnEnd = 0;
		for(int c = 0; c <= currentItemConfig.columnIndex; c++)
		{
			if(_columnWidths[c] == 0) { break; }
			
			if(c >= 1)
			{
				columnStart += _columnWidths[c - 1];
			}
			columnEnd = columnStart + _columnWidths[c];
		}

		uint16_t rowStart = this->LocY, rowEnd = 0;
		for(int r = 0; r <= currentItemConfig.rowIndex; r++)
		{
			if(_rowHeights[r] == 0) { break; }
			
			if(r >= 1)
			{
				rowStart += _rowHeights[r - 1];
			}
			rowEnd = rowStart + _rowHeights[r];
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