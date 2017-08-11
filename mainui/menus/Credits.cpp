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
#include "Bitmap.h"

static const char *uiCreditsDefault[] = 
{
	"CS16Client v1.4",
	"Build Date:" __DATE__ " " __TIME__ ,
	"",
	"Developers: ",
	"a1batross",
	"mittorn",
	"jeefo",
	"",
	"Touch & GFX: ",
	"SergioPoverony",
	"ahsim",
	"",
	"Beta-testers:",
	"1.kirill",
	"Romka_ZVO",
	"WolfReiser",
	"MakcuM56",
	"Mr.Lightning Bolt",
	"Kirpich",
	"MeL0maN",
	"LordAlfaruh",
	"Velaron",
	"KOBL1CK",
	"Rediska_Morkovka",
	"IcE",
	"CSPlayer",
	"Zu1iN~Mage",
	"lewa_j",
	"Cosmo",
	"Maks56873",
	"THE-Swank",
	"Namatrasnik",
	"picos",
	"BloodyLuxor",
	"AndroUser",
	"Bbltashit",
	"Athiend",
	"vlad[54rus]",
	"KinG",
	"erokhin",
	"Solexid",
	"",
	"Big thanks to Valve Corporation for Counter-Strike",
	"Uncle Mike for this powerful engine",
	"ONeiLL for inspiration",
	"Nagist and s1lentq for successful CS1.6 game researching",
	"Spirit of Half-Life developers for rain code",
	"hzqst for studio render code",
	"",
	"Copyright Flying With Gauss 2015-2017 (C)",
	"Flying With Gauss is not affiliated with Valve or any of their partners.",
	"All copyrights reserved to their respective owners.",
	"Thanks for playing!",
	NULL
};

class CMenuCredits : public CMenuFramework
{
public:
	CMenuCredits() : CMenuFramework("CMenuCredits") { }

	virtual void Draw();
	virtual const char *Key(int key, int down);

	friend void UI_DrawFinalCredits( void );
	friend void UI_FinalCredits( void );
	friend int UI_CreditsActive( void );

private:
	virtual void _Init();



	const char	**credits;
	int		startTime;
	int		showTime;
	int		fadeTime;
	int		numLines;
	int		active;
	int		finalCredits;
	char		*buffer;
};

CMenuCredits		uiCredits;


/*
=================
CMenuCredits::Draw
=================
*/
void CMenuCredits::Draw( void )
{
	int	i, y;
	float	speed;
	int	w = UI_MED_CHAR_WIDTH;
	int	h = UI_MED_CHAR_HEIGHT;
	int	color = 0;

	// draw the background first
	if( !uiCredits.finalCredits && !EngFuncs::GetCvarFloat( "cl_background" ) )
	{
		background.Draw();

		// otherwise running on cutscene
		speed = 32.0f * ( 768.0f / ScreenHeight );
	}
	else speed = 45.0f;	// syncronize with final background track :-)


	// now draw the credits
	UI_ScaleCoords( NULL, NULL, &w, &h );

	y = ScreenHeight - (((gpGlobals->time * 1000) - uiCredits.startTime ) / speed );

	// draw the credits
	for ( i = 0; i < uiCredits.numLines && uiCredits.credits[i]; i++, y += h )
	{
		// skip not visible lines, but always draw end line
		if( y <= -h && i != uiCredits.numLines - 1 ) continue;

		if(( y < ( ScreenHeight - h ) / 2 ) && i == uiCredits.numLines - 1 )
		{
			if( !uiCredits.fadeTime ) uiCredits.fadeTime = (gpGlobals->time * 1000);
			color = UI_FadeAlpha( uiCredits.fadeTime, uiCredits.showTime );
			if( UnpackAlpha( color ))
				UI_DrawString( 0, ( ScreenHeight - h ) / 2, ScreenWidth, h, uiCredits.credits[i], color, true, w, h, QM_CENTER, true );
		}
		else UI_DrawString( 0, y, ScreenWidth, h, uiCredits.credits[i], uiColorWhite, false, w, h, QM_CENTER, true );
	}

	if( y < 0 && UnpackAlpha( color ) == 0 )
	{
		uiCredits.active = false; // end of credits
		if( uiCredits.finalCredits )
			EngFuncs::HostEndGame( gMenu.m_gameinfo.title );
	}

	if( !uiCredits.active )
		Hide();
}

/*
=================
CMenuCredits::Key
=================
*/
const char *CMenuCredits::Key( int key, int down )
{
	if( !down ) return uiSoundNull;

	// final credits can't be intterupted
	if( uiCredits.finalCredits )
		return uiSoundNull;

	uiCredits.active = false;
	return uiSoundNull;
}

/*
=================
CMenuCredits::_Init
=================
*/
void CMenuCredits::_Init( void )
{
	// use built-in credits
	uiCredits.credits =  uiCreditsDefault;
	uiCredits.numLines = ( sizeof( uiCreditsDefault ) / sizeof( uiCreditsDefault[0] )) - 1; // skip term

	// run credits
	uiCredits.startTime = (gpGlobals->time * 1000) + 500; // make half-seconds delay
	uiCredits.showTime = bound( 1000, strlen( uiCredits.credits[uiCredits.numLines - 1]) * 1000, 10000 );
	uiCredits.fadeTime = 0; // will be determined later
	uiCredits.active = true;
}

void UI_DrawFinalCredits( void )
{
	if( uiCredits.finalCredits && uiCredits.active )
		uiCredits.Draw ();
}

int UI_CreditsActive( void )
{
	return uiCredits.active && uiCredits.finalCredits;
}

/*
=================
UI_Credits_Precache
=================
*/
void UI_Credits_Precache( void )
{
}

/*
=================
UI_Credits_Menu
=================
*/
void UI_Credits_Menu( void )
{
	UI_Credits_Precache();
	uiCredits.Show();
}

void UI_FinalCredits( void )
{
	uiCredits.finalCredits = true;
	uiCredits.Show();
}
