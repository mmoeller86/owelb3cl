/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "vi.h"
#include "win.h"
#ifdef _M_IX86
    #ifdef __WATCOMC__
        #include <i86.h>
    #endif
#endif
#if defined( __OS2__ )
    #define INCL_BASE
    #include <os2.h>
#endif

static vi_key   overrideKeyBuff[MAX_OVERRIDE_KEY_BUFF];
static int      overrideKeyPos, overrideKeyEnd;

#if !defined( __UNIX__ ) && !defined( __NT__ ) || defined( __WIN__ )
    #define BUSYWAIT
#endif

#if defined( __UNIX__ ) || defined( __NT__ ) && !defined( __WIN__ )
    #define NO_TRANSLATE
#endif

#if !defined( NO_TRANSLATE )
static unsigned char altLookup[] = { 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37,
                                     38, 50, 49, 24, 25, 16, 19, 31, 20, 22, 47,
                                     17, 45, 21, 44  };
static unsigned char altLookup2[] = { 117, 147, 146, 119, 132, 118, 115, 116, 141, 145,
                                      159, 163, 162, 151, 153, 161, 155, 158, 152, 160,
                                      148, 165 };
#endif
/*
 * clearSpin - clear spin area
 */
static void clearSpin( void )
{
    if( EditFlags.Spinning ) {
#if !defined(__WIN__)
        SpinLoc->cinfo_char = ' ';
#endif
#ifdef __VIO__
        MyVioShowBuf( SpinLoc - Scrn, 1 );
#endif
        EditFlags.SpinningOurWheels = FALSE;
    }

} /* clearSpin */

/*
 * getOverrideKey - get next key from the override buffer
 */
static vi_key getOverrideKey( void )
{
    int         c;
    bool        mouse = FALSE;
    int         mcnt = 0;

    for( ;; ) {
        c = overrideKeyBuff[overrideKeyPos];
        if( overrideKeyPos == MAX_OVERRIDE_KEY_BUFF - 1 ) {
            overrideKeyPos = 0;
        } else {
            overrideKeyPos++;
        }
        if( overrideKeyPos == overrideKeyEnd ) {
            EditFlags.KeyOverride = FALSE;
        }
        if( c == VI_KEY( MOUSEEVENT ) && !mouse ) {
            mouse = TRUE;
            mcnt = 0;
        } else {
            if( mouse ) {
                switch( mcnt ) {
                case 0:
                    LastMouseEvent = c;
                    break;
                case 1:
                    MouseRow = c;
                    break;
                case 2:
                    MouseCol = c;
                    break;
                case 3:
                    MouseStatus = c;
                    mouse = FALSE;
                    c = VI_KEY( MOUSEEVENT );
                    RedrawMouse( MouseRow, MouseCol );
                    DisplayMouse( TRUE );
                    break;
                }
                mcnt++;
            }
            if( !mouse ) {
#ifndef __WIN__
                if( c == 3 ) {
//                  ExitWithVerify();
                }
#endif
                break;
            }
        }
    }
    return( c );

} /* getOverrideKey */

/*
 * GetVIKey - return a VI equivalent of a given key/scan code
 */
vi_key GetVIKey( vi_key ch, int scan, bool shift )
{
#if !defined NO_TRANSLATE
    int     i;
#endif

    if( EditFlags.EscapedInsertChar ) {
        return( ch );
    }

    if( ch != 0 ) {
        if( ch == 127 && scan == 14 ) {
            return( VI_KEY( CTRL_BS ) );
        }
        if( ch == 8 ) {
            ch = VI_KEY( BS );
        } else if( ch == 9 ) {
            ch = VI_KEY( TAB );
        } else if( ch == 13 ) {
            ch = VI_KEY( ENTER );
        } else if( ch == 27 ) {
            ch = VI_KEY( ESC );
        }
        return( ch );
    }
#if !defined( NO_TRANSLATE )

    /*
     * special characters: fcn keys, and cursor control
     */
    switch( scan ) {
    case 14:
        return( VI_KEY( ALT_BS ) );
    case 15:
        return( VI_KEY( SHIFT_TAB ) );
    case 71:
        if( shift ) {
            return( VI_KEY( SHIFT_HOME ) );
        } else {
            return( VI_KEY( HOME ) );
        }
    case 72:
        if( shift ) {
            return( VI_KEY( SHIFT_UP ) );
        } else {
            return( VI_KEY( UP ) );
        }
    case 73:
        if( shift ) {
            return( VI_KEY( SHIFT_PAGEUP ) );
        } else {
            return( VI_KEY( PAGEUP ) );
        }
    case 75:
        if( shift ) {
            return( VI_KEY( SHIFT_LEFT ) );
        } else {
            return( VI_KEY( LEFT ) );
        }
    case 77:
        if( shift ) {
            return( VI_KEY( SHIFT_RIGHT ) );
        } else {
            return( VI_KEY( RIGHT ) );
        }
    case 79:
        if( shift ) {
            return( VI_KEY( SHIFT_END ) );
        } else {
            return( VI_KEY( END ) );
        }
    case 80:
        if( shift ) {
            return( VI_KEY( SHIFT_DOWN ) );
        } else {
            return( VI_KEY( DOWN ) );
        }
    case 81:
        if( shift ) {
            return( VI_KEY( SHIFT_PAGEDOWN ) );
        } else {
            return( VI_KEY( PAGEDOWN ) );
        }
    case 82:
        if( shift ) {
            return( VI_KEY( SHIFT_INS ) );
        } else {
            return( VI_KEY( INS ) );
        }
    case 83:
        if( shift ) {
            return( VI_KEY( SHIFT_DEL ) );
        } else {
            return( VI_KEY( DEL ) );
        }
    }
    if( scan >= 59 && scan <= 68 ) {
        return ( scan - 59 + VI_KEY( F1 ) );
    }
    if( scan >= 84 && scan <= 113 ) {
        if( scan >= 84 && scan <= 93 ) {
            return ( scan - 84 + VI_KEY( SHIFT_F1 ) );
        }
        if( scan >= 94 && scan <= 103 ) {
            return ( scan - 94 + VI_KEY( CTRL_F1 ) );
        }
#ifdef __WIN
        if( scan >= 104 && scan <= 105 ) {
            return( scan - 104 + VI_KEY( ALT_F1 ) );
        }
#else
        if( scan >= 104 && scan <= 113 ) {
            return( scan - 104 + VI_KEY( ALT_F1 ) );
        }
#endif
    }
    if( scan == 130 ) {
        return( VI_KEY( ALT_HYPHEN ) );
    }
    if( scan >= 133 && scan <= 140 ) {
        return( scan - 133 + VI_KEY( F11 ) );
    }
    for( i = 0; i < sizeof( altLookup ); i++ ) {
        if( altLookup[i] == scan ) {
            return( VI_KEY( ALT_A ) + i );
        }
    }
    for( i = 0; i < sizeof( altLookup2 ); i++ ) {
        if( altLookup2[i] == scan ) {
            return( VI_KEY( CTRL_END ) + i );
        }
    }
    return( VI_KEY( DUMMY ) );
#else
    /* avoid warning about unused arguments */
    (void) scan;
    (void) shift;
    return( ch );
#endif

} /* GetVIKey */

/*
 * GetKey - read a key from the keyboard
 */
vi_key GetKey( bool usemouse )
{
    bool        hit;
    vi_key      key;

    clearSpin();
    for( ;; ) {
        if( EditFlags.NoCapsLock ) {
            TurnOffCapsLock();
        }
        LastMouseEvent = GetMouseEvent();
        if( LastMouseEvent != MOUSE_NONE ) {
            RedrawMouse( MouseRow, MouseCol );
            DisplayMouse( TRUE );
            if( TestMouseEvent( usemouse ) ) {
                return( VI_KEY( MOUSEEVENT ) );
            }
        }
        /*
         * could get set by mouse handler or IDEGetKeys
         */
        if( EditFlags.KeyOverride ) {
            key = getOverrideKey();
#ifndef __WIN__
            if( key == VI_KEY( CTRL_C ) ) {
                ExitWithVerify();
                clearSpin();
                continue;
            }
#endif
            return( key );
        }
        hit = KeyboardHit();
        if( !hit ) {
            DoAutoSave();
#ifdef __IDE__
            IDEGetKeys();
#endif
#ifdef __QNX__
            WaitForProxy();
            if( !KeyboardHit() ) {
                continue;
            }
#endif
        }
#if defined( BUSYWAIT )
        else {
#endif
            key = GetKeyboard();
#if defined( __NT__ )
            if( key == VI_KEY( MOUSEEVENT ) ) {
                continue;
            }
#endif
            DisplayMouse( FALSE );
#ifdef __WIN__
            if( key == 0 ) {
                GetKeyboard();
                ExitWithVerify();
                clearSpin();
                continue;
            }
#else
            if( !EditFlags.EscapedInsertChar && (key == VI_KEY( CTRL_C ) || key == 0) ) {
                if( key == 0 ) {
                    GetKeyboard();
                }
                ExitWithVerify();
                clearSpin();
                continue;
            }
#endif
            break;
#if defined( BUSYWAIT )
        }
#endif
#if defined( __OS2__ )
        DosSleep( 1 );
#endif
    }

    if( EditFlags.Spinning ) {
        EditFlags.SpinningOurWheels = TRUE;
    }
    return( key );

} /* GetKey */

/*
 * NonKeyboardEventsPending - check if anything is buffered and waiting
 */
bool NonKeyboardEventsPending( void )
{
    if( EditFlags.KeyOverride ) {
        return( TRUE );
    }
    if( EditFlags.InputKeyMapMode || EditFlags.KeyMapMode ) {
        return( TRUE );
    }
    if( EditFlags.DotMode || EditFlags.AltDotMode ) {
        return( TRUE );
    }
    return( FALSE );

} /* NonKeyboardEventsPending */

/*
 * GetNextEvent - return next event from appropriate source
 */
vi_key GetNextEvent( bool usemouse )
{
    vi_key  key;

    if( EditFlags.NoCapsLock ) {
        TurnOffCapsLock();
    }
    if( EditFlags.KeyOverride ) {
        key = getOverrideKey();
        LastEvent = key;
        return( key );
    }

    if( EditFlags.InputKeyMapMode || EditFlags.KeyMapMode ) {

        key = CurrentKeyMap[CurrentKeyMapCount++];

        if( key == 0 ) {
            EditFlags.NoInputWindow = FALSE;
            if( EditFlags.InputKeyMapMode ) {
                DoneInputKeyMap();
            } else {
                EditFlags.KeyMapMode = FALSE;
            }
        } else {
            LastEvent = key;
            return( key );
        }

    }

    if( EditFlags.DotMode ) {
        LastEvent = DotCmd[DotCount++];
        return( LastEvent );
    }
    if( EditFlags.AltDotMode ) {
        if( AltDotCount > AltDotDigits ) {
            return( VI_KEY( ESC ) );
        }
        key = AltDotBuffer[AltDotCount++];
    } else {
        key = GetKey( usemouse );
    }

    if( !EditFlags.NoAddToDotBuffer ) {
        if( key != VI_KEY( MOUSEEVENT ) ) {
            if( EditFlags.AltMemorizeMode ) {
                if( AltDotDigits < maxdotbuffer ) {
                    AltDotBuffer[AltDotDigits++] = key;
                } else {
                    AltDotBuffer[AltDotDigits - 1] = VI_KEY( ESC );
                }
            }
            if( DotDigits < maxdotbuffer ) {
                DotBuffer[DotDigits++] = key;
            } else {
                DotBuffer[DotDigits - 1] = VI_KEY( ESC );
            }
        }
    }
    LastEvent = key;
    return( key );

} /* GetNextEvent */

/*
 *  ClearBreak - clear a break after it is pressed
 */
void ClearBreak( void )
{
    while( KeyboardHit() ) {
        GetKeyboard();
    }
    EditFlags.BreakPressed = FALSE;

} /* ClearBreak */

/*
 * KeyAdd - add a keystroke to the override buffer
 */
void KeyAdd( vi_key key )
{
    overrideKeyBuff[overrideKeyEnd] = key;
    if( overrideKeyEnd == MAX_OVERRIDE_KEY_BUFF - 1 ) {
        overrideKeyEnd = 0;
    } else {
        overrideKeyEnd++;
    }
    EditFlags.KeyOverride = TRUE;

} /* KeyAdd */

/*
 * KeyAddString - add an entire string
 */
void KeyAddString( char *str )
{
    key_map     scr;
    vi_rc       rc;
    vi_key      *s;

    rc = AddKeyMap( &scr, str );
    if( rc == ERR_NO_ERR ) {
        s = scr.data;
        while( *s != 0 ) {
            KeyAdd( *s );
            s++;
        }
    }
    MemFree( scr.data );

} /* KeyAddString */

/*
 * AddCurrentMouseEvent - add current mouse event to the override buffer
 */
void AddCurrentMouseEvent( void )
{
    KeyAdd( VI_KEY( MOUSEEVENT ) );
    KeyAdd( LastMouseEvent );
    KeyAdd( MouseRow );
    KeyAdd( MouseCol );
    KeyAdd( MouseStatus );

} /* AddCurrentMouseEvent */
