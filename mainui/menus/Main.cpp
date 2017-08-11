/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/


#include "extdll_menu.h"
#include "BaseMenu.h"
#include "Action.h"
#include "Bitmap.h"
#include "PicButton.h"
#include "YesNoMessageBox.h"
#include "Utils.h"
#include "keydefs.h"
#include "BtnsBMPTable.h"
#include "MenuStrings.h"

#define ART_MINIMIZE_N	"gfx/shell/min_n"
#define ART_MINIMIZE_F	"gfx/shell/min_f"
#define ART_MINIMIZE_D	"gfx/shell/min_d"
#define ART_CLOSEBTN_N	"gfx/shell/cls_n"
#define ART_CLOSEBTN_F	"gfx/shell/cls_f"
#define ART_CLOSEBTN_D	"gfx/shell/cls_d"

class CMenuMain: public CMenuFramework
{
public:
	CMenuMain() : CMenuFramework( "CMenuMain" ) { }

	virtual const char *Key( int key, int down );
	virtual const char *Activate( );

private:
	virtual void _Init();
	virtual void _VidInit( );

	void QuitDialog();
	void DisconnectDialog();
	DECLARE_EVENT_TO_MENU_METHOD( CMenuMain, QuitDialog )
	DECLARE_EVENT_TO_MENU_METHOD( CMenuMain, DisconnectDialog )
	static void DisconnectCb( CMenuBaseItem *pSelf, void *pExtra );
	static void QuitCb( CMenuBaseItem *, void * );

	CMenuPicButton	console;

	CMenuPicButton	resumeGame;
	CMenuPicButton	disconnect;
	CMenuPicButton  createGame;
	CMenuPicButton	configuration;
	CMenuPicButton	multiPlayer;
	CMenuPicButton	previews;
	CMenuPicButton	quit;

	// quit dialog
	CMenuYesNoMessageBox dialog;
};

static CMenuMain uiMain;

void CMenuMain::QuitDialog()
{
	dialog.Link( this );
	if( CL_IsActive() )
		dialog.SetMessage( MenuStrings[IDS_MAIN_QUITPROMPTINGAME] );
	else
		dialog.SetMessage( MenuStrings[IDS_MAIN_QUITPROMPT] );

	dialog.onPositive.SetCommand( FALSE, "quit\n" );
	dialog.Show();
}

void CMenuMain::DisconnectDialog()
{
	dialog.Link( this );
	dialog.SetMessage( "Really disconnect?" );
	dialog.onPositive = DisconnectCb;
	dialog.Show();
}



/*
=================
CMenuMain::Key
=================
*/
const char *CMenuMain::Key( int key, int down )
{
	if( down && key == K_ESCAPE )
	{
		if ( CL_IsActive( ))
		{
			if( !dialog.IsVisible() )
				UI_CloseMenu();
		}
		else
		{
			QuitDialog();
		}
		return uiSoundNull;
	}
	return CMenuFramework::Key( key, down );
}

/*
=================
UI_Main_ActivateFunc
=================
*/
const char *CMenuMain::Activate( void )
{
	if ( CL_IsActive( ))
	{
		resumeGame.Show();
		disconnect.Show();
	}
	else
	{
		resumeGame.Hide();
		disconnect.Hide();
	}

	if( gpGlobals->developer )
	{
		console.pos.y = CL_IsActive() ? 280 : 380;
	}

	CMenuPicButton::ClearButtonStack();

	return 0;
}

void CMenuMain::DisconnectCb( CMenuBaseItem *pSelf, void* )
{
	EngFuncs::ClientCmd( FALSE, "cmd disconnect;endgame disconnect;wait;wait;wait;menu_options;menu_main\n");
	((CMenuMain*)pSelf->Parent())->dialog.Hide( );
}

void CMenuMain::_Init( void )
{
	// console
	console.SetNameAndStatus( "Console", "Show console" );
	console.iFlags |= QMF_NOTIFY;
	console.SetPicture( PC_CONSOLE );
	SET_EVENT( console, onActivated )
	{
		UI_SetActiveMenu( FALSE );
		EngFuncs::KEY_SetDest( KEY_CONSOLE );
	}
	END_EVENT( console, onActivated )

	resumeGame.SetNameAndStatus( "Resume Game", MenuStrings[IDS_MAIN_RETURNHELP] );
	resumeGame.SetPicture( PC_RESUME_GAME );
	resumeGame.iFlags |= QMF_NOTIFY;
	resumeGame.onActivated = UI_CloseMenu;

	disconnect.SetNameAndStatus( "Disconnect", "Disconnect from server" );
	disconnect.SetPicture( PC_DISCONNECT );
	disconnect.iFlags |= QMF_NOTIFY;
	disconnect.onActivated = DisconnectDialogCb;

	createGame.SetNameAndStatus( "Create game", "Create server for Internet or local play" );
	createGame.SetPicture( PC_CREATE_GAME );
	createGame.iFlags |= QMF_NOTIFY;
	createGame.onActivated = UI_InternetGames_Menu;

	multiPlayer.SetNameAndStatus( "Multiplayer", MenuStrings[IDS_MAIN_MULTIPLAYERHELP] );
	multiPlayer.SetPicture( PC_MULTIPLAYER );
	multiPlayer.iFlags |= QMF_NOTIFY;
	multiPlayer.onActivated = UI_MultiPlayer_Menu;

	configuration.SetNameAndStatus( "Configuration", MenuStrings[IDS_MAIN_CONFIGUREHELP] );
	configuration.SetPicture( PC_CONFIG );
	configuration.iFlags |= QMF_NOTIFY;
	configuration.onActivated = UI_Options_Menu;

	previews.SetNameAndStatus( "View readme", "Read about CS16Client developers");
	previews.SetPicture( PC_VIEW_README );
	previews.iFlags |= QMF_NOTIFY;
	previews.onActivated = UI_Credits_Menu;

	quit.SetNameAndStatus( "Quit", MenuStrings[IDS_MAIN_QUITPROMPT] );
	quit.SetPicture( PC_QUIT );
	quit.iFlags |= QMF_NOTIFY;
	quit.onActivated = QuitDialogCb;

	AddItem( background );

	if ( gpGlobals->developer )
		AddItem( console );

	AddItem( disconnect );
	AddItem( resumeGame );
	AddItem( createGame );
	AddItem( configuration );
	AddItem( multiPlayer );
	AddItem( previews );
	AddItem( quit );
}

/*
=================
UI_Main_Init
=================
*/
void CMenuMain::_VidInit( void )
{
	Activate();

	console.pos.x = 72;
	resumeGame.SetCoord( 72, 330 );
	disconnect.SetCoord( 72, 380 );
	createGame.SetCoord( 72, 430 );
	configuration.SetCoord( 72, 480 );
	multiPlayer.SetCoord( 72, 530 );
	previews.SetCoord( 72, 580 );
	quit.SetCoord( 72, 630 );
}

/*
=================
UI_Main_Precache
=================
*/
void UI_Main_Precache( void )
{
}

/*
=================
UI_Main_Menu
=================
*/
void UI_Main_Menu( void )
{
	UI_Main_Precache();
	uiMain.Show();
}
