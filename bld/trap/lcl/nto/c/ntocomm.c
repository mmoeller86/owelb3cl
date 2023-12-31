/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2022 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  QNX Neutrino common trap file routines.
*
****************************************************************************/


#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "trpimp.h"


#if 0
// TODO: NIDs don't exist on Neutrino. There is a different mechanism
// available to support distributed computing.
const char *CollectNid( const char *ptr, size_t len, nid_t *nidp )
{
    const char  *start;
    nid_t       nid;
    char        ch;

    nid = 0;
    if( ptr[0] == '/' && ptr[1] == '/' ) {
        start = ptr;
        len -= 2;
        ptr += 2;
        // NYI: will need beefing up when NID's can be symbolic
        while( len > 0 ) {
            ch = *ptr;
            if( ch < '0' || ch > '9' )
                break;
            nid = ( nid * 10 ) + ( ch - '0' );
            ++ptr;
            --len;
        }
        // NYI: how do I check to see if NID is valid?
        if( len > 0 ) {
            switch( ptr[0] ) {
            CASE_SEPS
                break;
            default:
                *nidp = 0;
                ptr = start;
                break;
            }
        }
    }
    *nidp = nid;
    return( ptr );
}
#endif


trap_retval TRAP_CORE( Read_user_keyboard )( void )
{
    read_user_keyboard_req  *acc;
    read_user_keyboard_ret  *ret;
    fd_set                  rdfs;
    struct timeval          tv;
    struct timeval          *ptv;
    struct termios          old, new;

    acc = GetInPtr( 0 );
    ret = GetOutPtr( 0 );
    CONV_LE_16( acc->wait );

    tcgetattr( STDIN_FILENO, &old );
    new = old;
    new.c_iflag &= ~(IXOFF | IXON);
    new.c_lflag &= ~(ECHO | ICANON | NOFLSH);
    new.c_lflag |= ISIG;
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;
    tcsetattr( STDIN_FILENO, TCSADRAIN, &new );

    FD_ZERO( &rdfs );
    FD_SET( STDIN_FILENO, &rdfs );
    tv.tv_sec = acc->wait;
    tv.tv_usec = 0;
    ptv = &tv;
    if( acc->wait == 0 )
        ptv = NULL;

    ret->key = '\0';
    if ( select( 1, &rdfs, NULL, NULL, ptv ) )
        read( STDIN_FILENO, &ret->key, 1 );

    tcsetattr( STDIN_FILENO, TCSADRAIN, &old );
    return( sizeof( *ret ) );
}


trap_retval TRAP_CORE( Get_err_text )( void )
{
    get_err_text_req    *acc;
    char                *err_txt;

    acc = GetInPtr( 0 );
    CONV_LE_32( acc->err );
    err_txt = GetOutPtr( 0 );
    strcpy( err_txt, strerror( acc->err ) );
    return( strlen( err_txt ) + 1 );
}


trap_retval TRAP_CORE( Split_cmd )( void )
{
    char                ch;
    const char          *cmd;
    const char          *start;
    split_cmd_ret       *ret;
    size_t              len;
//    nid_t               nid;

    start = cmd = GetInPtr( sizeof( split_cmd_req ) );
    len = GetTotalSizeIn() - sizeof( split_cmd_req );
    ret = GetOutPtr( 0 );
    ret->parm_start = 0;
//    cmd = CollectNid( cmd, len, &nid );
    len -= cmd - start;
    while( len > 0 ) {
        switch( *cmd ) {
        CASE_SEPS
            break;
        defaults:
            while( len > 0 ) {
                switch( *cmd ) {
                CASE_SEPS
                    ret->parm_start = 1;
                    len = 0;
                    continue;
                }
                ++cmd;
            }
            continue;
        }
        ++cmd;
        --len;
    }
    ret->parm_start += cmd - start;
    ret->cmd_end = cmd - start;
    CONV_LE_16( ret->cmd_end );
    CONV_LE_16( ret->parm_start );
    return( sizeof( *ret ) );
}

trap_retval TRAP_CORE( Get_next_alias )( void )
{
    get_next_alias_ret  *ret;

    ret = GetOutPtr( 0 );
    ret->seg = 0;
    ret->alias = 0;
    return( sizeof( *ret ) );
}

trap_retval TRAP_CORE( Set_user_screen )( void )
{
    return( 0 );
}

trap_retval TRAP_CORE( Set_debug_screen )( void )
{
    return( 0 );
}
