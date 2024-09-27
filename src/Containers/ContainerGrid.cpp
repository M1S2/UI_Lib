/*
 * ContainerGrid.cpp
 */ 

#include "Containers/ContainerGrid.h"
#include "Core/UI_Manager.h"

ContainerGrid::ContainerGrid()
{
	Type = UI_CONTAINER;
}

ContainerGrid::ContainerGrid(uint16_t locX, uint16_t locY, uint16_t width, uint16_t height)
{
	Type = UI_CONTAINER;
	LocX = locX;
	LocY = locY;
	Width = width;
	Height = height;
}

void ContainerGrid::Draw(Adafruit_GFX* gfx)
{
	if (Visible)
	{
		#ifdef DEBUG_SHOW_CONTAINER_GRID_CELLS

			gfx->fillRect(LocX - 1, LocY - 1, Width + 2, Height + 2, UiManager.ColorBackground);

			uint16_t currentY = LocY;
			gfx->drawFastHLine(LocX, currentY, Width, UiManager.ColorForeground);
			for(int r = 0; r < MAX_CONTAINER_GRID_ROWS; r++)
			{
				if(_rowHeights[r] == 0) { break; }
				currentY += _rowHeights[r];
				gfx->drawFastHLine(LocX, currentY, Width, UiManager.ColorForeground);
			}

			uint16_t currentX = LocX;
			gfx->drawFastVLine(currentX, LocY, Height, UiManager.ColorForeground);
			for(int c = 0; c < MAX_CONTAINER_GRID_COLUMNS; c++)
			{
				if(_columnWidths[c] == 0) { break; }
				currentX += _columnWidths[c];
				gfx->drawFastVLine(currentX, LocY, Height, UiManager.ColorForeground);
			}
		#endif
		ContainerPage::Draw(gfx);
	}
	else
	{
		gfx->fillRect(LocX, LocY, Width, Height, UiManager.ColorBackground);
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

bool ContainerGrid::AddItem(UIElement* item, uint8_t columnIndex, uint8_t rowIndex, GridCellAlignment_t cellAlignment)
{
	if(columnIndex >= MAX_CONTAINER_GRID_COLUMNS || rowIndex >= MAX_CONTAINER_GRID_ROWS) { return false; }
	if(!Container::AddItem(item)) { return false; }

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

void ContainerGrid::RecalculateDimensions()
{
	Width = 0;
	for(int i = 0; i <= MAX_CONTAINER_GRID_COLUMNS; i++)
	{
		if(_columnWidths[i] == 0) { break; }
		Width += _columnWidths[i];
	}

	Height = 0;
	for(int i = 0; i <= MAX_CONTAINER_GRID_ROWS; i++)
	{
		if(_rowHeights[i] == 0) { break; }
		Height += _rowHeights[i];
	}
}