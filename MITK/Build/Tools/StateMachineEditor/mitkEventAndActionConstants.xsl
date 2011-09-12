<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="text" indent="yes"/>
<xsl:template match="/">
<xsl:text disable-output-escaping = "yes">/*=========================================================================

Program:   Medical Imaging &amp; Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef MITKINTERACTCONST_H
#define MITKINTERACTCONST_H

//##Documentation
//## @file mitkInteractionConst.h
//## @brief Constants for most interaction classes, due to the generic StateMachines.
//##
//## Changes in Type, ButtonState or Key has to be don in mitkEventMapper.cpp, too.
//## @ingroup Interaction

/*Prefixes for Constants:
E = Enumeration
EID = EventId's
Op = Operations
Ac = Action
Type_ = Type of Event
BS_ = ButtonStates and Buttons
Key_ = Keys like in QT
*/
namespace mitk{

/*
//UndoMechanism:
//constants for UndoMechanism;
*/
//different UndoModels:
typedef enum EUndoModels{LIMITEDLINEARUNDO=10, VERBOSE_LIMITEDLINEARUNDO=11, TREEUNDO=20};

//Constants for EventIds; use the according constant to through an event in the code
typedef enum EEventIds
{
</xsl:text>

<xsl:for-each select="//event">
<xsl:text>  </xsl:text>
<xsl:value-of select="@NAME"/>
<xsl:text> = </xsl:text>
<xsl:value-of select="@ID"/>
<xsl:if test="position()!=last()">      
<xsl:text>,</xsl:text>
</xsl:if>
<xsl:if test="boolean(./comment())">
<xsl:text>  // </xsl:text>
<xsl:value-of select="./comment()"/>
</xsl:if>
<xsl:text disable-output-escaping = "yes">
</xsl:text>
</xsl:for-each>
<xsl:text disable-output-escaping = "yes">

};

//##Constants for Operations
//## xomments are always examples of the usage
typedef enum EOperations
{
  OpNOTHING = 0,
  OpTEST = 1,
  OpNEWCELL = 10, //add a new cell
  OpADD = 100, //add a point or a vessel
  OpUNDOADD = 101,
  OpADDLINE = 1001, //add a line
  OpINSERT = 200, //insert a point at position
  OpINSERTLINE = 201, //insert a line at position
  OpINSERTPOINT = 202,
  OpCLOSECELL = 250, //close a cell (to a polygon)
  OpOPENCELL = 251, //close a cell (to a polygon)
  OpMOVE = 300, //move a point
  OpMOVELINE = 301, //move a line
  OpMOVECELL = 302, //move a line
  OpUNDOMOVE = 303,
  OpREMOVE = 400, //remove a point at position
  OpREMOVELINE = 401, //remove a line at position
  OpREMOVECELL = 402, //remove a cell
  OpREMOVEPOINT = 403,
  OpDELETE = 500, //delete
  OpDELETELINE = 501, //delete the last line in a cell
  OpUNDELETE = 502,
  OpDELETECELL = 505,
  OpSTATECHANGE = 600, //change a state
  OpTERMINATE = 666, //change a state
  OpSELECTPOINT = 700,
  OpSELECTLINE = 701,
  OpSELECTCELL = 702,
  OpSELECTSUBOBJECT = 703, //for VesselGraphInteractor
  //OpSELECTNEWSUBOBJECT = 704, //for VesselGraphInteractor
  OpSELECT = 705, 
  OpDESELECTPOINT = 800,
  OpDESELECTLINE = 801,
  OpDESELECTCELL = 802,
  OpDESELECTSUBOBJECT = 803,  //for VesselGraphInteractor
  OpDESELECTALL = 804,     //for VesselGraphInteractor
  OpDESELECT = 805,    
  OpNAVIGATE = 900,       
  OpZOOM = 1000,
  OpSCALE = 1100,
  OpROTATE = 1200,
  OpSETPOINTTYPE = 1210,
  OpMODECHANGE = 1500,
  OpSENDCOORDINATES = 1600,
  OpPERIPHERYSEARCH = 2000,  //used in VesselGraphInteractor
  OpROOTSEARCH = 2001,        //used in VesselGraphInteractor
  OpTHICKSTVESSELSEARCH = 2002,       //used in VesselGraphInteractor
  OpSHORTESTPATHSEARCH = 2003,       //used in VesselGraphInteractor
  OpATTRIBUTATION = 2004,       //used in VesselGraphInteractor
  OpDEFAULT = 2006,                  //used in VesselGraphInteractor
};

//##Constants for EventMapping...
//##connects the statemachine.xml-File with the implemented conditions.
//##within one statemachine the choice of the actionconstants is freely
//##
//## ActionId
typedef enum EActions
{
</xsl:text>


<xsl:for-each select="//action">	
<xsl:text>  </xsl:text>
<xsl:value-of select="@NAME"/>
<xsl:text> = </xsl:text>
<xsl:value-of select="@ID"/>
<xsl:if test="position()!=last()">      
<xsl:text>,</xsl:text>
</xsl:if>
<xsl:if test="boolean(./comment())">
<xsl:text>  // </xsl:text>
<xsl:value-of select="./comment()"/>
</xsl:if>
<xsl:text disable-output-escaping = "yes">
</xsl:text>
</xsl:for-each>
<xsl:text disable-output-escaping = "yes">

};


/*
//!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!
//EventMechanism:
//If you change anything from here on, then change in mitkEventMapper.cpp (Array of constants) as well.
//!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!
*/

//Type of an Event;
typedef enum EEventType
{
  Type_None = 0,				// invalid event
  Type_Timer = 1,				// timer event
  Type_MouseButtonPress = 2,	// mouse button pressed
  Type_MouseButtonRelease = 3,	// mouse button released
  Type_MouseButtonDblClick = 4,	// mouse button double click
  Type_MouseMove = 5,			// mouse move
  Type_KeyPress = 6,			// key pressed
  Type_KeyRelease = 7,			// key released
  Type_FocusIn = 8,				// keyboard focus received
  Type_FocusOut = 9,			// keyboard focus lost
  Type_Enter = 10,				// mouse enters widget
  Type_Leave = 11,				// mouse leaves widget
  Type_Paint = 12,				// paint widget
  Type_Move = 13,				// move widget
  Type_Resize = 14,				// resize widget
  Type_Create = 15,				// after object creation
  Type_Destroy = 16,			// during object destruction
  Type_Show = 17,   			// widget is shown
  Type_Hide = 18,				// widget is hidden
  Type_Close = 19,				// request to close widget
  Type_Quit = 20,				// request to quit application
  Type_Reparent = 21,			// widget has been reparented
  Type_ShowMinimized = 22,	    // widget is shown minimized
  Type_ShowNormal = 23,    		// widget is shown normal
  Type_WindowActivate = 24,		// window was activated
  Type_WindowDeactivate = 25,   // window was deactivated
  Type_ShowToParent = 26,  		// widget is shown to parent
  Type_HideToParent = 27,  		// widget is hidden to parent
  Type_ShowMaximized = 28,    	// widget is shown maximized
  Type_ShowFullScreen = 29, 	// widget is shown full-screen
  Type_Accel = 30,				// accelerator event
  Type_Wheel = 31,				// wheel event
  Type_AccelAvailable = 32,	    // accelerator available event
  Type_CaptionChange = 33,		// caption changed
  Type_IconChange = 34,			// icon changed
  Type_ParentFontChange = 35,	// parent font changed
  Type_ApplicationFontChange = 36,	// application font changed
  Type_ParentPaletteChange = 37,		// parent palette changed
  Type_ApplicationPaletteChange = 38,		// application palette changed
  Type_PaletteChange = 39,		// widget palette changed
  Type_Clipboard = 40,				// internal clipboard event
  Type_Speech = 42,				// reserved for speech input
  Type_SockAct = 50,				// socket activation
  Type_AccelOverride = 51,			// accelerator override event
  Type_DeferredDelete = 52,			// deferred delete event
  Type_DragEnter = 60,				// drag moves into widget
  Type_DragMove = 61,				// drag moves in widget
  Type_DragLeave = 62,				// drag leaves or is cancelled
  Type_Drop = 63,				// actual drop
  Type_DragResponse = 64,			// drag accepted/rejected
  Type_ChildInserted = 70,			// new child widget
  Type_ChildRemoved = 71,			// deleted child widget
  Type_LayoutHint = 72,			// child min/max size changed
  Type_ShowWindowRequest = 73,			// widget's window should be mapped
  Type_ActivateControl = 80,			// ActiveX activation
  Type_DeactivateControl = 81,			// ActiveX deactivation
  Type_ContextMenu = 82,			// context popup menu
  Type_IMStart = 83,				// input method composition start
  Type_IMCompose = 84,				// input method composition
  Type_IMEnd = 85,				// input method composition end
  Type_Accessibility = 86,			// accessibility information is requested
  Type_TabletMove = 87,			// Wacom tablet event
  Type_LocaleChange = 88,			// the system locale changed
  Type_LanguageChange = 89,			// the application language changed
  Type_LayoutDirectionChange = 90,		// the layout direction changed
  Type_Style = 91,				// internal style event
  Type_TabletPress = 92,			// tablet press
  Type_TabletRelease = 93,			// tablet release
  Type_User = 1000,				// first user event id
  Type_MaxUser = 65535
};


//##ButtonState
// mouse/keyboard state values
//QT combinations if MOUSEBUTTONRelease: left MouseButton + ControlButton: 0x201
typedef enum EButtonStates
{
  BS_NoButton        = 0x0000,
  BS_LeftButton      = 0x0001,
  BS_RightButton     = 0x0002,
  BS_MidButton       = 0x0004,
  BS_MouseButtonMask = 0x0007,
  BS_ShiftButton     = 0x0100,
  BS_ControlButton   = 0x0200,
  BS_AltButton       = 0x0400,
  BS_MetaButton		 = 0x0800,
  BS_KeyButtonMask   = 0x0f00,
  BS_Keypad          = 0x4000
};
//##Key
typedef enum EKeys
{
  Key_Escape = 0x1000,           // misc keys
  Key_Tab = 0x1001,
  Key_Backtab = 0x1002,
  Key_BackTab = 0x1002, //= Key_Backtab
  Key_Backspace = 0x1003,
  Key_BackSpace = 0x1003, //= Key_Backspace
  Key_Return = 0x1004,
  Key_Enter = 0x1005,
  Key_Insert = 0x1006,
  Key_Delete = 0x1007,
  Key_Pause = 0x1008,
  Key_Print = 0x1009,
  Key_SysReq = 0x100a,
  Key_Home = 0x1010,   // cursor movement
  Key_End = 0x1011,
  Key_Left = 0x1012,
  Key_Up = 0x1013,
  Key_Right = 0x1014,
  Key_Down = 0x1015,
  Key_Prior = 0x1016,
  Key_PageUp = 0x1016, //=Key_Prior
  Key_Next = 0x1017,
  Key_PageDown = 0x1017, //=Key_Next
  Key_Shift = 0x1020,     // modifiers
  Key_Control = 0x1021,
  Key_Meta = 0x1022,
  Key_Alt = 0x1023,
  Key_CapsLock = 0x1024,
  Key_NumLock = 0x1025,
  Key_ScrollLock = 0x1026,
  Key_F1 = 0x1030,                // function keys
  Key_F2 = 0x1031,
  Key_F3 = 0x1032,
  Key_F4 = 0x1033,
  Key_F5 = 0x1034,
  Key_F6 = 0x1035,
  Key_F7 = 0x1036,
  Key_F8 = 0x1037,
  Key_F9 = 0x1038,
  Key_F10 = 0x1039,
  Key_F11 = 0x103a,
  Key_F12 = 0x103b,
  Key_F13 = 0x103c,
  Key_F14 = 0x103d,
  Key_F15 = 0x103e,
  Key_F16 = 0x103f,
  Key_F17 = 0x1040,
  Key_F18 = 0x1041,
  Key_F19 = 0x1042,
  Key_F20 = 0x1043,
  Key_F21 = 0x1044,
  Key_F22 = 0x1045,
  Key_F23 = 0x1046,
  Key_F24 = 0x1047,
  Key_F25 = 0x1048,              // F25 .. F35 only on X11
  Key_F26 = 0x1049,
  Key_F27 = 0x104a,
  Key_F28 = 0x104b,
  Key_F29 = 0x104c,
  Key_F30 = 0x104d,
  Key_F31 = 0x104e,
  Key_F32 = 0x104f,
  Key_F33 = 0x1050,
  Key_F34 = 0x1051,
  Key_F35 = 0x1052,
  Key_Super_L = 0x1053,           // extra keys
  Key_Super_R = 0x1054,
  Key_Menu = 0x1055,
  Key_Hyper_L = 0x1056,
  Key_Hyper_R = 0x1057,
  Key_Help = 0x1058,
        // International input method support (X keycode - = 0xEE00)
        // Only interesting if you are writing your own input method
  Key_Muhenkan  = 0x1122,  // Cancel Conversion
  Key_Henkan  = 0x1123,  // Start/Stop Conversion
  Key_Hiragana_Katakana = 0x1127,  // Hiragana/Katakana toggle
  Key_Zenkaku_Hankaku = 0x112A,  // Zenkaku/Hankaku toggle
  Key_Space = 0x20,               // 7 bit printable ASCII
  Key_Any  = 0x20, //= Key_Space
  Key_Exclam = 0x21,
  Key_QuoteDbl = 0x22,
  Key_NumberSign = 0x23,
  Key_Dollar = 0x24,
  Key_Percent = 0x25,
  Key_Ampersand = 0x26,
  Key_Apostrophe = 0x27,
  Key_ParenLeft = 0x28,
  Key_ParenRight = 0x29,
  Key_Asterisk = 0x2a,
  Key_Plus = 0x2b,
  Key_Comma = 0x2c,
  Key_Minus = 0x2d,
  Key_Period = 0x2e,
  Key_Slash = 0x2f,
  Key_0 = 0x30,
  Key_1 = 0x31,
  Key_2 = 0x32,
  Key_3 = 0x33,
  Key_4 = 0x34,
  Key_5 = 0x35,
  Key_6 = 0x36,
  Key_7 = 0x37,
  Key_8 = 0x38,
  Key_9 = 0x39,
  Key_Colon = 0x3a,
  Key_Semicolon = 0x3b,
  Key_Less = 0x3c,
  Key_Equal = 0x3d,
  Key_Greater = 0x3e,
  Key_Question = 0x3f,
  Key_At = 0x40,
  Key_A = 0x41,
  Key_B = 0x42,
  Key_C = 0x43,
  Key_D = 0x44,
  Key_E = 0x45,
  Key_F = 0x46,
  Key_G = 0x47,
  Key_H = 0x48,
  Key_I = 0x49,
  Key_J = 0x4a,
  Key_K = 0x4b,
  Key_L = 0x4c,
  Key_M = 0x4d,
  Key_N = 0x4e,
  Key_O = 0x4f,
  Key_P = 0x50,
  Key_Q = 0x51,
  Key_R = 0x52,
  Key_S = 0x53,
  Key_T = 0x54,
  Key_U = 0x55,
  Key_V = 0x56,
  Key_W = 0x57,
  Key_X = 0x58,
  Key_Y = 0x59,
  Key_Z = 0x5a,
  Key_BracketLeft = 0x5b,
  Key_Backslash = 0x5c,
  Key_BracketRight = 0x5d,
  Key_AsciiCircum = 0x5e,
  Key_Underscore = 0x5f,
  Key_QuoteLeft = 0x60,
  Key_BraceLeft = 0x7b,
  Key_Bar = 0x7c,
  Key_BraceRight = 0x7d,
  Key_AsciiTilde = 0x7e,
  Key_nobreakspace = 0x0a0,
  Key_exclamdown = 0x0a1,
  Key_cent = 0x0a2,
  Key_sterling = 0x0a3,
  Key_currency = 0x0a4,
  Key_yen = 0x0a5,
  Key_brokenbar = 0x0a6,
  Key_section = 0x0a7,
  Key_diaeresis = 0x0a8,
  Key_copyright = 0x0a9,
  Key_ordfeminine = 0x0aa,
  Key_guillemotleft = 0x0ab,      // left angle quotation mark
  Key_notsign = 0x0ac,
  Key_hyphen = 0x0ad,
  Key_registered = 0x0ae,
  Key_macron = 0x0af,
  Key_degree = 0x0b0,
  Key_plusminus = 0x0b1,
  Key_twosuperior = 0x0b2,
  Key_threesuperior = 0x0b3,
  Key_acute = 0x0b4,
  Key_mu = 0x0b5,
  Key_paragraph = 0x0b6,
  Key_periodcentered = 0x0b7,
  Key_cedilla = 0x0b8,
  Key_onesuperior = 0x0b9,
  Key_masculine = 0x0ba,
  Key_guillemotright = 0x0bb,     // right angle quotation mark
  Key_onequarter = 0x0bc,
  Key_onehalf = 0x0bd,
  Key_threequarters = 0x0be,
  Key_questiondown = 0x0bf,
  Key_Agrave = 0x0c0,
  Key_Aacute = 0x0c1,
  Key_Acircumflex = 0x0c2,
  Key_Atilde = 0x0c3,
  Key_Adiaeresis = 0x0c4,
  Key_Aring = 0x0c5,
  Key_AE = 0x0c6,
  Key_Ccedilla = 0x0c7,
  Key_Egrave = 0x0c8,
  Key_Eacute = 0x0c9,
  Key_Ecircumflex = 0x0ca,
  Key_Ediaeresis = 0x0cb,
  Key_Igrave = 0x0cc,
  Key_Iacute = 0x0cd,
  Key_Icircumflex = 0x0ce,
  Key_Idiaeresis = 0x0cf,
  Key_ETH = 0x0d0,
  Key_Ntilde = 0x0d1,
  Key_Ograve = 0x0d2,
  Key_Oacute = 0x0d3,
  Key_Ocircumflex = 0x0d4,
  Key_Otilde = 0x0d5,
  Key_Odiaeresis = 0x0d6,
  Key_multiply = 0x0d7,
  Key_Ooblique = 0x0d8,
  Key_Ugrave = 0x0d9,
  Key_Uacute = 0x0da,
  Key_Ucircumflex = 0x0db,
  Key_Udiaeresis = 0x0dc,
  Key_Yacute = 0x0dd,
  Key_THORN = 0x0de,
  Key_ssharp = 0x0df,
  Key_agrave = 0x0e0,
  Key_aacute = 0x0e1,
  Key_acircumflex = 0x0e2,
  Key_atilde = 0x0e3,
  Key_adiaeresis = 0x0e4,
  Key_aring = 0x0e5,
  Key_ae = 0x0e6,
  Key_ccedilla = 0x0e7,
  Key_egrave = 0x0e8,
  Key_eacute = 0x0e9,
  Key_ecircumflex = 0x0ea,
  Key_ediaeresis = 0x0eb,
  Key_igrave = 0x0ec,
  Key_iacute = 0x0ed,
  Key_icircumflex = 0x0ee,
  Key_idiaeresis = 0x0ef,
  Key_eth = 0x0f0,
  Key_ntilde = 0x0f1,
  Key_ograve = 0x0f2,
  Key_oacute = 0x0f3,
  Key_ocircumflex = 0x0f4,
  Key_otilde = 0x0f5,
  Key_odiaeresis = 0x0f6,
  Key_division = 0x0f7,
  Key_oslash = 0x0f8,
  Key_ugrave = 0x0f9,
  Key_uacute = 0x0fa,
  Key_ucircumflex = 0x0fb,
  Key_udiaeresis = 0x0fc,
  Key_yacute = 0x0fd,
  Key_thorn = 0x0fe,
  Key_ydiaeresis = 0x0ff,
  Key_unknown = 0xffff,
  Key_none = 0xffff//= Key_unknown
};

}//namespace mitk
#endif //ifndef MITKINTERACTCONST_H

</xsl:text>
</xsl:template>
</xsl:stylesheet>