/****************************************************************************
 * A debug only (needs double parenthesis) printf() like debug facility
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef DEBUG_PRINTF_H
#define DEBUG_PRINTF_H

// Use double parenthesis to make DebugPrintf() a conditional compilation, eg;
//  DebugPrintf(("Hello world, x=%d",x));   // <- eliminated unless _DEBUG build
#ifdef _DEBUG
    #define DebugPrintf(x) DebugPrintfInner x
#else
    #define DebugPrintf(x)
#endif
void DebugPrintfInner( const char *fmt, ... );

#endif			// DEBUG_PRINTF_H
