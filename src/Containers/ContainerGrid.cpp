/*
 * ContainerGrid.cpp
 */ 

#include "Containers/ContainerGrid.h"

ContainerGrid::ContainerGrid(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	Type = UI_CONTAINER;
	LocX = locX;
	LocY = locY;
	Width = width;
	Height = height;
}

void ContainerGrid::Draw(Adafruit_GFX* gfx, bool wasScreenCleared)
{
	if (Visible)
	{
		#ifdef DEBUG_SHOW_CONTAINER_GRID_CELLS

			gfx->fillRect(LocX - 1, LocY - 1, Width + 2, Height + 2, UI_LIB_COLOR_BACKGROUND);

			uint16_t currentY = LocY;
			gfx->drawFastHLine(LocX, currentY, Width, UI_LIB_COLOR_FOREGROUND);
			for(int r = 0; r < MAX_CONTAINER_GRID_ROWS; r++)
			{
				if(_rowHeights[r] == 0) { break; }
				currentY += _rowHeights[r];
				gfx->drawFastHLine(LocX, currentY, Width, UI_LIB_COLOR_FOREGROUND);
			}

			uint16_t currentX = LocX;
			gfx->drawFastVLine(currentX, LocY, Height, UI_LIB_COLOR_FOREGROUND);
			for(int c = 0; c < MAX_CONTAINER_GRID_COLUMNS; c++)
			{
				if(_columnWidths[c] == 0) { break; }
				currentX += _columnWidths[c];
				gfx->drawFastVLine(currentX, LocY, Height, UI_LIB_COLOR_FOREGROUND);
			}
		#endif
		ContainerPage::Draw(gfx, wasScreenCleared);
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UI_LIB_COLOR_BACKGROUND);
	}
}

bool ContainerGrid::SetRowHeight(uint8_t rowIndex, uint16_t rowHeight)
{
	if(rowIndex >= MAX_CONTAINER_GRID_ROWS) { return false; }

	_rowHeights[rowIndex] = rowHeight;
	return true;
}

bool ContainerGrid::SetColumnWidth(uint8_t columnIndex, uint16_t columnWidth)
{
	if(columnIndex >= MAX_CONTAINER_GRID_COLUMNS) { return false; }

	_columnWidths[columnIndex] = columnWidth;
	return true;
}

bool ContainerGrid::AddItemToCell(UIElement* item, uint8_t columnIndex, uint8_t rowIndex, GridCellAlignment_t cellAlignment)
{
	if(columnIndex >= MAX_CONTAINER_GRID_COLUMNS || rowIndex >= MAX_CONTAINER_GRID_ROWS) { return false; }
	if(!AddItem(item)) { return false; }

	// adapt location accordingly

	uint16_t columnStart = LocX, columnEnd = 0;
	for(int i = 0; i <= columnIndex; i++)
	{
		if(_columnWidths[i] == 0) { break; }
		
		if(i >= 1)
		{
			columnStart += _columnWidths[i - 1];
		}
		columnEnd = columnStart + _columnWidths[i];
	}

	uint16_t rowStart = LocY, rowEnd = 0;
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