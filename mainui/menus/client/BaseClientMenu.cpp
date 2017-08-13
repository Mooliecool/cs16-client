#include "BaseClientMenu.h"

CMenuBaseClientWindow::CMenuBaseClientWindow(const char *name) : CMenuBaseWindow(name)
{
}

void CMenuBaseClientWindow::Show()
{
	CMenuBaseWindow::Show();
	
	uiStatic.clientMenuShown++;
}

void CMenuBaseClientWindow::Hide()
{
	CMenuBaseWindow::Hide();
	
	uiStatic.clientMenuShown--;
}
