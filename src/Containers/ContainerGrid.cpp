/*
 * ContainerGrid.cpp
 */ 

#include "Containers/ContainerGrid.h"
#include "Core/UI_Manager.h"

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns>
ContainerGrid<maxItems, maxGridRows, maxGridColumns>::ContainerGrid()
{
	this->Type = UI_CONTAINER;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns>
ContainerGrid<maxItems, maxGridRows, maxGridColumns>::ContainerGrid(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	this->Type = UI_CONTAINER;
	this->LocX = locX;
	this->LocY = locY;
	this->Width = width;
	this->Height = height;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns>::Draw(Adafruit_GFX* gfx)
{
	if (this->Visible)
	{
		#ifdef DEBUG_SHOW_CONTAINER_GRID_CELLS

			gfx->fillRect(this->LocX - 1, this->LocY - 1, this->Width + 2, this->Height + 2, UiManager.ColorBackground);

			uint16_t currentY = this->LocY;
			gfx->drawFastHLine(this->LocX, currentY, this->Width, UiManager.ColorForeground);
			for(int r = 0; r < maxGridRows; r++)
			{
				if(_rowHeights[r] == 0) { break; }
				currentY += _rowHeights[r];
				gfx->drawFastHLine(this->LocX, currentY, this->Width, UiManager.ColorForeground);
			}

			uint16_t currentX = this->LocX;
			gfx->drawFastVLine(currentX, this->LocY, this->Height, UiManager.ColorForeground);
			for(int c = 0; c < maxGridColumns; c++)
			{
				if(_columnWidths[c] == 0) { break; }
				currentX += _columnWidths[c];
				gfx->drawFastVLine(currentX, this->LocY, this->Height, UiManager.ColorForeground);
			}
		#endif
		ContainerPage<maxItems>::Draw(gfx);
	}
	else
	{
		gfx->fillRect(this->LocX, this->LocY, this->Width, this->Height, UiManager.ColorBackground);
	}
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns>::SetRowHeight(uint8_t rowIndex, uint16_t rowHeight)
{
	if(rowIndex >= maxGridRows) { return false; }

	_rowHeights[rowIndex] = rowHeight;
	return true;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns>::SetColumnWidth(uint8_t columnIndex, uint16_t columnWidth)
{
	if(columnIndex >= maxGridColumns) { return false; }

	_columnWidths[columnIndex] = columnWidth;
	return true;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns>
bool ContainerGrid<maxItems, maxGridRows, maxGridColumns>::AddItem(UIElement* item, uint8_t columnIndex, uint8_t rowIndex, GridCellAlignment_t cellAlignment)
{
	if(columnIndex >= maxGridColumns || rowIndex >= maxGridRows) { return false; }
	if(!Container<maxItems>::AddItem(item)) { return false; }

	// adapt location accordingly

	uint16_t columnStart = this->LocX, columnEnd = 0;
	for(int i = 0; i <= columnIndex; i++)
	{
		if(_columnWidths[i] == 0) { break; }
		
		if(i >= 1)
		{
			columnStart += _columnWidths[i - 1];
		}
		columnEnd = columnStart + _columnWidths[i];
	}

	uint16_t rowStart = this->LocY, rowEnd = 0;
	for(int i = 0; i <= rowIndex; i++)
	{
		if(_rowHeights[i] == 0) { break; }
		
		if(i >= 1)
		{
			rowStart += _rowHeights[i - 1];
		}
		rowEnd = rowStart + _rowHeights[i];
	}

	if(columnEnd < columnStart || rowEnd < rowStart)
	{
		return false;
	}
	else
	{
		switch (cellAlignment)
		{
			case GRID_CELL_ALIGNMENT_TOP_LEFT:
				item->LocX = columnStart;
				item->LocY = rowStart;
				break;
			case GRID_CELL_ALIGNMENT_LEFT:
				item->LocX = columnStart;
				item->LocY = rowStart + ((rowEnd - rowStart - item->Height) / 2);
				break;
			case GRID_CELL_ALIGNMENT_BOTTOM_LEFT:
				item->LocX = columnStart;
				item->LocY = rowEnd - item->Height;
				break;
			case GRID_CELL_ALIGNMENT_TOP:
				item->LocX = columnStart + ((columnEnd - columnStart - item->Width) / 2);
				item->LocY = rowStart;
				break;
			case GRID_CELL_ALIGNMENT_MIDDLE:
				item->LocX = columnStart + ((columnEnd - columnStart - item->Width) / 2);
				item->LocY = rowStart + ((rowEnd - rowStart - item->Height) / 2);
				break;
			case GRID_CELL_ALIGNMENT_BOTTOM:
				item->LocX = columnStart + ((columnEnd - columnStart - item->Width) / 2);
				item->LocY = rowEnd - item->Height;
				break;
			case GRID_CELL_ALIGNMENT_TOP_RIGHT:
				item->LocX = columnEnd - item->Width;
				item->LocY = rowStart;
				break;
			case GRID_CELL_ALIGNMENT_RIGHT:
				item->LocX = columnEnd - item->Width;
				item->LocY = rowStart + ((rowEnd - rowStart - item->Height) / 2);
				break;
			case GRID_CELL_ALIGNMENT_BOTTOM_RIGHT:
				item->LocX = columnEnd - item->Width;
				item->LocY = rowEnd - item->Height;
				break;
			default:
				break;
		}		
	}

	return true;
}

template <uint8_t maxItems, uint8_t maxGridRows, uint8_t maxGridColumns>
void ContainerGrid<maxItems, maxGridRows, maxGridColumns>::RecalculateDimensions()
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