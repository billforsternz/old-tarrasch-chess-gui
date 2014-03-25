/****************************************************************************
 * Games Cache - Abstracted interface to  a list of games
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#include "wx/wx.h"
#include "wx/valtext.h"
#include "wx/valgen.h"
#include "wx/filename.h"
#include "wx/filefn.h"
#include "wx/listctrl.h"
#include "Appdefs.h"
#include "DebugPrintf.h"
#include "ChessPosition.h"
#include "GameLogic.h"
#include "Objects.h"
#include "PgnFiles.h"
#include "Lang.h"
#include "GamesCache.h"
#include <stdio.h>
using namespace std;

bool GamesCache::Load(std::string &filename )
{
    file_irrevocably_modified = false;
    resume_previous_window = false;
    loaded = false;
    FILE *pgn_file = objs.gl->pf.OpenRead( filename, pgn_handle );
    if( pgn_file )
    {
        pgn_filename = filename;
        gds.clear();
        loaded = Load(pgn_file);
        int sz = gds.size();
        if( sz > 0 )
        {
            fseek(pgn_file,0,SEEK_END);
            long filelen = ftell(pgn_file);
            fseek(pgn_file,0,SEEK_SET);
            for( int i=0; i<sz; i++ )
                gds[i].fposn3 = (i+1==sz ? filelen : gds[i+1].fposn1);
        }
        objs.gl->pf.Close(NULL);  // clipboard only needed after ReopenModify()
    }
    return loaded;
}

bool GamesCache::IsLoaded()
{
    return loaded;
}

bool GamesCache::IsSynced()
{
    return( IsLoaded() && objs.gl->pf.IsAvailable(pgn_handle) );
}

bool GamesCache::Load( FILE *pgn_file )
{
    file_irrevocably_modified = false;
    bool ok=true;
    GameDocument gd;
    game_nbr=0;
    int fposn_base = 0;
    const int BUFSIZE=10000;
    char buf[BUFSIZE+5];
    int req;
    int len = fread(buf,1,req=BUFSIZE,pgn_file);
    bool nomore = (len!=req);
    const char *beg = buf;
    const char *end = buf + (len>=0?len:0);
    const char *s = beg;
    bool done = (s>=end);
    while( !done )
    {
        bool have_line=false;
        const char *line = s;
        while( s < end )
        {
            if( *s=='\r' || *s=='\n' )
            {
                have_line = true;
                break;
            }
            s++;
        }
        if( !have_line )
        {
            // ragged, incomplete line
            if( nomore )
            {
                done = true;
                if( s > line )
                    have_line = true;   // accept ragged final line with no "\n"
            }
            else
            {

                // move the incomplete line to the start of buffer and refill
                fposn_base += (line-buf);
                const char *src = line;
                char *dst = buf;
                int count = s-line;
                req = BUFSIZE-count;
                for( int i=0; i<count; i++ )
                    *dst++ = *src++;
                len = fread(buf+count,1,req,pgn_file);
                nomore = (len!=req);
                s = buf;
                end = buf + count + (len>=0?len:0);
            }    
        }
        if( have_line )
        {
            char *make_c_str = (char *)s;
            char save = *make_c_str;
            *make_c_str = '\0';
            LoadLine( gd, fposn_base +(line-buf), line );
            *make_c_str = save;
            while( s < end )
            {
                if( *s!='\r' && *s!='\n' )
                    break;
                s++;
            }
        }
    }
    return ok;
}
            
void GamesCache::LoadLine( GameDocument &gd, int fposn, const char *line )
{            
    const char *s = line;
    while( *s==' ' || *s=='\t' )
        s++;
    switch( state )
    {
        case HEADER:
        {
            if( *s == '[' )
                Tagline(gd,s);
            else if( *s )
            {
                state = INGAME;
                gd.fposn2 = fposn;
                gd.game_nbr = game_nbr++;
                gd.moves_txt = s;
                LangLine( gd.moves_txt, NULL, LangGet() );  // English -> Current language
                int len = gd.moves_txt.length();
                if( len>=1 && gd.moves_txt[len-1] == '*' )
                    gd.moves_txt = gd.moves_txt.substr(0,len-1);
                gd.pgn_handle = pgn_handle;
                gds.push_back( gd );
                thc::ChessPosition initial_position;
                gd.Init(initial_position);
            }
            break;
        }
        case STARTUP:
        case INGAME:
        {
            if( *s == '[' )
            {
                if( Tagline(gd,s) )
                {
                    state = HEADER;
                    gd.fposn1 = fposn;
                }
            }
            break;
        }
    }
}

// Check whether text s is a valid header, return true if it is,
//  add info to a GameDocument, optionally clearing it first
bool GamesCache::Tagline( GameDocument &gd,  const char *s )
{
    const char *tag_begin, *tag_end, *val_begin, *val_end;
    bool is_header = false;

    // Skip '['
    s++;

    // Skip whitespace
    while( *s==' ' || *s=='\t' )
        s++;

    // Is there a tag before a leading " ?
    tag_begin = s;
    bool tag=false;
    while( *s && *s!=']' && *s!=' ' && *s!='\t' && *s!='\"' )
    {
        tag = true;    // at least 1 non-whitespace
        s++;
    }
    tag_end = s;
    if( tag )
    {

        // Make sure there is whitespace, but skip it
        tag = false;
        while( *s==' ' || *s=='\t' )
        {
            tag = true;  // at least 1 whitespace
            s++;
        }
    }

    // If there is a tag, then whitespace, then a leading "
    if( tag && *s=='\"')
    {
        s++;
        val_begin = s;

        // Skip to 2nd " or end of string
        while( *s && *s!='\"' )
            s++;

        // If we have a 2nd " then we have a tag and a val, i.e. a header
        if( *s == '\"' )        
        {
            is_header = true;
            val_end = s;
            string tag(tag_begin,tag_end-tag_begin);
            string val(val_begin,val_end-val_begin);
            if( tag == "White" )
                gd.white = val;
            if( tag == "Black" )
                gd.black = val;
            if( tag == "Event" )
                gd.event = val;
            if( tag == "Site" )
                gd.site = val;
            if( tag == "Date" )
                gd.date = val;
            if( tag == "Round" )
                gd.round = val;
            if( tag == "Result" )
                gd.result = val;
            if( tag == "ECO" )
                gd.eco = val;
            if( tag == "WhiteElo" )
                gd.white_elo = val;
            if( tag == "BlackElo" )
                gd.black_elo = val;
            if( tag == "FEN" )
                gd.start_position.Forsyth(val.c_str());
        }
    }
    return is_header;
}

// Create a new file
bool GamesCache::FileCreate( std::string &filename, GameDocument &gd )
{
    pgn_filename = filename;
    resume_previous_window = false;
    loaded = false;

    gds.clear();
    gd.in_memory = true;
    gd.pgn_handle = 0;
    gds.push_back(gd);
    FILE *pgn_out = objs.gl->pf.OpenCreate( pgn_filename, pgn_handle );
    if( pgn_out )
    {
        loaded = true;
        FileSaveInner( NULL, NULL, pgn_out );
        objs.gl->pf.Close(NULL);    // close all handles (gc_clipboard
                                    //  only needed for ReopenModify())
    }
    return loaded;
}

// Save the existing file
void GamesCache::FileSave( GamesCache *gc_clipboard )
{
    FILE *pgn_in;
    FILE *pgn_out;
    bool ok = objs.gl->pf.ReopenModify( pgn_handle, pgn_in, pgn_out );
    if( ok )
    {
        FileSaveInner( gc_clipboard, pgn_in, pgn_out );
        objs.gl->pf.Close( gc_clipboard );    // close all handles
    }
}

// Save the existing file with a new name
void GamesCache::FileSaveAs( std::string &filename, GamesCache *gc_clipboard )
{
    FILE *pgn_in;
    FILE *pgn_out;
    bool ok = objs.gl->pf.ReopenCopy( pgn_handle, filename, pgn_in, pgn_out );
    if( ok )
    {
        pgn_filename = filename;
        FileSaveInner( gc_clipboard, pgn_in, pgn_out );
        objs.gl->pf.Close( gc_clipboard );    // close all handles
    }
}

// Save all as a new file
void GamesCache::FileSaveAllAsAFile( std::string &filename )
{
    renumber = true;   // save session or clipboard in order files are listed
    FILE *pgn_out = objs.gl->pf.OpenCreate( filename, pgn_handle );
    if( pgn_out )
    {
        FileSaveInner( NULL, NULL, pgn_out );
        objs.gl->pf.Close(NULL);    // close all handles (gc_clipboard
                                    //  only needed for ReopenModify())
    }
}

// Save common
void GamesCache::FileSaveInner( GamesCache *gc_clipboard, FILE *pgn_in, FILE *pgn_out )
{
    char *buf;
    int buflen=100;
    file_irrevocably_modified = false;
    buf = new char [buflen];
    int gds_nbr = gds.size();
    // Sort by game_nbr, save current order in .game_nbr for restore later
    if( !renumber )
    {
        for( int i=0; i<gds_nbr; i++ )    
        {   
            gds[i].sort_idx = gds[i].game_nbr;
            gds[i].game_nbr = i;
        }
        sort( gds.begin(), gds.end() );
    }
    long posn=0;
    for( int i=0; i<gds_nbr; i++ )    
    {   
        gds[i].modified = false;
        bool replace_game_details = gds[i].game_details_edited || gds[i].pgn_handle==0;
        gds[i].game_details_edited = false;
        bool replace_moves = gds[i].in_memory || gds[i].pgn_handle==0;
        bool no_replacements = (!replace_moves && !replace_game_details);
        bool replace_both    = (replace_moves && replace_game_details);

        //
        //   fposn1
        //     [game details]
        //   fposn2
        //     [game moves]
        //   fposn3
        //
        long fposn1 = gds[i].fposn1;
        long fposn2 = gds[i].fposn2;
        long fposn3 = gds[i].fposn3;
        long len;
        bool same_file = (gds[i].pgn_handle==pgn_handle);
        FILE *pgn = pgn_in;
        if( !same_file && !replace_both )
        {
            pgn = objs.gl->pf.ReopenRead( gds[i].pgn_handle );
            if( !pgn )
                continue; // whoops, can't read the game
        }
        gds[i].pgn_handle = pgn_handle;  // irrespective of where it came from, now this
                                         //  game is in this file
        gds[i].fposn1 = posn;            
                                            
        if( no_replacements )
        {
            len = fposn3-fposn1;
            fseek(pgn,fposn1,SEEK_SET);
            while( len >= buflen )
            {
                delete[] buf;
                buflen *= 2;
                buf = new char [buflen];
            }
            fread(buf,1,len,pgn);
            fwrite(buf,1,len,pgn_out);
            gds[i].fposn2 = posn + (fposn2-fposn1);
            posn += len;
            gds[i].fposn3 = posn;
        }
        else
        {
            if( replace_game_details )
            {
                std::string str;
                gds[i].ToFileTxtGameDetails( str );
                fwrite(str.c_str(),1,str.length(),pgn_out);
                posn += str.length();
            }
            else
            {
                len = fposn2-fposn1;
                fseek(pgn,fposn1,SEEK_SET);
                while( len >= buflen )
                {
                    delete[] buf;
                    buflen *= 2;
                    buf = new char [buflen];
                }
                fread(buf,1,len,pgn);
                fwrite(buf,1,len,pgn_out);
                posn += len;
            }
            gds[i].fposn2 = posn;
            if( replace_moves )
            {
                std::string str;
                gds[i].ToFileTxtGameBody( str );
                fwrite(str.c_str(),1,str.length(),pgn_out);
                posn += str.length();
            }
            else
            {
                len = fposn3-fposn2;
                fseek(pgn,fposn2,SEEK_SET);
                while( len >= buflen )
                {
                    delete[] buf;
                    buflen *= 2;
                    buf = new char [buflen];
                }
                fread(buf,1,len,pgn);
                fwrite(buf,1,len,pgn_out);
                posn += len;
            }
            gds[i].fposn3 = posn;

            // Fix a nasty bug in T2 up to and including V2.01. A later PutBackDocument()
            //  was overwriting the correctly calculated values of fposn1 etc. with stale
            //  values. Fix is to update those stale values here.
            if( gds[i].current_game )
            {
                objs.gl->gd.fposn1 = gds[i].fposn1;
                objs.gl->gd.fposn2 = gds[i].fposn2;
                objs.gl->gd.fposn3 = gds[i].fposn3;
                objs.gl->gd.pgn_handle = gds[i].pgn_handle;
            }
        }
    }

    // Restore sort order .game_nbr field is restored to its original value
    if( !renumber )
    {
        for( int i=0; i<gds_nbr; i++ )    
        {   
            int temp = gds[i].sort_idx;
            gds[i].sort_idx = gds[i].game_nbr;
            gds[i].game_nbr = temp;
        }
        sort( gds.begin(), gds.end() );
    }
    delete[] buf;
}


void GamesCache::Debug( const char *intro_message )
{
    DebugPrintf(( "Cache dump>%s\n", intro_message ));
    int gds_nbr = gds.size();
    for( int i=0; i<gds_nbr; i++ )    
    {   
        GameDocument doc = gds[i];
        DebugPrintf(( "game_nbr=%d, white=%s, moves_txt=%s, pgn_handle=%d\n",
                        doc.game_nbr,
                        doc.white.c_str(),
                        doc.moves_txt.c_str(),
                        doc.pgn_handle
                   ));

        DebugPrintf(( " modified=%s, in_memory=%s, game_details_edited=%s\n",
                        doc.modified ? "true":"false",
                        doc.in_memory ? "true":"false",
                        doc.game_details_edited ? "true":"false"
                   ));

        DebugPrintf(( " current_game=%s, selected=%s, focus=%s\n",
                        doc.current_game ? "true":"false",
                        doc.selected ? "true":"false",
                        doc.focus ? "true":"false"
                   ));
    }
}
