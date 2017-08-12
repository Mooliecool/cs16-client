#include "BaseClientMenu.h"

CMenBaseClientWindow::CMenuBaseClientWindow(const char *name) : CMenuBaseWindow(name)
{
}

void CMenuBaseClientWindow::Show()
{
	CMenuBaseWindow::Show();
	
	uiStatic.clientWindowShown++;
}

void CMenuBaseClientWindow::Hide()
{
	CMenuBaseWindow::Hide();
	
	uiStatic.clientWindowShown--;
}