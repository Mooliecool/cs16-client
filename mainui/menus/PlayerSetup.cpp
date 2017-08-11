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

#include "mathlib.h"
#include "extdll_menu.h"
#include "const.h"
#include "BaseMenu.h"
#include "Utils.h"
#include "keydefs.h"
#include "ref_params.h"
#include "cl_entity.h"
#include "com_model.h"
#include "entity_types.h"
#include "BtnsBMPTable.h"
#include "Bitmap.h"
#include "PicButton.h"
#include "CheckBox.h"
#include "Slider.h"
#include "Field.h"
#include "SpinControl.h"

#define ART_BANNER		"gfx/shell/head_customize"
#define ART_CROSSHAIR   "gfx/vgui/crosshair"

class CMenuCrosshairView : public CMenuBitmap
{
public:
	void Draw();
};

class CMenuPlayerSetup : public CMenuFramework
{
private:
	void _Init();
	void _VidInit();
public:
	CMenuPlayerSetup() : CMenuFramework( "CMenuPlayerSetup" ) { }

	void GetConfig();
	void SetConfig();
	void SaveAndPopMenu();

	CMenuPicButton	done;
	CMenuPicButton	gameOptions;
	CMenuPicButton	advOptions;

	CMenuCheckBox	hiModels;

	CMenuField	name;

	CMenuCrosshairView	  crosshairView;
	CMenuSlider	crosshairRed;
	CMenuSlider	crosshairGreen;
	CMenuSlider	crosshairBlue;
	CMenuSpinControl crosshairSize;
	CMenuSpinControl crosshairColor;

	CMenuCheckBox    crosshairTranslucent;

	HIMAGE uiWhite;

} uiPlayerSetup;

static const char *g_szCrosshairAvailSizes[4] = { "auto", "small", "medium", "large" };

void CMenuCrosshairView::Draw()
{
	HIMAGE uiWhite;

	CMenuBitmap::Draw();

	uiWhite = EngFuncs::PIC_Load( "*white" );

	int l;
	int val = uiPlayerSetup.crosshairSize.GetCurrentValue();
	switch( val )
	{
	case 1:
		l = 10;
		break;
	case 2:
		l = 20;
		break;
	case 3:
		l = 30;
		break;
	case 0:
		if( ScreenWidth < 640 )
			l = 30;
		else if( ScreenWidth < 1024 )
			l = 20;
		else l = 10;
	}

	l *= ScreenHeight / 768.0f;

	int x = m_scPos.x, // xpos
		y = m_scPos.y, // ypos
		w = m_scSize.w, // width
		h = m_scSize.h, // height
		// delta distance
		d = (m_scSize.w / 2 - l) * 0.5,
		// alpha
		a = 180,
		// red
		r = uiPlayerSetup.crosshairRed.GetCurrentValue(),
		// green
		g = uiPlayerSetup.crosshairGreen.GetCurrentValue(),
		// blue
		b = uiPlayerSetup.crosshairBlue.GetCurrentValue();

	if( uiPlayerSetup.crosshairTranslucent.bChecked )
	{
		// verical
		EngFuncs::PIC_Set(uiWhite, r, g, b, a);
		EngFuncs::PIC_DrawTrans(x + w / 2, y + d,         1, l );

		EngFuncs::PIC_Set(uiWhite, r, g, b, a);
		EngFuncs::PIC_DrawTrans(x + w / 2, y + h / 2 + d, 1, l );

		// horizontal
		EngFuncs::PIC_Set(uiWhite, r, g, b, a);
		EngFuncs::PIC_DrawTrans(x + d,         y + h / 2, l, 1 );

		EngFuncs::PIC_Set(uiWhite, r, g, b, a);
		EngFuncs::PIC_DrawTrans(x + w / 2 + d, y + h / 2, l, 1 );
	}
	else
	{
		// verical
		EngFuncs::PIC_Set(uiWhite, r, g, b, a);
		EngFuncs::PIC_DrawAdditive(x + w / 2, y + d,         1, l );

		EngFuncs::PIC_Set(uiWhite, r, g, b, a);
		EngFuncs::PIC_DrawAdditive(x + w / 2, y + h / 2 + d, 1, l );

		// horizontal
		EngFuncs::PIC_Set(uiWhite, r, g, b, a);
		EngFuncs::PIC_DrawAdditive(x + d,         y + h / 2, l, 1 );

		EngFuncs::PIC_Set(uiWhite, r, g, b, a);
		EngFuncs::PIC_DrawAdditive(x + w / 2 + d, y + h / 2, l, 1 );
	}

}

void CMenuPlayerSetup::GetConfig( void )
{
	const char *colors = EngFuncs::GetCvarString( "cl_crosshair_color" );
	if( colors )
	{
		int r, g, b;
		sscanf( colors, "%d %d %d", &r, &g, &b );

		crosshairRed.SetCurrentValue( r );
		crosshairGreen.SetCurrentValue( g );
		crosshairBlue.SetCurrentValue( b );
	}

	crosshairTranslucent.UpdateEditable();
}

/*
=================
UI_PlayerSetup_SetConfig
=================
*/
void CMenuPlayerSetup::SetConfig( void )
{
	name.WriteCvar();
	hiModels.WriteCvar();

	char color[CS_SIZE];
	int r = uiPlayerSetup.crosshairRed.GetCurrentValue(), // red
		// green
		g = uiPlayerSetup.crosshairGreen.GetCurrentValue(),
		// blue
		b = uiPlayerSetup.crosshairBlue.GetCurrentValue();
	snprintf( color, CS_SIZE, "%d %d %d", r, g, b );

	EngFuncs::CvarSetString( "cl_crosshair_color", color );
}

void CMenuPlayerSetup::SaveAndPopMenu( void )
{
	SetConfig();
	CMenuFramework::SaveAndPopMenu();
}

/*
=================
UI_PlayerSetup_Init
=================
*/
void CMenuPlayerSetup::_Init( void )
{
	g_szCrosshairAvailSizes[0] = "auto";
	g_szCrosshairAvailSizes[1] = "small";
	g_szCrosshairAvailSizes[2] = "medium";
	g_szCrosshairAvailSizes[3] = "large";

	banner.SetPicture(ART_BANNER);

	done.SetNameAndStatus( "Done", "Go back to the Multiplayer Menu" );
	done.SetPicture( PC_DONE );\
	done.onActivated = SaveAndPopMenuCb;

	gameOptions.SetNameAndStatus( "Game options", "Configure handness, fov and other advanced options" );
	gameOptions.SetPicture( PC_GAME_OPTIONS );
	SET_EVENT( gameOptions, onActivated )
	{
		((CMenuPlayerSetup*)pSelf->Parent())->SetConfig();
		UI_GameOptions_Menu();
	}
	END_EVENT( gameOptions, onActivated )

	advOptions.SetNameAndStatus( "Adv options", "" );
	advOptions.SetPicture( PC_ADV_OPT );
	advOptions.onActivated = UI_AdvUserOptions_Menu;

	name.szStatusText = "Enter your multiplayer display name";
	name.iMaxLength = 32;
	name.LinkCvar( "name" );

	hiModels.SetNameAndStatus( "High quality models", "Show HD models in multiplayer" );
	hiModels.LinkCvar( "cl_himodels" );
	hiModels.onCvarChange = CMenuEditable::WriteCvarCb;

	crosshairView.SetPicture( ART_CROSSHAIR );
	crosshairView.SetRect( 320, 310, 96, 96 );
	crosshairView.iFlags = QMF_INACTIVE;

	crosshairSize.SetRect( 450, 315, 256, 26 );
	crosshairSize.Setup( g_szCrosshairAvailSizes, 4 );
	crosshairSize.eTextAlignment = QM_CENTER;
	crosshairSize.eFocusAnimation = QM_HIGHLIGHTIFFOCUS;
	crosshairSize.iFlags = QMF_DROPSHADOW;
	crosshairSize.szStatusText = "Set crosshair size";
	crosshairSize.onChanged = CMenuEditable::WriteCvarCb;
	crosshairSize.LinkCvar( "cl_crosshair_size", CMenuEditable::CVAR_STRING );

	crosshairColor.SetRect( 450, 360, 256, 26 );
	crosshairColor.Setup( 0.0f, 9.0f, 1.0f );
	crosshairColor.eTextAlignment = QM_CENTER;
	crosshairColor.eFocusAnimation = QM_HIGHLIGHTIFFOCUS;
	crosshairColor.iFlags = QMF_DROPSHADOW;
	crosshairColor.szStatusText = "Set crosshair color";
	SET_EVENT( crosshairColor, onChanged )
	{
		char cmd[CS_SIZE];
		snprintf( cmd, CS_SIZE, "adjust_crosshair %d\n", (int)((CMenuSpinControl*)pSelf)->GetCurrentValue() );
		EngFuncs::ClientCmd( TRUE, cmd );
		uiPlayerSetup.GetConfig();
	}
	END_EVENT( crosshairColor, onChanged )

	crosshairRed.eFocusAnimation = QM_PULSEIFFOCUS;
	crosshairRed.SetNameAndStatus( "Red:", "Texture red channel" );
	crosshairRed.Setup( 0, 255, 1 );
	crosshairRed.SetCoord( 320, 450 );

	crosshairGreen.eFocusAnimation = QM_PULSEIFFOCUS;
	crosshairGreen.SetNameAndStatus( "Green:", "Texture green channel" );
	crosshairGreen.Setup( 0, 255, 1 );
	crosshairGreen.SetCoord( 320, 500 );

	crosshairBlue.eFocusAnimation = QM_PULSEIFFOCUS;
	crosshairBlue.SetNameAndStatus( "Blue:", "Texture blue channel" );
	crosshairBlue.Setup( 0, 255, 1 );
	crosshairBlue.SetCoord( 320, 550 );

	crosshairTranslucent.SetNameAndStatus( "Translucent crosshair", "Set additive render crosshair" );
	crosshairTranslucent.SetCoord( 450, 400 );
	crosshairTranslucent.LinkCvar( "cl_crosshair_translucent" );
	crosshairTranslucent.iFlags = QMF_ACT_ONRELEASE | QMF_DROPSHADOW;
	crosshairTranslucent.onChanged = CMenuEditable::WriteCvarCb;

	AddItem( background );
	AddItem( banner );
	AddItem( done );
	AddItem( gameOptions );
	AddItem( advOptions );
	AddItem( name );
	AddItem( crosshairView );
	AddItem( crosshairSize );
	AddItem( crosshairColor );
	AddItem( crosshairRed );
	AddItem( crosshairGreen );
	AddItem( crosshairBlue );
	AddItem( crosshairTranslucent );
}

void CMenuPlayerSetup::_VidInit()
{
	done.SetCoord( 72, 230 );
	gameOptions.SetCoord( 72, 280 );
	advOptions.SetCoord( 72, 330 );
	name.SetRect( 320, 260, 256, 36 );
	hiModels.SetCoord( 340, 430 );

	advOptions.SetGrayed( !UI_AdvUserOptions_IsAvailable() );
}

/*
=================
UI_PlayerSetup_Precache
=================
*/
void UI_PlayerSetup_Precache( void )
{
	EngFuncs::PIC_Load( ART_BANNER );
}

/*
=================
UI_PlayerSetup_Menu
=================
*/
void UI_PlayerSetup_Menu( void )
{
	if ( gMenu.m_gameinfo.gamemode == GAME_SINGLEPLAYER_ONLY )
		return;

	UI_PlayerSetup_Precache();
	uiPlayerSetup.Show();
}
