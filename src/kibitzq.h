/****************************************************************************
 * A queue of lines of kibitz text from a kibitzing chess engine
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef KIBITZQ_H
#define KIBITZQ_H

#include <string.h>
#include "Appdefs.h"    // for MATE_IN_ZERO_ENGINE_BUG_WORKAROUND
#ifndef nbrof
    #define nbrof(x) ( sizeof(x) / sizeof((x)[0]) )
#endif

class Kibitzq
{
    #define KQDEPTH 64
    #define KQBUFSIZE 256
    char buf[KQDEPTH][KQBUFSIZE];
    bool cleared;
    unsigned int kqdepth;
    unsigned int put;
    unsigned int get;
    unsigned int  Bump( unsigned int put_or_get ) { return( put_or_get+1 >= kqdepth ? 0 : put_or_get+1 ); }
    bool Full()  { return( Bump(put) == get ); }
    bool Empty() { return( put == get ); }

public:
    Kibitzq() { kqdepth=KQDEPTH; cleared=false; put=get=0; }
    void SetDepth( int depth ) { if( depth<=KQDEPTH ) kqdepth=depth; }
    void Clear() { cleared=true; put=0; get=0; }
    bool TestCleared() { bool temp=cleared; cleared=false; return temp; }
    void Put( const char *txt )
    {
        bool full = Full();
        unsigned int temp = put;
        unsigned int len = strlen(txt);
        if( len < KQBUFSIZE )
            strcpy( &buf[temp][0], txt );
        else
        {
            memcpy( &buf[temp][0], txt, KQBUFSIZE-1 );
            buf[temp][KQBUFSIZE-1] = '\0';

            // This is the ugliest workaround in the world
            #ifdef  MATE_IN_ZERO_ENGINE_BUG_WORKAROUND
            if( strstr(txt," score mate 0 ") )  // bug?: should never be mate 0, should be +ve or -ve integer
            {
                const char *s = strstr(txt," pv ");  // so find pv
                if( s )
                {
                    s += 4;             // point after " pv " at first half move of pv;

                    // Count how many plies (i.e. half moves) there are before mate
                    int ply_count=0;
                    while( *s  )
                    {
                        if( *s == ' ' )
                            s++;
                        else
                        {
                            ply_count++;              // found start of move
                            while( *s && *s != ' ' )  // skip over move
                                s++;
                        }
                    }
                    char nbr[80];
                    sprintf( nbr, " NUM %d", ply_count );

                    // Replace end of buffered text with eg " NUM 34" if there are 34 plies
                    char *end = &buf[temp][KQBUFSIZE-1];    // point at terminating '\0'
                    end -= strlen(nbr);                     // here or before here
                    while( end > &buf[temp][0] )            // find an existing space
                    {
                        if( *end == ' ' )
                        {
                            strcpy( end, nbr );
                            break;
                        }
                        end--;
                    }
                }
            }
            #endif
        }
        put = Bump(temp);
        if( full )
            get = Bump(get);
    }
    bool Get( char buf[], unsigned int max_strlen )
    {
        bool data_read = false;
        if( !Empty() )
        {
            unsigned int temp = get;
            const char *src = &this->buf[temp][0];
            unsigned int len = strlen(src);
            if( len <= max_strlen )
                strcpy( buf, src );
            else
            {
                memcpy( buf, src, max_strlen );
                buf[max_strlen] = '\0';
            }
            get = Bump(temp);
            data_read = true;
        }
        return data_read;
    }
};

#endif // KITBITZQ
