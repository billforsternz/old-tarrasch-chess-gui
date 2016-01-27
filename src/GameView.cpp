/****************************************************************************
 * A complete view of a game's variations, built from a MoveTree
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#include "ChessRules.h"
#include "GameLogic.h"
#include "Lang.h"
#include "DebugPrintf.h"
#include "Objects.h"
#include "Repository.h"
#include "DebugPrintf.h"
#include "GameView.h"
using namespace std;
using namespace thc;

void GameView::Build( std::string &result, MoveTree *tree, ChessPosition &start_position )
{
    this->result = result;
    final_position_node = NULL;
    final_position_txt = "Initial position";
    final_position.Init();
    this->tree = tree;
    expansion.clear();
    level = -1;
    offset = 0;
    newline = true;
    comment = false;
    this->start_position = start_position;
    cr = start_position;
/*  if( tree->game_move.comment.length() )
    {
        GameViewElement gve;
        gve.type    = COMMENT;
        gve.level   = level;
        gve.offset1 = offset;
        offset += ( (expansion.size()?2:1) + tree->game_move.comment.length());
        gve.offset2 = offset;
        gve.node    = tree;
        expansion.push_back(gve);
        comment = true;
    } */
    Crawler( tree, true, false );

    // Save a copy of the language as it was when we built the view
    memcpy( language_lookup, LangGet(), sizeof(language_lookup) );
}

void GameView::Debug()
{   /*
    if( expansion.size() >= 626 )
    {
        GameViewElement gve = expansion[626];
        if( gve.type==MOVE && gve.offset1==3129 )
        {
            if( gve.node->move.src == a5 )
                DebugPrintf(("no bug",gve.node));
            else
                DebugPrintf(("bug",gve.node));
        }
    } */
}

static const char *nag_array1[] =
{   // no leading space on these so Nf3+! not Nf3+ !
    "",
    "!",     // $1   
    "?",     // $2   
    "!!",    // $3   
    "??",    // $4   
    "!?",    // $5   
    "?!",    // $6   
    "",      // $7   
    "",      // $8   
    "??"     // $9   
};

static const char *nag_array2[] =
{   // leading space on these so Nf3+! +- not Nf3+!+-
    //  apart from anything else this helps us PgnParse() our comments properly
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    " =",     // $10  
    " =",     // $11  
    " =",     // $12  
    "",       // $13  
    " +=",    // $14  
    " =+",    // $15  
    " +/-",   // $16  
    " -/+",   // $17  
    " +-",    // $18  
    " -+",    // $19  
    " +-",    // $20  
    " -+"     // $21
};

/*
static const char *pgn_result_array_screen[] =
{
    "",              // PGN_RESULT_NULL, special value means no associated pgn result
    "",              // PGN_RESULT_LINE (note no "*")
    " 1/2-1/2",      // PGN_RESULT_DRAW
    " 1-0",          // PGN_RESULT_WHITE_WIN
    " 0-1",          // PGN_RESULT_BLACK_WIN
};

static const char *pgn_result_array_file[] =
{
    "",              // PGN_RESULT_NULL, special value means no associated pgn result
    " *",            // PGN_RESULT_LINE (note "*")
    " 1/2-1/2",      // PGN_RESULT_DRAW
    " 1-0",          // PGN_RESULT_WHITE_WIN
    " 0-1",          // PGN_RESULT_BLACK_WIN
}; */

void GameView::Crawler( MoveTree *node, bool move0, bool last_move )
{
    level++;
    bool root = (level==0);

    // No move for root, but it is allowed a comment
    if( root )
    {
        if( node->game_move.comment.length() )
        {
            GameViewElement gve;
            gve.type    = COMMENT;
            gve.level   = level;
            gve.offset1 = offset;
            offset += (node->game_move.comment.length() + 1);
            gve.offset2 = offset;
            gve.node    = node;
            expansion.push_back(gve);
            comment = true;
        }
    }

    // If we have a move (no move for root, only variations)
    else
    {

        // Pre comment
        if( node->game_move.pre_comment.length() )
        {
            GameViewElement gve;
            gve.type    = PRE_COMMENT;
            gve.level   = level;
            gve.offset1 = offset;
            offset += ((expansion.size()?2:1) + node->game_move.pre_comment.length());
            gve.offset2 = offset;
            gve.node    = node;
            expansion.push_back(gve);
        }

        // Move0 (can move cursor to position before move0 of a variation)
        if( move0 )
        {
            GameViewElement gve;
            gve.type    = MOVE0;
            gve.level   = level;
            gve.offset1 = offset;
            gve.offset2 = offset;
            gve.node    = node;
            expansion.push_back(gve);
        }

        // Body of move
        char buf[80];
        bool need_extra_space=true;
        if( move0 || newline || comment )
            sprintf( buf, "%d%s", cr.full_move_count, cr.white?".":"..." );
        else
        {
            if( cr.white )
                sprintf( buf, " %d.", cr.full_move_count );
            else
            {
                strcpy( buf, " " );
                need_extra_space = false;
            }
        }
        newline = false;
        comment = false;
        string intro = buf;
        string move_body = node->game_move.move.NaturalOut(&cr);
        LangOut(move_body);
        string fragment  = intro + move_body;
        string file_view = need_extra_space ? (intro + " " + move_body) : fragment;
        GameViewElement gve;
        gve.nag_value1 = 0;
        if( node->game_move.nag_value1 && 
            node->game_move.nag_value1 < (sizeof(nag_array1)/sizeof(nag_array1[0]))
          )
        {
            gve.nag_value1 = node->game_move.nag_value1;
            fragment.append( nag_array1[node->game_move.nag_value1] );
            char buf[10];
            sprintf(buf," $%d",node->game_move.nag_value1);
            file_view.append(buf);
        }
        gve.nag_value2 = 0;
        if( node->game_move.nag_value2 && 
            node->game_move.nag_value2 < (sizeof(nag_array2)/sizeof(nag_array2[0]))
          )
        {
            gve.nag_value2 = node->game_move.nag_value2;
            fragment.append( nag_array2[node->game_move.nag_value2] );
            char buf[10];
            sprintf(buf," $%d",node->game_move.nag_value2);
            file_view.append(buf);
        }
        if( last_move )
        {
            if( level == 1 )
            {
                final_position = cr;
                final_position.PlayMove( node->game_move.move );
                final_position_node = node;
                sprintf( buf, "Final position after %d%s", cr.full_move_count, cr.white?".":"..." );
                final_position_txt  = buf + move_body;
            }
        }
        gve.type    = MOVE;
        gve.level   = level;
        gve.offset1 = offset;
        offset += (fragment.length());
        gve.offset2 = offset;
        gve.node    = node;
        gve.str     = fragment;
        gve.str_for_file_move_only = file_view;
        expansion.push_back(gve);

        // Comment
        if( node->game_move.comment.length() )
        {
            GameViewElement gve;
            gve.type    = COMMENT;
            gve.level   = level;
            gve.offset1 = offset;
            offset += ((expansion.size()?2:1) + node->game_move.comment.length());
            gve.offset2 = offset;
            gve.node    = node;
            expansion.push_back(gve);
            comment = true;
        }
    }

    // Loop through the variations
    int nbr_vars = node->variations.size();
    if( nbr_vars )
    {
        ChessRules cr_before_move = cr;
        for( int i=0; i<nbr_vars; i++ )
        {

            // If not root variation, add "\n\t\t...\t(" prefix
            if( !root )
            {
                GameViewElement gve;
                gve.type    = NEWLINE;
                newline     = true;
                gve.level   = level+1;
                gve.offset1 = offset;
                offset++;   // "\n"
                gve.offset2 = offset;
                gve.node    = node;
                expansion.push_back(gve);

                gve.type   = START_OF_VARIATION;
                gve.offset1 = offset;
                offset++;   // "("
                gve.offset2 = offset;
                expansion.push_back(gve);
            }

            // Loop through the variation
            vector<MoveTree> &var = node->variations[i];
            int nbr_moves = var.size();
            for( int j=0; j<nbr_moves; j++ )
            {
                Crawler( &var[j], j==0, j==nbr_moves-1 );
            }

            // If not root variation, add ")" or ")\n\t\t...\t" suffix
            if( !root )
            {
                bool after_last_variation = (i==nbr_vars-1);
                GameViewElement gve;
                gve.type    = END_OF_VARIATION;
                gve.level   = level+1;
                gve.offset1 = offset;
                offset++;       // ")"
                gve.offset2 = offset;
                gve.node    = node;
                expansion.push_back(gve);
                if( after_last_variation )
                {
                    gve.type    = NEWLINE;
                    gve.level   = level;
                    gve.offset1 = offset;
                    offset++;       // "\n"
                    gve.offset2 = offset;
                    newline = true;
                    expansion.push_back(gve);
                }
            }

            // Go back to position before variation starts
            cr = cr_before_move;
        }
    }

    // If we have a move, play it
    if( !root )
        cr.PlayMove(node->game_move.move);

    // After all root variations done add END_OF_GAME
    else
    {
        GameViewElement gve;
        gve.type    = END_OF_GAME;
        gve.level   = level+1;
        gve.offset1 = offset;

        // Add space before result if not following comment and not following newline
        //  (it would be END_OF_VARIATION rather than NEWLINE, except that the last
        //   variation's END_OF_VARIATION is followed by NEWLINE)
        int sz=expansion.size();
        bool add_space = (sz==0 ? false : (expansion[sz-1].type!=NEWLINE && expansion[sz-1].type!=COMMENT) );
        std::string temp = (result==""?"*":result); // in file case we make sure we DO write "*"
        gve.str_for_file_move_only = (add_space ? " " + temp : temp); 
        gve.str = "";
        int len = result.length();
        if( len > 1 )   // not "*", in screen case we make sure we DON'T write "*"
            gve.str = (add_space ? " " + result : result); 
        offset += gve.str.length();
        gve.offset2 = offset;
        gve.node    = node;
        expansion.push_back(gve);
    }
    level--;
}

void GameView::Display( unsigned long pos )
{
    wxRichTextCtrl *ctrl = objs.canvas->lb;
    if( ctrl )
    {
        ctrl->Freeze();
        ctrl->BeginSuppressUndo();
        ctrl->Clear();
        ctrl->EndAllStyles();
        ctrl->BeginParagraphSpacing(0, 10);
        bool italic=false;
        bool bold=true;
        ctrl->BeginBold();
        ctrl->BeginLeftIndent(0);
        int nbr = expansion.size();
        for( int i=0; i<nbr; i++ )
        {
            const char *c_str = NULL;
            GameViewElement &gve = expansion[i];
            switch( gve.type )
            {
                case PRE_COMMENT:
                    c_str = gve.node->game_move.pre_comment.c_str();   // fall-thru
                case COMMENT:
                {
                    if( gve.type != PRE_COMMENT )
                        c_str = gve.node->game_move.comment.c_str();
                    ctrl->BeginTextColour(wxColour(0, 0, 255));
                    if( i )
                        ctrl->WriteText( " " );
                    ctrl->WriteText( c_str );
                    ctrl->WriteText( " " );
                    ctrl->EndTextColour();
                    break;
                }
                case MOVE0:
                {
                    break;
                }
                case MOVE:
                {
                    ctrl->WriteText( gve.str.c_str() );
                    break;
                }
                case START_OF_VARIATION:
                {
                    ctrl->WriteText( "(" );
                    break;
                }
                case END_OF_VARIATION:
                {
                    ctrl->WriteText( ")" );
                    break;
                }
                case END_OF_GAME:
                {
                    if( gve.str.length() )
                        ctrl->WriteText( gve.str.c_str() );
                    break;
                }
                case NEWLINE:
                {
                    ctrl->EndLeftIndent();
                    if( bold )
                    {
                        ctrl->EndBold();
                        bold = false;
                    }
                    if( italic )
                    {
                        ctrl->EndItalic();
                        italic = false;
                    }
                    if( gve.level > 0 )
                        ctrl->BeginLeftIndent(20*(gve.level-1));
                    else
                        ctrl->BeginLeftIndent(0);
                    ctrl->Newline();
                    if( gve.level == 1 )
                    {
                        bold = true;
                        ctrl->BeginBold();
                    }
                    else if( gve.level > 2 && !objs.repository->general.m_no_italics )
                    {
                        italic = true;
                        ctrl->BeginItalic();
                    }
                    break;
                }
            }
        }
        if( bold )
            ctrl->EndBold();
        if( italic )
            ctrl->EndItalic();
        ctrl->SetInsertionPoint(pos);
        ctrl->EndSuppressUndo();
        ctrl->Thaw();
        ctrl->ShowPosition(pos);
        ctrl->Update();
    }
}

void GameView::ToString( std::string &str )
{
    int nbr = expansion.size();
    ToString( str, 0, nbr );
}

std::string ReplaceAll( const std::string &in, const std::string &from, const std::string &to )
{
    std::string out = in;
    int pos = out.find(from);
    while( pos != std::string::npos )
    {
        out.replace( pos, from.length(), to );
        pos = out.find( from, pos+to.length() );
    }
    return out;
}

void GameView::ToString( std::string &str, int begin, int end )
{
    str = "";
    int col = 0;
    for( int i=begin; i<end; i++ )
    {
        std::string frag="";
        GameViewElement &gve = expansion[i];
        bool make_comment = false;
        switch( gve.type )
        {
            case PRE_COMMENT:
                frag = gve.node->game_move.pre_comment;   // fall-thru
            case COMMENT:
            {

                // New policy with V2.03c
                //  Only create '{' comments (most chess software doesn't understand ';' comments)
                //  If  "}" appears in comment transform to "|>"    (change *is* restored when reading .pgn)
                //  If  "|>" appears in comment transform to "| >"  (change is *not* restored when reading .pgn)
                if( gve.type != PRE_COMMENT )
                    frag = gve.node->game_move.comment;
                std::string groomed = ReplaceAll(frag,"|>", "| >");
                groomed = ReplaceAll(groomed,"}","|>");
                if( i == begin )
                    frag = "{" + groomed + "} ";
                else
                    frag = " {" + groomed + "} ";
                break;
            }
            case MOVE0:
            {
                break;
            }
            case MOVE:
            {
                frag = gve.str_for_file_move_only;
                LangToEnglish(frag,language_lookup);
                break;
            }
            case START_OF_VARIATION:
            {
                frag = " (";
                break;
            }
            case END_OF_VARIATION:
            {
                frag =  ") ";
                break;
            }
            case END_OF_GAME:
            {
                frag = gve.str_for_file_move_only;
                break;
            }
        }

        // Append with line wrap
        int idx = frag.find(' ');
        bool atomic = (idx==string::npos);  // atomic if no space
        bool splitting = !atomic;           // don't split if atomic
        while( splitting )
        {
            int len = frag.length();
            if( col+len < 80 )
                break;  // don't need to split
            int idx = len-1;

            // Split the fragment in two if possible
            splitting = false;  
            while( idx > 0 )
            {
                if( col+idx<80 && frag[idx]==' ' )
                {
                    string part1 = frag.substr(0,idx);
                    string part2 = frag.substr(idx+1);

                    // In column 0, left trim
                    if( col == 0 )
                    {
                        int idx = part1.find_first_not_of(' ');
                        if( idx!=string::npos && idx>0 )
                            part1 = part1.substr(idx);
                    }   
                    str += part1;
                    str += "\n";
                    if( make_comment )
                        str += ";";
                    col = 0;
                    frag = part2;
                    splitting = true;   // split again if possible
                    break;
                }
                idx--;
            }

            // Maybe couldn't split because first split was beyond col 80
            if( !splitting )
            {
                idx = frag.find(' ');
                if( 0<=idx && idx<78 )
                {
                    splitting = true;
                    str += "\n";
                    if( make_comment )
                        str += ";";
                    col = 0;
                }
            }
        }

        // In column 0, left trim
        if( col == 0 )
        {
            int idx = frag.find_first_not_of(' ');
            if( idx!=string::npos && idx>0 )
                frag = frag.substr(idx);
        }   
        int len = frag.length();
        if( col+len < 80 )
        {
            str += frag;
            col += len;
        }
        else
        {
            str += "\n";
            if( make_comment )
                str += ";";
            str += frag;
            col = len;
        }
        if( make_comment )
            str += "\n";
    }
}

void GameView::ToCommentString( std::string &str )
{
    int nbr = expansion.size();
    ToCommentString( str, 0, nbr );
}

void GameView::ToCommentString( std::string &str, int begin, int end )
{
    str = "";
    for( int i=begin; i<end; i++ )
    {
        std::string frag="";
        GameViewElement &gve = expansion[i];
        switch( gve.type )
        {
            case PRE_COMMENT:
                frag = gve.node->game_move.pre_comment;   // fall-thru
            case COMMENT:
            {
                if( gve.type != PRE_COMMENT )
                    frag = gve.node->game_move.comment;
                if( i == begin )
                    frag = "{" + frag + "} ";
                else
                    frag = " {" + frag + "} ";
                break;
            }
            case MOVE0:
            {
                break;
            }
            case MOVE:
            {
                frag = gve.str;
                break;
            }
            case START_OF_VARIATION:
            {
                frag = " (";
                break;
            }
            case END_OF_VARIATION:
            {
                frag =  ") ";
                break;
            }
            case END_OF_GAME:
            {
                frag = gve.str;
                break;
            }
        }
        if( str.length()>0 && frag!="" && frag[0]!=' ' )
        {
            if( str[str.length()-1] != ' ' )
                str += " ";
        }
        str += frag;
    }
}

unsigned long GameView::NavigationKey( unsigned long pos, NAVIGATION_KEY nk ) 
{
    unsigned save_pos=pos;
    bool forward=true;              // down, right
    bool skip_to_mainline=true;     // right, left
    if( nk==NK_UP || nk==NK_LEFT || nk==NK_HOME || nk==NK_PGUP )
        forward = false;
    if( nk==NK_LEFT || nk==NK_RIGHT )
        skip_to_mainline = false;
    int nbr = expansion.size();
    for( int repeat=0; repeat < (nk==NK_PGUP||nk==NK_PGDOWN ? 4 : 1); repeat++ )
    {
        bool found=false;
        int level_original=0, level=0;
        int start = forward ? 0   : nbr-1;
        int end   = forward ? nbr : -1;
        int iter  = forward ? +1  : -1;
        bool justfound=true;    // this is a truly horrible hack
        for( int i=start; i!=end; i+=iter )
        {
            GameViewElement &gve = expansion[i];
            if( !found )
            {
                if( gve.offset1<=pos && pos<=gve.offset2 )
                {
                    found = true;
                    level_original = level = gve.level;
                }
            }
            else
            {
                if( nk==NK_HOME )
                {
                    if( gve.type==END_OF_VARIATION )
                        justfound = false;
                    if( justfound && gve.type==MOVE && gve.level!=level_original )
                        level_original = gve.level;  // adjust, we might have had wrong
                                                     //  level because of NEWLINE
                    if( gve.level==level_original && gve.type==MOVE0 )
                        save_pos = gve.offset2;
                    if( i+iter == end )
                    {
                        pos = save_pos;
                        break;
                    }
                    else if( gve.level==level_original && gve.type==START_OF_VARIATION )
                    {
                        pos = save_pos;
                        break;
                    }
                }
                else if( nk==NK_END )
                {
                    if( gve.level==level_original && gve.type==MOVE )
                        save_pos = gve.offset2;
                    if( i+iter == end )
                    {
                        pos = save_pos;
                        break;
                    }
                    else if( gve.level==level_original && gve.type==END_OF_VARIATION )
                    {
                        pos = save_pos;
                        break;
                    }
                }
                else
                {
                    if( gve.type  == (forward?START_OF_VARIATION:END_OF_VARIATION) &&
                        gve.level == level_original
                    )
                        level--;
                    if( gve.type==END_OF_GAME && forward )
                    {
                        pos = gve.offset2;
                        break;
                    }
                    if( gve.type==MOVE0 && gve.offset1!=pos )
                    {
                        if( !skip_to_mainline || gve.level<=level )
                        {
                            pos = gve.offset1;
                            break;
                        }
                    }
                    if( gve.type==MOVE && gve.offset2!=pos )
                    {
                        if( !skip_to_mainline || gve.level<=level )
                        {
                            pos = gve.offset2;
                            break;
                        }
                    }
                }
            }
        }
    }
    return pos;
}

int GameView::GetInternalOffset( MoveTree *move )
{
    int ret=0;
    int nbr = expansion.size();
    for( int i=0; i<nbr; i++ )
    {
        GameViewElement gve = expansion[i];
        if( gve.type==MOVE && gve.node==move )
        {
            ret = i;
            break;
        }
    }
    return ret;
}

int GameView::GetInternalOffsetEndOfVariation( int start )
{
    int nbr = expansion.size();
    GameViewElement &gve = expansion[start];
    int level = gve.level;
    int end = nbr;
    for( int i=start; i<nbr; i++ )
    {
        GameViewElement &gve = expansion[i];
        if( gve.type==END_OF_VARIATION && gve.level==level )
        {
            end = i;
            break;
        }
        if( gve.type == END_OF_GAME )
        {
            end = i;
            break;
        }
    }
    return end;
}

MoveTree *GameView::Locate( unsigned long pos, ChessRules &cr, string &title, bool &at_move0 )
{
    MoveTree *found = NULL;
    at_move0 = false;
    title = "Initial position";
    bool have_a_move=false;
    GameViewElement most_recent_move;
    most_recent_move.type = END_OF_GAME;
    int nbr = expansion.size();
    for( int i=0; i<nbr; i++ )
    {
        GameViewElement gve = expansion[i];
        bool in_range   = (gve.offset1<=pos && pos<=gve.offset2);
        bool its_a_move = (gve.type==MOVE0 || gve.type==MOVE);
        bool gone_past  = (pos<gve.offset1);
        bool not_yet    = (pos>gve.offset2);

        // Special case 1, after result
        if( in_range && gve.type==END_OF_GAME && final_position_node )
        {
            title = final_position_txt;
            cr = final_position;
            found = final_position_node;
        }

        // Special case 2, in lone comment
        else if( in_range && i==0 && nbr==2 && gve.type==COMMENT )
        {
            found = gve.node;
            break;
        }
        else
        {
            if( not_yet && gve.type==MOVE )
            {
                most_recent_move = gve;
                have_a_move = true;
            }
            if( gone_past || (in_range&&its_a_move) )
            {
                #if 0
                if( gone_past )
                    gve = most_recent_move;
                else
                    have_a_move = true;
                if( gve.type==MOVE0 && gve.offset1==pos )
                    at_move0 = true;
                #else
                if( gve.type==MOVE0 && pos<=gve.offset1 )
                {
                    at_move0 = true;
                    have_a_move = true;
                }
                else
                {
                    if( gone_past )
                        gve = most_recent_move;
                    else
                        have_a_move = true;
                }
                #endif
                if( have_a_move )
                {
                    int ivar, imove;
                    MoveTree *parent = tree->Parent( gve.node, cr, ivar, imove );
                    if( parent )
                    {
                        found = gve.node;
                        vector<MoveTree> &variation = parent->variations[ivar];
                        int i=0;
                        for( i=0; i<imove; i++ )
                            cr.PlayMove( variation[i].game_move.move );
                        std::string nmove =  variation[i].game_move.move.NaturalOut(&cr);
                        LangOut(nmove);
                        char buf[80];
                        sprintf( buf, "%s %d%s%s",
                                at_move0?"Position before":"Position after",
                                cr.full_move_count,
                                cr.white?".":"...",
                                nmove.c_str() );
                        title = buf;
                        if( !at_move0 )
                            cr.PlayMove( variation[i].game_move.move );
                    }
                }
                break;
            }
        }
    }
    return found;
}

unsigned long GameView::FindMove0()
{
    int nbr = expansion.size();
    for( int i=0; i<nbr; i++ )
    {
        GameViewElement gve = expansion[i];
        if( gve.type == MOVE0 )
            return gve.offset1;
    }
    return 0;
}

unsigned long GameView::FindEnd()
{
    unsigned long offset=0;
    int nbr = expansion.size();
    if( nbr > 0 )
    {
        GameViewElement gve = expansion[nbr-1];
        offset = gve.offset2;
    }
    return offset;
}

bool GameView::IsAtEnd( unsigned long pos )
{
    bool at_end=true;
    int nbr = expansion.size();
    for( int i=0; i<nbr; i++ )
    {
        GameViewElement gve = expansion[i];
        // bool in_range   = (gve.offset1<=pos && pos<=gve.offset2);
        bool its_a_move = (gve.type==MOVE0 || gve.type==MOVE);
        bool gone_past  = (pos<gve.offset1);
        //bool not_yet    = (pos>gve.offset2);
        if( gone_past && its_a_move )
        {
            at_end = false;
            break;
        }
    }
    return at_end;
}

unsigned long GameView::GetMoveOffset( MoveTree *node )
{
    unsigned long pos=0;
    int nbr = expansion.size();
    for( int i=0; i<nbr; i++ )
    {
        GameViewElement gve = expansion[i];
        if( gve.type==MOVE && gve.node==node )
        {
            pos = gve.offset2;
            break;
        }
    }
    return pos;
}

bool GameView::GetOffsetWithinComment( unsigned long pos, unsigned long &pos_within_comment )
{
    bool found = false;
    int nbr = expansion.size();
    for( int i=0; !found && i<nbr; i++ )
    {
        GameViewElement &gve = expansion[i];
        if( gve.offset1<pos && pos<gve.offset2 )
        {
            if( gve.type==COMMENT ) // || gve.type==PRE_COMMENT )
            {
                found = true;
                pos_within_comment = pos-gve.offset1-1;
            }
        }
    }
    return found;
}

bool GameView::CommentEdit( wxRichTextCtrl *ctrl, std::string &txt_to_insert, long keycode )
{
    bool used = false;
    unsigned long pos = gl->atom.GetInsertionPoint();
    unsigned long orig_pos = pos;
    int nbr = expansion.size();
    if( 0x20<=keycode && keycode<=0xff && keycode!=0x7f /*delete*/ )
    {
        char c = (char)keycode;
        txt_to_insert = c;
    }
    for( int i=0; i<nbr; i++ )
    {
        GameViewElement &gve = expansion[i];
        if( gve.offset1<=pos && pos<=gve.offset2 )
        {
            if( gve.type==COMMENT || gve.type==PRE_COMMENT )
            {
                unsigned long home = gve.offset1+1; // 1 after leading ' '
                unsigned long end  = gve.offset2-1; // 1 before trailing ' '
                if( home == 1 )
                    home = 0;   // no leading space at offset 0
                if( home<=pos && pos<=end )
                {
                    int offset_within_comment = pos - home;
                    switch( keycode )
                    {
                        case WXK_BACK:
                        {
                            if( pos > home ) // can't backspace from home
                            {
                                bool empty;
                                if( gve.type == COMMENT )
                                {
                                    gve.node->game_move.comment.erase( offset_within_comment-1, 1 );
                                    empty = (gve.node->game_move.comment.length()==0);
                                }
                                else
                                {
                                    gve.node->game_move.pre_comment.erase( offset_within_comment-1, 1 );
                                    empty = (gve.node->game_move.pre_comment.length()==0);
                                }
                                if( home == 0 )
                                    pos--;
                                else
                                    pos -= (empty?2:1);
                                comment_edited = true;
                                used = true;
                            }
                            break;
                        }
                        case WXK_DELETE:
                        {
                            if( pos < end ) // can't delete at end
                            {
                                bool empty;
                                if( gve.type == COMMENT )
                                {
                                    gve.node->game_move.comment.erase( offset_within_comment, 1 );
                                    empty = (gve.node->game_move.comment.length()==0);
                                }
                                else
                                {
                                    gve.node->game_move.pre_comment.erase( offset_within_comment, 1 );
                                    empty = (gve.node->game_move.pre_comment.length()==0);
                                }
                                comment_edited = true;
                                used = true;
                            }
                            break;
                        }
                        case WXK_LEFT:
                        {
                            if( pos > home )
                            {
                                pos--;
                                used = true;
                            }
                            break;
                        }
                        case WXK_RIGHT:
                        {
                            if( pos < end )
                            {
                                pos++;
                                used = true;
                            }
                            break;
                        }
                        default:
                        {
                            if( txt_to_insert.length() )
                            {
                                if( gve.type == COMMENT )
                                    gve.node->game_move.comment.insert( offset_within_comment, txt_to_insert );
                                else
                                    gve.node->game_move.pre_comment.insert( offset_within_comment, txt_to_insert );
                                pos += txt_to_insert.length();
                                comment_edited = true;
                                used = true;
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    if( !used &&  txt_to_insert.length() )
    {
        for( int i=0; i<nbr; i++ )
        {
            GameViewElement &gve = expansion[i];
            if( gve.offset1<=pos && pos<=gve.offset2 )
            {
                bool create_lone_comment = (gve.type==END_OF_GAME&&pos==0);
                if( gve.type==MOVE || gve.type==MOVE0 || create_lone_comment )
                {
                    comment_edited = true;
                    if( gve.type == MOVE0 )
                        gve.node->game_move.pre_comment += txt_to_insert;
                    else
                        gve.node->game_move.comment += txt_to_insert;
                    if( create_lone_comment )
                    {
                        used = true;
                        pos += txt_to_insert.length();
                    }
                    else if( gve.type == MOVE0 )
                    {
                        used = true;
                        pos += (txt_to_insert.length() + (pos==0?0:1));
                    }
                    else
                    {
                        used = true;
                        pos = gve.offset2 + gve.node->game_move.comment.length() + 1;
                    }
                    break;
                }
            }
        }
    }
    if( comment_edited )
    {
        gl->gd.Rebuild();
        gl->atom.Display( pos );
        gl->atom.Undo();
    }
    else if( orig_pos != pos )
    {
        gl->atom.SetInsertionPoint(pos);
    }
    return used;
}

bool GameView::IsInComment( wxRichTextCtrl *ctrl )
{
    bool pos_in_comment=false;
    long pos;
    pos = gl->atom.GetInsertionPoint();
    int nbr = expansion.size();
    for( int i=0; i<nbr; i++ )
    {
        GameViewElement &gve = expansion[i];
        if( gve.type==COMMENT || gve.type==PRE_COMMENT )
        {
            if( gve.offset1<pos && pos<gve.offset2 )
            {
                pos_in_comment = true;
                break;
            }
        }
    }
    return pos_in_comment;
}

bool GameView::IsSelectionInComment( wxRichTextCtrl *ctrl )
{
    bool selection_in_comment=false;
    long pos1, pos2;
    ctrl->GetSelection( &pos1, &pos2 );
    if( pos2 > pos1 )
    {
        int nbr = expansion.size();
        for( int i=0; i<nbr; i++ )
        {
            GameViewElement &gve = expansion[i];
            if( gve.type==COMMENT || gve.type==PRE_COMMENT )
            {
                if( gve.offset1<=pos1 && pos2<gve.offset2 )
                {
                    selection_in_comment = true;
                    break;
                }
            }
        }
    }
    return selection_in_comment;
}

void GameView::DeleteSelection( wxRichTextCtrl *ctrl )
{
    unsigned long pos = ctrl->GetInsertionPoint();
    unsigned long loc;
    long pos1, pos2;
    ctrl->GetSelection( &pos1, &pos2 );
    ctrl->SetSelection(pos,pos);
    int nbr = expansion.size();
    int found=0;
    for( int i=0; i<nbr; i++ )
    {
        GameViewElement &gve = expansion[i];
        if( gve.offset1<=pos && pos<=gve.offset2 && gve.offset1<=pos1 && pos2<=gve.offset2 )
        {
            found = i;
            if( gve.type==COMMENT || gve.type==PRE_COMMENT )
            {
                unsigned long home = gve.offset1+1; // 1 after leading ' '
                unsigned long end  = gve.offset2-1; // 1 before trailing ' '
                if( home == 1 )
                    home = 0;   // no leading space at offset 0
                if( home<=pos && pos<=end && home<=pos1 && pos2<=end  )
                {

                    // First delete, possibly the whole comment
                    int offset_within_comment = pos1 - home;
                    bool empty;
                    if( gve.type == COMMENT )
                    {
                        gve.node->game_move.comment.erase( offset_within_comment, pos2-pos1 );
                        empty = (gve.node->game_move.comment.length()==0);
                    }
                    else
                    {
                        gve.node->game_move.pre_comment.erase( offset_within_comment, pos2-pos1 );
                        empty = (gve.node->game_move.pre_comment.length()==0);
                    }
                    if( home == 0 )
                        pos = empty?home:pos1, empty?end+1:pos2;
                    else
                        pos = empty?home-1:pos1, empty?end+1:pos2;
                    gl->gd.Rebuild();
                    gl->atom.Redisplay( pos );
                    gl->atom.Undo();
                }
            }
            break;
        }
    }
//  ctrl->SetInsertionPoint(pos);   // todo/later remove this and get caller's to use returned pos
//  return pos;
/*
    if( offset_adjust )
    {
        nbr = expansion.size();
        for( int i=nbr-1; i>=found; i-- )
        {
            GameViewElement &gve = expansion[i];
            if( i != found )
            {
                gve.offset1 += offset_adjust;
                gve.offset2 += offset_adjust;
            }
            else
            {
                gve.offset2 += offset_adjust;
                if( gve.type==COMMENT && gve.offset1==gve.offset2 )
                {
                    if( gve.node->game_move.comment.length() == 0 )
                    {
                        vector<GameViewElement>::iterator it = expansion.begin() + i;
                        expansion.erase(it);
                    }
                }
                else if( gve.type==PRE_COMMENT && gve.offset1==gve.offset2 )
                {
                    if( gve.node->game_move.pre_comment.length() == 0 )
                    {
                        vector<GameViewElement>::iterator it = expansion.begin() + i;
                        expansion.erase(it);
                    }
                }
            }
        }
    } */
}


