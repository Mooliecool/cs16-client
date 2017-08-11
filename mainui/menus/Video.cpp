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
#include "Utils.h"
#include "BtnsBMPTable.h"
#include "Bitmap.h"
#include "PicButton.h"
#include "Slider.h"
#include "CheckBox.h"

#define ART_BANNER	  	"gfx/shell/head_vidoptions"
#define ART_GAMMA		"gfx/shell/gamma"

class CMenuVidOptions : public CMenuFramework
{
private:
	void _Init();
	void _VidInit();

public:
	CMenuVidOptions() : CMenuFramework( "CMenuVidOptions" ) { }
	void SaveAndPopMenu();
	void GetConfig();

	int		outlineWidth;

	class CMenuVidPreview : public CMenuBitmap
	{
		virtual void Draw();
	} testImage;

	CMenuPicButton	done;

	CMenuSlider	gammaIntensity;
	CMenuSlider	glareReduction;
	CMenuCheckBox	vsync;
	CMenuCheckBox	fastSky;
	CMenuCheckBox	hiTextures;

	HIMAGE		hTestImage;
} uiVidOptions;


/*
=================
CMenuVidOptions::GetConfig
=================
*/
void CMenuVidOptions::GetConfig( void )
{
	if( EngFuncs::GetCvarFloat( "gl_ignorehwgamma" ))
	{
		gammaIntensity.SetCurrentValue( RemapVal( EngFuncs::GetCvarFloat( "gamma" ), 1.8f, 7.0f, 0.0f, 1.0f ) );
		EngFuncs::ProcessImage(hTestImage, EngFuncs::GetCvarFloat( "gamma" ));
	}
	else gammaIntensity.SetCurrentValue( RemapVal( EngFuncs::GetCvarFloat( "gamma" ), 0.5f, 2.3f, 0.0f, 1.0f ) );
}

void CMenuVidOptions::SaveAndPopMenu( void )
{
	glareReduction.WriteCvar();
	fastSky.WriteCvar();
	hiTextures.WriteCvar();
	vsync.WriteCvar();
	// gamma is already written

	CMenuFramework::SaveAndPopMenu();
}

/*
=================
CMenuVidOptions::Ownerdraw
=================
*/
void CMenuVidOptions::CMenuVidPreview::Draw( )
{
	int		color = 0xFFFF0000; // 255, 0, 0, 255

	EngFuncs::PIC_Set( uiVidOptions.hTestImage, 255, 255, 255 );
	EngFuncs::PIC_Draw( m_scPos, m_scSize );

	UI_DrawRectangleExt( m_scPos, m_scSize, color, ((CMenuVidOptions*)Parent())->outlineWidth );
}

/*
=================
CMenuVidOptions::Init
=================
*/
void CMenuVidOptions::_Init( void )
{
	hTestImage = EngFuncs::PIC_Load( ART_GAMMA, PIC_KEEP_8BIT | PIC_KEEP_RGBDATA );

	banner.SetPicture(ART_BANNER);

	testImage.iFlags = QMF_INACTIVE;
	testImage.SetRect( 390, 225, 480, 450 );
	testImage.SetPicture( ART_GAMMA );

	gammaIntensity.SetNameAndStatus( "Gamma", "Set gamma value (0.5 - 2.3)" );
	gammaIntensity.SetCoord( 72, 250 );
	gammaIntensity.Setup( 0.0, 1.0, 0.05 );
	SET_EVENT( gammaIntensity, onChanged )
	{
		CMenuSlider *self = (CMenuSlider*)pSelf;

		if( EngFuncs::GetCvarFloat( "gl_ignorehwgamma" ))
		{
			EngFuncs::CvarSetValue( "gamma", RemapVal( self->GetCurrentValue(), 0.0f, 1.0f, 1.8f, 7.0f ));
			EngFuncs::ProcessImage( uiVidOptions.hTestImage, EngFuncs::GetCvarFloat( "gamma" ));
		}
		else EngFuncs::CvarSetValue( "gamma", RemapVal( self->GetCurrentValue(), 0.0f, 1.0f, 0.5f, 2.3f ));
	}
	END_EVENT( gammaIntensity, onChanged )

	glareReduction.SetNameAndStatus( "Glare reduction", "Set glare reduction level" );
	glareReduction.SetCoord( 72, 310 );
	glareReduction.Setup( 100, 300, 15 );
	glareReduction.LinkCvar( "r_flaresize" );

	vsync.SetNameAndStatus( "Vertical sync", "Enable vertical synchronization" );
	vsync.SetCoord( 72, 350 );
	vsync.LinkCvar( "gl_swapInterval" );

	fastSky.SetNameAndStatus( "Draw simple sky", "enable/disable fast sky rendering (for old computers)" );
	fastSky.SetCoord( 72, 400 );
	fastSky.LinkCvar( "r_fastsky" );

	hiTextures.SetNameAndStatus( "Allow materials", "let engine replace 8-bit textures with full color hi-res prototypes (if present)" );
	hiTextures.SetCoord( 72, 450 );
	hiTextures.LinkCvar( "host_allow_materials" );

	done.SetNameAndStatus( "Done", "Go back to the Video Menu" );
	done.SetCoord( 72, 680 );
	done.SetPicture( PC_DONE );
	done.onActivated = SaveAndPopMenuCb;

	GetConfig();

	AddItem( background );
	AddItem( banner );
	AddItem( done );
	AddItem( gammaIntensity );
	AddItem( glareReduction );
	AddItem( vsync );
	AddItem( fastSky );
	AddItem( hiTextures );
	AddItem( testImage );
}

void CMenuVidOptions::_VidInit()
{
	outlineWidth = 2;
	UI_ScaleCoords( NULL, NULL, &outlineWidth, NULL );
}

/*
=================
CMenuVidOptions::Precache
=================
*/
void UI_Video_Precache( void )
{
	EngFuncs::PIC_Load( ART_BANNER );
}

/*
=================
CMenuVidOptions::Menu
=================
*/
void UI_Video_Menu( void )
{
	UI_Video_Precache();
	uiVidOptions.Show();
}
