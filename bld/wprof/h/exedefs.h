/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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


#ifndef _EXEDEFS_H
#define _EXEDEFS_H

#include "exedos.h"
#include "exeos2.h"
#include "exephar.h"
#include "exenov.h"
#include "exeflat.h"
#include "exeqnx.h"
#include "exepe.h"
#include "exeelf.h"
#if 0   /* not yet supported by WPROF */
#include "exe16m.h"
#endif

typedef enum {
    EXE_TYPE_NONE,
    EXE_TYPE_MZ,                /* DOS */
    EXE_TYPE_MP,                /* PharLap Simple */
    EXE_TYPE_P2,                /* PharLap Extended 286 */
    EXE_TYPE_P3,                /* PharLap Extended 386 */
    EXE_TYPE_NW,                /* NetWare 386 Loadable Module */
    EXE_TYPE_OS2,               /* OS/2 1.x 16-bit executable */
    EXE_TYPE_OVL,               /* overlay file (e.g., SECTION FILE=xxx) */
    EXE_TYPE_P3_BOUND,          /* PharLap Extended 386 bound executable */
    EXE_TYPE_MP_BOUND,          /* WATCOM Windows 386 bound executable */
    EXE_TYPE_OS2_FLAT,          /* OS/2 2.x FLAT executable */
    EXE_TYPE_QNX,               /* QNX 286 executable */
    EXE_TYPE_QNX_386,           /* QNX 386 executable */
    EXE_TYPE_OS2_LX,            /* OS/2 2.x Extended executable */
    EXE_TYPE_PE,                /* Windows NT Executable */
    EXE_TYPE_PL,                /* Windows NT Executable */
    EXE_TYPE_QNX_386_FLAT,      /* QNX 386 flat model executable */
    EXE_TYPE_ELF,               /* ELF executable (QNX Neutrino) */
    EXE_TYPE_UNKNOWN
} EXE_TYPE;

#endif
