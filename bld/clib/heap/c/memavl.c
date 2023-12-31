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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "dll.h"        // needs to be first
#include "variety.h"
#include <stddef.h>
#include <malloc.h>
#include "roundmac.h"
#include "heap.h"
#include "heapacc.h"


/* Return the amount of memory available in the near heap. */
/* Best done at start of program and after _nheapgrow() has been called. */

_WCRTLINK size_t _memavl( void )
{
    size_t          length;
    freelist_nptr   frl;
    heapblk_nptr    heap;

    length = 0;
    _AccessNHeap();
    for( heap = __nheapbeg; heap != NULL; heap = heap->next.nptr ) {
        for( frl = heap->freehead.next.nptr; frl != (freelist_nptr)&heap->freehead; frl = frl->next.nptr ) {
            length += __ROUND_DOWN_SIZE_HEAP( frl->len );
        }
    }
    _ReleaseNHeap();
    return( length );
}
