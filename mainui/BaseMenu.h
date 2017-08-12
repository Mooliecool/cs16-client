/*
basemenu.h - menu basic header 
Copyright (C) 2010 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#ifndef BASEMENU_H
#define BASEMENU_H
#include "netadr.h"
#include "keydefs.h"

#define BIT( x ) ( 1U << x )

// engine constants
enum
{
	GAME_NORMAL = 0,
	GAME_SINGLEPLAYER_ONLY,
	GAME_MULTIPLAYER_ONLY
};

enum
{
	KEY_CONSOLE = 0,
	KEY_GAME,
	KEY_MENU
};

#define CS_SIZE			64	// size of one config string
#define CS_TIME			16	// size of time string

// color strings
#define ColorIndex( c )		((( c ) - '0' ) & 7 )
#define IsColorString( p )		( p && *( p ) == '^' && *(( p ) + 1) && *(( p ) + 1) >= '0' && *(( p ) + 1 ) <= '9' )

#define UI_MAX_MENUDEPTH		64
#define UI_MAX_MENUITEMS		64

#define UI_PULSE_DIVISOR		75
#define UI_BLINK_TIME		250
#define UI_BLINK_MASK		499

#define UI_SMALL_CHAR_WIDTH		10
#define UI_SMALL_CHAR_HEIGHT	20

#define UI_MED_CHAR_WIDTH		18
#define UI_MED_CHAR_HEIGHT		26

#define UI_BIG_CHAR_WIDTH		20
#define UI_BIG_CHAR_HEIGHT		40

#define UI_OUTLINE_WIDTH		uiStatic.outlineWidth	// outline thickness

#define UI_MAXGAMES			900	// slots for savegame/demos
#define UI_MAX_SERVERS		256
#define UI_MAX_BGMAPS		32

#define MAX_HINT_TEXT		512


// menu buttons dims
#define UI_BUTTONS_WIDTH		240
#define UI_BUTTONS_HEIGHT		40
#define UI_BUTTON_CHARWIDTH		14	// empirically determined value

#define UI_LEFTARROW		"gfx/shell/larrowdefault"
#define UI_LEFTARROWFOCUS		"gfx/shell/larrowflyover"
#define UI_LEFTARROWPRESSED		"gfx/shell/larrowpressed"
#define UI_RIGHTARROW		"gfx/shell/rarrowdefault"
#define UI_RIGHTARROWFOCUS		"gfx/shell/rarrowflyover"
#define UI_RIGHTARROWPRESSED		"gfx/shell/rarrowpressed"
#define UI_UPARROW			"gfx/shell/uparrowd"
#define UI_UPARROWFOCUS		"gfx/shell/uparrowf"
#define UI_UPARROWPRESSED		"gfx/shell/uparrowp"
#define UI_DOWNARROW		"gfx/shell/dnarrowd"
#define UI_DOWNARROWFOCUS		"gfx/shell/dnarrowf"
#define UI_DOWNARROWPRESSED		"gfx/shell/dnarrowp"

struct Point
{
	Point() {}
	Point( int x, int y ) : x(x), y(y) {}

	int x, y;
	Point Scale();
	friend Point operator +( Point &a, Point &b ) { return Point( a.x + b.x, a.y + b.y ); }
	friend Point operator -( Point &a, Point &b ) { return Point( a.x - b.x, a.y - b.y ); }

	Point& operator+=( Point &a )
	{
		x += a.x;
		y += a.y;
		return *this;
	}

	Point& operator-=( Point &a )
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}

	Point operator *( float scale ) { return Point( x * scale, y * scale );	}
	Point operator /( float scale ) { return Point( x / scale, y / scale );	}
};

struct Size
{
	Size() {}
	Size( int w, int h ) : w(w), h(h) {}

	int w, h;
	Size Scale();
};

#include "extdll_menu.h"
#include "enginecallback.h"
#include "EventSystem.h"
#include "Framework.h"
#include "BaseItem.h"
#include "BaseWindow.h"

// =====================================================================
// Main menu interface

extern cvar_t	*ui_precache;
extern cvar_t	*ui_showmodels;
extern cvar_t   *ui_show_window_stack;

typedef struct
{
	CMenuBaseWindow *rootActive; // current active fullscreen holder(menu framework)
	CMenuBaseWindow *menuActive; // current active window
	CMenuBaseWindow *prevMenu;   // previous active window
	CMenuBaseWindow *menuStack[UI_MAX_MENUDEPTH];
	int      menuDepth;
	int      rootPosition;

	netadr_t serverAddresses[UI_MAX_SERVERS];
	char	serverNames[UI_MAX_SERVERS][256];
	float	serverPings[UI_MAX_SERVERS];
	int		serversRefreshTime;
	int		numServers;
	int		updateServers;	// true is receive new info about servers

	char	bgmaps[UI_MAX_BGMAPS][80];
	int		bgmapcount;

	HIMAGE	hFont;		// mainfont

	bool	m_fDemosPlayed;
	int		m_iOldMenuDepth;
	bool	m_fNoOldBackground;

	float	scaleX;
	float	scaleY;
	int		outlineWidth;

	int		cursorX;
	int		cursorY;
	int		realTime;
	int		firstDraw;
	float	enterSound;
	int		mouseInRect;
	int		hideCursor;
	int		visible;
	int		framecount;	// how many frames menu visible
	int		initialized;

	bool	precached;

	// btns_main.bmp stuff
	HIMAGE	buttonsPics[71];	// Fpos.xME: replace with PC_BUTTONCOUNT

	int		buttons_width;	// btns_main.bmp global width
	int		buttons_height;	// per one button with all states (inactive, focus, pressed)

	int		buttons_draw_width;	// scaled image what we drawing
	int		buttons_draw_height;
	int		width;
	bool		textInput;
	
	int clientMenuShown;
} uiStatic_t;

extern float	cursorDY;			// use for touch scroll
extern bool g_bCursorDown;
extern uiStatic_t		uiStatic;

#define DLG_X ((uiStatic.width - 640) / 2 - 192) // Dialogs are 640px in width

extern const char		*uiSoundIn;
extern const char		*uiSoundOut;
extern const char		*uiSoundKey;
extern const char		*uiSoundRemoveKey;
extern const char		*uiSoundLaunch;
extern const char		*uiSoundBuzz;
extern const char		*uiSoundGlow;
extern const char		*uiSoundMove;
extern const char		*uiSoundNull;

extern int	uiColorHelp;
extern int	uiPromptBgColor;
extern int	uiPromptTextColor;
extern int	uiPromptFocusColor;
extern int	uiInputTextColor;
extern int	uiInputBgColor;
extern int	uiInputFgColor;

extern int	uiColorWhite;
extern int	uiColorDkGrey;
extern int	uiColorBlack;

// TODO: Move it under namespace?

void UI_Precache( void );

void UI_ScaleCoords( int *x, int *y, int *w, int *h );
void UI_ScaleCoords( int &x, int &y, int &w, int &h );
void UI_ScaleCoords( int &x, int &y );

bool UI_CursorInRect( int x, int y, int w, int h );
void UI_DrawPic( int x, int y, int w, int h, const int color, const char *pic );
void UI_DrawPicAdditive( int x, int y, int w, int h, const int color, const char *pic );
void UI_DrawPicTrans( int x, int y, int width, int height, const int color, const char *pic );
void UI_DrawPicHoles( int x, int y, int width, int height, const int color, const char *pic );
void UI_FillRect( int x, int y, int w, int h, const int color );
void UI_DrawRectangleExt( int in_x, int in_y, int in_w, int in_h, const int color, int outlineWidth );
void UI_DrawString(int x, int y, int w, int h, const char *str, const int col, int forceCol, int charW, int charH, ETextAlignment justify, bool shadow, EVertAlignment vertAlign = QM_TOP);
inline void UI_DrawRectangle( int x, int y, int w, int h, const int color )
{
	UI_DrawRectangleExt( x, y, w, h, color, uiStatic.outlineWidth );
}

inline bool UI_CursorInRect( Point pos, Size size )
{
	return UI_CursorInRect( pos.x, pos.y, size.w, size.h );
}
inline void UI_DrawPic( Point pos, Size size, const int color, const char *pic )
{
	UI_DrawPic( pos.x, pos.y, size.w, size.h, color, pic );
}
inline void UI_DrawPicAdditive( Point pos, Size size, const int color, const char *pic )
{
	UI_DrawPicAdditive( pos.x, pos.y, size.w, size.h, color, pic );
}
inline void UI_DrawPicTrans( Point pos, Size size, const int color, const char *pic )
{
	UI_DrawPicTrans( pos.x, pos.y, size.w, size.h, color, pic );
}
inline void UI_DrawPicHoles( Point pos, Size size, const int color, const char *pic )
{
	UI_DrawPicHoles( pos.x, pos.y, size.w, size.h, color, pic );
}
inline void UI_FillRect( Point pos, Size size, const int color )
{
	UI_FillRect( pos.x, pos.y, size.w, size.h, color );
}
inline void UI_DrawRectangle( Point pos, Size size, const int color )
{
	UI_DrawRectangleExt( pos.x, pos.y, size.w, size.h, color, uiStatic.outlineWidth );
}
inline void UI_DrawRectangleExt( Point pos, Size size, const int color, int outlineWidth )
{
	UI_DrawRectangleExt( pos.x, pos.y, size.w, size.h, color, outlineWidth );
}
inline void UI_DrawString( Point pos, Size size, const char *str, const int col, int forceCol, Size chSize, ETextAlignment justify, bool shadow, EVertAlignment verticalAlignment = QM_TOP  )
{
	UI_DrawString( pos.x, pos.y, size.w, size.h, str, col, forceCol, chSize.w, chSize.h, justify, shadow, verticalAlignment );
}

void UI_StartSound( const char *sound );
void UI_LoadBmpButtons( void );

void UI_RefreshInternetServerList( void );
void UI_RefreshServerList( void );
int UI_CreditsActive( void );
void UI_DrawFinalCredits( void );

void UI_CloseMenu( void );

// SCR support
void UI_LoadScriptConfig( void );

// Precache
void UI_Main_Precache( void );
void UI_MultiPlayer_Precache( void );
void UI_Options_Precache( void );
void UI_InternetGames_Precache( void );
void UI_LanGame_Precache( void );
void UI_PlayerSetup_Precache( void );
void UI_Controls_Precache( void );
void UI_GameOptions_Precache( void );
void UI_CreateGame_Precache( void );
void UI_Audio_Precache( void );
void UI_Video_Precache( void );
void UI_Credits_Precache( void );
void UI_Touch_Precache( void );
void UI_TouchOptions_Precache( void );
void UI_TouchButtons_Precache( void );
void UI_TouchEdit_Precache( void );
void UI_FileDialog_Precache( void );
void UI_GamePad_Precache( void );
void UI_DynamicMenu_Precache( void );

// Menus
void UI_Main_Menu( void );
void UI_MultiPlayer_Menu( void );
void UI_Options_Menu( void );
void UI_InternetGames_Menu( void );
void UI_LanGame_Menu( void );
void UI_ServerBrowser_Menu( void );
void UI_PlayerSetup_Menu( void );
void UI_Controls_Menu( void );
void UI_GameOptions_Menu( void );
void UI_CreateGame_Menu( void );
void UI_Audio_Menu( void );
void UI_Video_Menu( void );
void UI_Credits_Menu( void );
void UI_Touch_Menu( void );
void UI_TouchOptions_Menu( void );
void UI_TouchButtons_Menu( void );
void UI_TouchEdit_Menu( void );
void UI_FileDialog_Menu( void );
void UI_TouchButtons_AddButtonToList( const char *name, const char *texture, const char *command, unsigned char *color, int flags );
void UI_TouchButtons_GetButtonList();
void UI_GamePad_Menu( void );

bool UI_AdvUserOptions_IsAvailable( void );
void UI_AdvUserOptions_Menu( void );
bool UI_AdvServerOptions_IsAvailable( void );
void UI_AdvServerOptions_Menu( void );

//
//-----------------------------------------------------
//
class CMenu
{
public:
	// Game information
	GAMEINFO		m_gameinfo;
};

typedef struct
{
	char patterns[32][256];
	int npatterns;
	char result[256];
	bool valid;
	void ( * callback )( bool success );
	bool preview;
} uiFileDialogGlobal_t;

extern uiFileDialogGlobal_t uiFileDialogGlobal;

extern CMenu gMenu;

#endif // BASEMENU_H
