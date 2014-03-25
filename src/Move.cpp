/****************************************************************************
 * Chess classes - Move
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <algorithm>
#include <string>
#include "DebugPrintf.h"
#include "PrivateChessDefs.h"
#include "ChessRules.h"
#include "Move.h"
using namespace std;
using namespace thc;

/****************************************************************************
 * Read natural string move eg "Nf3"
 *  return bool okay
 ****************************************************************************/
bool Move::NaturalIn( ChessRules *cr, const char *natural_in )
{
    MOVELIST list;
    int  i, len=0;
    char src_file='\0', src_rank='\0', dst_file='\0', dst_rank='\0';
    char promotion='\0';
    bool enpassant=false;
    bool kcastling=false;
    bool qcastling=false;
    Square dst=a8;
    Move *m, *found=NULL;
    char *s;
    char  move[10];
    bool  white=cr->white;
    char  piece=(white?'P':'p');
    bool  default_piece=true;
    
    // Indicate no move found (yet)
    bool okay=true;

    // Copy to read-write variable
    okay = false;
    for( i=0; i<sizeof(move); i++ )
    {
        move[i] = natural_in[i];
        if( move[i]=='\0' || move[i]==' ' || move[i]=='\t' ||
            move[i]=='\r' || move[i]=='\n' )
        {
            move[i] = '\0';
            okay = true;
            break;
        }
    }
    if( okay )
    {

        // Trim string from end
        s = strchr(move,'\0') - 1;
        while( s>=move && !(isascii(*s) && isalnum(*s)) )
            *s-- = '\0';

        // Trim string from start
        s = move;
        while( *s==' ' || *s=='\t' )
            s++;
        len = (int)strlen(s);
        for( i=0; i<len+1; i++ )  // +1 gets '\0' at end
            move[i] = *s++;  // if no leading space this does
                            //  nothing, but no harm either

        // Trim enpassant
        if( len>=2 && move[len-1]=='p' )
        {
            if( 0 == strcmp(&move[len-2],"ep") )
            {
                move[len-2] = '\0';
                enpassant = true;
            }
            else if( len>=3 && 0==strcmp(&move[len-3],"e.p") )
            {
                move[len-3] = '\0';
                enpassant = true;
            }

            // Trim string from end, again
            s = strchr(move,'\0') - 1;
            while( s>=move && !(isascii(*s) && isalnum(*s)) )
                *s-- = '\0';
            len = (int)strlen(move);
        }

        // Promotion
        if( strchr(move,'=') )
        {
            switch( move[len-1] )
            {
                case 'q':
                case 'Q':   promotion='Q';  break;
                case 'r':
                case 'R':   promotion='R';  break;
                case 'b':
                case 'B':   promotion='B';  break;
                case 'n':
                case 'N':   promotion='N';  break;
                default:    okay = false;   break;
            }
            if( okay )
            {

                // Trim string from end, again
                move[len-1] = '\0';
                s = strchr(move,'\0') - 1;
                while( s>=move && !(isascii(*s) && isalnum(*s)) )
                    *s-- = '\0';
                len = (int)strlen(move);
            }
        }    
    }

    // Castling
    if( okay )
    {
        if( 0==_strcmpi(move,"oo") || 0==_strcmpi(move,"o-o") )
        {
            strcpy( move, (white?"e1g1":"e8g8") );
            len       = 4;
            piece     = (white?'K':'k');
            default_piece = false;
            kcastling = true;
        }
        else if( 0==_strcmpi(move,"ooo") || 0==_strcmpi(move,"o-o-o") )
        {
            strcpy( move, (white?"e1c1":"e8c8") );
            len       = 4;
            piece     = (white?'K':'k');
            default_piece = false;
            qcastling = true;
        }
    }

    // Destination square for all except pawn takes pawn (eg "ef")
    if( okay )
    {
        if( len==2 && 'a'<=move[0] && move[0]<='h'
                   && 'a'<=move[1] && move[1]<='h' )
        {
            src_file = move[0]; // pawn takes pawn
            dst_file = move[1];
        }
        else if( len>=2 && 'a'<=move[len-2] && move[len-2]<='h'
                        && '1'<=move[len-1] && move[len-1]<='8' )
        {
            dst_file = move[len-2];
            dst_rank = move[len-1];
            dst = SQ(dst_file,dst_rank);
        }
        else
            okay = false;
    }

    // Source square and or piece
    if( okay )
    {
        if( len > 2 )
        {
            if( 'a'<=move[0] && move[0]<='h' &&
                '1'<=move[1] && move[1]<='8' )
            {
                src_file = move[0];
                src_rank = move[1];
            }
            else
            {
                switch( move[0] )
                {
                    case 'K':   piece = (white?'K':'k');    default_piece=false; break;
                    case 'Q':   piece = (white?'Q':'q');    default_piece=false; break;
                    case 'R':   piece = (white?'R':'r');    default_piece=false; break;
                    case 'N':   piece = (white?'N':'n');    default_piece=false; break;
                    case 'P':   piece = (white?'P':'p');    default_piece=false; break;
                    case 'B':   piece = (white?'B':'b');    default_piece=false; break;
                    default:
                    {
                        if( 'a'<=move[0] && move[0]<='h' )
                            src_file = move[0]; // eg "ef4"
                        else
                            okay = false;
                        break;
                    }
                }       
                if( len>3  && src_file=='\0' )  // not eg "ef4" above
                {
                    if( '1'<=move[1] && move[1]<='8' )
                        src_rank = move[1];
                    else if( 'a'<=move[1] && move[1]<='h' )
                    {
                        src_file = move[1];
                        if( len>4 && '1'<=move[2] && move[2]<='8' )
                            src_rank = move[2];
                    }
                }
            }
        }
    }

    // Check against all possible moves
    if( okay )
    {
        cr->GenLegalMoveList( &list );

        // Have source and destination, eg "d2d3"
        if( enpassant )
            src_rank = dst_rank = '\0';
        if( src_file && src_rank && dst_file && dst_rank )
        {
            for( i=0; i<list.count; i++ )
            {
                m = &list.moves[i];
                if( (default_piece || piece==cr->squares[m->src])  &&
                    src_file  ==   FILE(m->src)       &&
                    src_rank  ==   RANK(m->src)       &&
                    dst       ==   m->dst
                )
                {
                    if( kcastling )
                    {
                        if( m->special ==
                             (white?SPECIAL_WK_CASTLING:SPECIAL_BK_CASTLING) )
                            found = m;
                    }
                    else if( qcastling )
                    {
                        if( m->special ==
                             (white?SPECIAL_WQ_CASTLING:SPECIAL_BQ_CASTLING) )
                            found = m;
                    }
                    else
                        found = m;
                    break;
                }
            }
        }

        // Have source file only, eg "Rae1"
        else if( src_file && dst_file && dst_rank )
        {
            for( i=0; i<list.count; i++ )
            {
                m = &list.moves[i];
                if( piece     ==   cr->squares[m->src]  &&
                    src_file  ==   FILE(m->src)         &&
                 /* src_rank  ==   RANK(m->src)  */
                    dst       ==   m->dst
                )
                {
                    found = m;
                    break;
                }
            }
        }

        // Have source rank only, eg "R2d2"
        else if( src_rank && dst_file && dst_rank )
        {
            for( i=0; i<list.count; i++ )
            {
                m = &list.moves[i];
                if( piece     ==   cr->squares[m->src]   &&
                 /* src_file  ==   FILE(m->src) */
                    src_rank  ==   RANK(m->src)          &&        
                    dst       ==   m->dst
                )
                {
                    found = m;
                    break;
                }
            }
        }

        // Have destination file only eg e4f (because 2 ef moves are possible)
        else if( src_file && src_rank && dst_file )
        {
            for( i=0; i<list.count; i++ )
            {
                m = &list.moves[i];
                if( piece     ==   cr->squares[m->src]      &&
                    src_file  ==   FILE(m->src)             &&
                    src_rank  ==   RANK(m->src)             &&
                    dst_file  ==   FILE(m->dst)
                )
                {
                    found = m;
                    break;
                }
            }
        }

        // Have files only, eg "ef"
        else if( src_file && dst_file )
        {
            for( i=0; i<list.count; i++ )
            {
                m = &list.moves[i];
                if( piece     ==   cr->squares[m->src]      &&
                    src_file  ==   FILE(m->src)             &&
                 /* src_rank  ==   RANK(m->src) */                  
                    dst_file  ==   FILE(m->dst)
                )
                {
                    if( enpassant )
                    {
                        if( m->special ==
                             (white?SPECIAL_WEN_PASSANT:SPECIAL_BEN_PASSANT) )
                            found = m;
                    }
                    else
                        found = m;
                    break;
                }
            }
        }

        // Have destination square only eg "a4"
        else if( dst_rank && dst_file )
        {
            for( i=0; i<list.count; i++ )
            {
                m = &list.moves[i];
                if( piece     ==   cr->squares[m->src]          &&
                    dst       ==   m->dst
                )
                {
                    found = m;
                    break;
                }
            }
        }
    }

    // Copy found move
    if( okay && found )
    {
        bool found_promotion =
            ( found->special == SPECIAL_PROMOTION_QUEEN ||
              found->special == SPECIAL_PROMOTION_ROOK ||
              found->special == SPECIAL_PROMOTION_BISHOP ||
              found->special == SPECIAL_PROMOTION_KNIGHT
            );
        if( promotion && !found_promotion )
            okay = false;
        if( found_promotion )
        {
            switch( promotion )
            {
                default:
                case 'Q': found->special = SPECIAL_PROMOTION_QUEEN;   break;
                case 'R': found->special = SPECIAL_PROMOTION_ROOK;    break;
                case 'B': found->special = SPECIAL_PROMOTION_BISHOP;  break;
                case 'N': found->special = SPECIAL_PROMOTION_KNIGHT;  break;
            }
        }
    }
    if( !found )
        okay = false;
    if( okay )
        *this = *found;
    return okay;
}

/****************************************************************************
 * Read terse string move eg "g1f3"
 *  return bool okay
 ****************************************************************************/
bool Move::TerseIn( ChessRules *cr, const char *tmove )
{
    MOVELIST list;
    int i;
    bool okay=false;
    if( strlen(tmove)>=4 && 'a'<=tmove[0] && tmove[0]<='h'
                         && '1'<=tmove[1] && tmove[1]<='8'
                         && 'a'<=tmove[2] && tmove[2]<='h'
                         && '1'<=tmove[3] && tmove[3]<='8' )
    {
        Square src   = SQ(tmove[0],tmove[1]);
        Square dst   = SQ(tmove[2],tmove[3]);
        char   expected_promotion_if_any = 'Q';
        if( tmove[4] )
        {
            if( tmove[4]=='n' || tmove[4]=='N' )
                expected_promotion_if_any = 'N';
            else if( tmove[4]=='b' || tmove[4]=='B' )
                expected_promotion_if_any = 'B';
            else if( tmove[4]=='r' || tmove[4]=='R' )
                expected_promotion_if_any = 'R';
        }

        // Generate legal moves, then search for this move
        cr->GenLegalMoveList( &list );
        for( i=0; !okay && i<list.count; i++ )
        {
            if( list.moves[i].dst==dst && list.moves[i].src==src )
            {
                switch( list.moves[i].special )
                {
                    default:    okay=true;  break;
                    case SPECIAL_PROMOTION_QUEEN:
                    {
                        if( expected_promotion_if_any == 'Q' )
                            okay = true;
                        break;
                    }
                    case SPECIAL_PROMOTION_ROOK:
                    {
                        if( expected_promotion_if_any == 'R' )
                            okay = true;
                        break;
                    }
                    case SPECIAL_PROMOTION_BISHOP:
                    {
                        if( expected_promotion_if_any == 'B' )
                            okay = true;
                        break;
                    }
                    case SPECIAL_PROMOTION_KNIGHT:
                    {
                        if( expected_promotion_if_any == 'N' )
                            okay = true;
                        break;
                    }
                }
            }
            if( okay )
                *this = list.moves[i];
        }
    }
    return okay;
}

/****************************************************************************
 * Convert to natural string
 *    eg "Nf3"
 ****************************************************************************/
std::string Move::NaturalOut( ChessRules *cr )
{
    
// Improved algorithm

    /* basic procedure is run the following algorithms in turn:
        pawn move     ?
        castling      ?
        Nd2 or Nxd2   ? (loop through all legal moves check if unique)
        Nbd2 or Nbxd2 ? (loop through all legal moves check if unique)   
        N1d2 or N1xd2 ? (loop through all legal moves check if unique)
        Nb1d2 or Nb1xd2 (fallback if nothing else works)
    */

    char nmove[10];
    nmove[0] = '-';    
    nmove[1] = '-';    
    nmove[2] = '\0';    
    MOVELIST list;
    bool check[MAXMOVES];
    bool mate[MAXMOVES];
    bool stalemate[MAXMOVES];
    enum
    {
        ALG_PAWN_MOVE,
        ALG_CASTLING,
        ALG_ND2,
        ALG_NBD2,
        ALG_N1D2,
        ALG_NB1D2
    };
    bool done=false;
    bool found = false;
    char append='\0';
    cr->GenLegalMoveList( &list, check, mate, stalemate );
    Move mfound = list.moves[0];   // just to prevent a bogus compiler uninitialized var warning
    for( int i=0; !found && i<list.count; i++ )
    {
        mfound = list.moves[i];
        if( mfound == *this )
        {
            found = true;
            if( mate[i] )
                append = '#';
            else if( check[i] )
                append = '+';
        }
    }

    // Loop through algorithms
    for( int alg=ALG_PAWN_MOVE; found && !done && alg<=ALG_NB1D2; alg++ )
    {
        bool do_loop = (alg==ALG_ND2 || alg==ALG_NBD2 || alg==ALG_N1D2);
        int matches=0;
        Move m;

        // Run the algorithm on the input move (i=-1) AND on all legal moves
        //  in a loop if do_loop set for this algorithm (i=0 to i=count-1)
        for( int i=-1; !done && i<(do_loop?list.count:0); i++ )
        {
            char *str_dst;
            char compare[10];
            if( i == -1 )
            {
                m = *this;
                str_dst = nmove;
            }
            else
            {
                m = list.moves[i];
                str_dst = compare;
            }
            Square src = m.src;
            Square dst = m.dst;
            char t, p = cr->squares[src];
            if( islower(p) )
                p = (char)toupper(p);
            if( !IsEmptySquare(m.capture) ) // until we did it this way, enpassant was '-' instead of 'x'
                t = 'x';
            else
                t = '-';
            switch( alg )
            {
                // pawn move ? "e4" or "exf6", plus "=Q" etc if promotion
                case ALG_PAWN_MOVE:
                {
                    if( p == 'P' )
                    {
                        done = true;
                        if( t == 'x' )
                            sprintf( nmove, "%cx%c%c", FILE(src),FILE(dst),RANK(dst) );
                        else
                            sprintf( nmove, "%c%c",FILE(dst),RANK(dst) );
                        char *s = strchr(nmove,'\0');
                        switch( m.special )
                        {
                            case SPECIAL_PROMOTION_QUEEN:
                                strcpy( s, "=Q" );  break;
                            case SPECIAL_PROMOTION_ROOK:
                                strcpy( s, "=R" );  break;
                            case SPECIAL_PROMOTION_BISHOP:
                                strcpy( s, "=B" );  break;
                            case SPECIAL_PROMOTION_KNIGHT:
                                strcpy( s, "=N" );  break;
                        }
                    }
                    break;
                }

                // castling ?
                case ALG_CASTLING:
                {
                    if( m.special==SPECIAL_WK_CASTLING || m.special==SPECIAL_BK_CASTLING )
                    {
                        strcpy( nmove, "O-O" );
                        done = true;
                    }
                    else if( m.special==SPECIAL_WQ_CASTLING || m.special==SPECIAL_BQ_CASTLING )
                    {
                        strcpy( nmove, "O-O-O" );
                        done = true;
                    }
                    break;
                }

                // Nd2 or Nxd2
                case ALG_ND2:
                {
                    if( t == 'x' )
                        sprintf( str_dst, "%cx%c%c", p, FILE(dst), RANK(dst) );
                    else
                        sprintf( str_dst, "%c%c%c", p, FILE(dst), RANK(dst) );
                    if( i >= 0 )
                    {
                        if( 0 == strcmp(nmove,compare) )
                            matches++;
                    }
                    break;
                }

                // Nbd2 or Nbxd2    
                case ALG_NBD2:
                {
                    if( t == 'x' )
                        sprintf( str_dst, "%c%cx%c%c", p, FILE(src), FILE(dst), RANK(dst) );
                    else
                        sprintf( str_dst, "%c%c%c%c", p, FILE(src), FILE(dst), RANK(dst) );
                    if( i >= 0 )
                    {
                        if( 0 == strcmp(nmove,compare) )
                            matches++;
                    }
                    break;
                }

                // N1d2 or N1xd2
                case ALG_N1D2:
                {
                    if( t == 'x' )
                        sprintf( str_dst, "%c%cx%c%c", p, RANK(src), FILE(dst), RANK(dst) );
                    else
                        sprintf( str_dst, "%c%c%c%c", p, RANK(src), FILE(dst), RANK(dst) );
                    if( i >= 0 )
                    {
                        if( 0 == strcmp(nmove,compare) )
                            matches++;
                    }
                    break;
                }

                //  Nb1d2 or Nb1xd2
                case ALG_NB1D2:
                {
                    done = true;
                    if( t == 'x' )
                        sprintf( nmove, "%c%c%cx%c%c", p, FILE(src), RANK(src), FILE(dst), RANK(dst) );
                    else
                        sprintf( nmove, "%c%c%c%c%c", p, FILE(src), RANK(src), FILE(dst), RANK(dst) );
                    break;
                }
            }
        }   // end loop for all legal moves with given algorithm

        // If it's a looping algorithm and only one move matches nmove, we're done
        if( do_loop && matches==1 )
            done = true;
    }   // end loop for all algorithms
    if( append )
    {
        char *s = strchr(nmove,'\0');
        *s++ = append;
        *s = '\0';
    }
    return nmove;
}

/****************************************************************************
 * Convert to terse string eg "e7e8q"
 ****************************************************************************/
std::string Move::TerseOut()
{    
    char tmove[6];
    if( src == dst )   // null move should be "0000" according to UCI spec
    {
        tmove[0] = '0';
        tmove[1] = '0';
        tmove[2] = '0';
        tmove[3] = '0';
        tmove[4] = '\0';
    }
    else
    {
        tmove[0] = FILE(src);
        tmove[1] = RANK(src);
        tmove[2] = FILE(dst);
        tmove[3] = RANK(dst);
        if( special == SPECIAL_PROMOTION_QUEEN )
            tmove[4] = 'q';
        else if( special == SPECIAL_PROMOTION_ROOK )
            tmove[4] = 'r';
        else if( special == SPECIAL_PROMOTION_BISHOP )
            tmove[4] = 'b';
        else if( special == SPECIAL_PROMOTION_KNIGHT )
            tmove[4] = 'n';
        else
            tmove[4] = '\0';
        tmove[5] = '\0';
    }
    return tmove;
}    

