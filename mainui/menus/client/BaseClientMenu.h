#pragma once
#ifndef BASECLIENTMENU_H
#define BASECLIENTMENU_H

#include "BaseWindow.h"

class CMenuBaseClientWindow : public CMenuBaseWindow
{
public:
	CMenuBaseClientWindow( const char *name = "Unnamed Client Window" );

	virtual void Hide();
	virtual void Show();
	virtual bool IsVisible();
};

#endif // BASECLIENTMENU_H
