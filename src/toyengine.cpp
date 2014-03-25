/****************************************************************************
 * A simple UCI engine to demonstrate the Tarrasch GUI
 ****************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include "ChessEngine.h"
#include "DebugPrintf.h"
using namespace std;
using namespace thc;

//-- preferences
//#define CREATE_LOG
//#define JUST_TESTING
#define VERSION "Beta V0.908"
#define ENGINE_NAME "Tarrasch Toy Engine"

FILE *file_log;
static ChessEngine chess_engine;
bool process( const char *buf );
const char *cmd_uci();
const char *cmd_isready();
const char *cmd_stop();
const char *cmd_go( const char *cmd );
const char *cmd_go_infinite();
void        cmd_multipv( const char *cmd );
const char *cmd_position( const char *cmd );
const char *log_time();
static bool is_new_game();
void DebugPrintfInner( const char *fmt, ... )
{
	static char buf[1024];
	va_list stk;
	va_start( stk, fmt );
	vsnprintf( buf, sizeof(buf)-2, fmt, stk );
    if( file_log )
    {
	    fprintf( file_log, buf );
        fflush(file_log);
    	//fprintf( stdout, buf );
    }
    //else
    //	fprintf( stdout, buf );
	va_end(stk);
}


HANDLE hSem;
#define BIGBUF 8190
static char cmdline[8][BIGBUF+2];
static int cmd_put;
static int cmd_get;
extern bool gbl_stop;
static Move bestmove;

void read_stdin()
{
    bool quit=false;
    while(!quit)
    {
        #if 0
        static int test;
        if( test == 0 )
        {
            strcpy( &cmdline[cmd_put][0], "uci\n" );
            test++;
        }
        else if( test == 1 )
        {
            strcpy( &cmdline[cmd_put][0], "position fen r1b2rk1/1p1n1ppp/pBqp4/3Np1b1/2B1P3/1Q3P2/PPP3PP/2KR3R w - - 8 16\n" );
            test++;
        }
        else if( test == 2 )
        {
            strcpy( &cmdline[cmd_put][0], "go wtime 873260 btime 711020 movestogo 25\n" );
            test++;
        }
        else if( NULL == fgets(&cmdline[cmd_put][0],BIGBUF,stdin) )
            quit = true;
        if( !quit )
        #else
        if( NULL == fgets(&cmdline[cmd_put][0],BIGBUF,stdin) )
            quit = true;
        else
        #endif
        {
            if( strchr(&cmdline[cmd_put][0],'\n') )
                *strchr(&cmdline[cmd_put][0],'\n') = '\0';
            DebugPrintfInner( "cmd[%s]>%s\n", log_time(), &cmdline[cmd_put][0] );
            if( 0 == _strcmpi(&cmdline[cmd_put][0],"quit") )
            {
                gbl_stop = true;
                quit = true;
            }
            else if( 0 == _strcmpi(&cmdline[cmd_put][0],"stop") )
                gbl_stop = true;
        }
        ReleaseSemaphore(hSem, 1, 0);
        cmd_put++;
        cmd_put &= 7;
    }
}

void write_stdout()
{
    bool quit=false;
    while(!quit)
    {
        WaitForSingleObject(hSem, INFINITE);
        quit = process(&cmdline[cmd_get][0]);
        cmd_get++;
        cmd_get &= 7;
    }
} 

int main( int argc, char *argv[] )
{
    #ifdef CREATE_LOG
    file_log = fopen( "C:\\toy.file_log", "at" );
    #endif
#ifdef JUST_TESTING
    chess_engine.Test();
#else
    unsigned long tid1;
//    unsigned long tid2;

    HANDLE hThread1;
//    HANDLE hThread2;

    HANDLE handle = (HANDLE)_get_osfhandle(_fileno(stdin));
    int last=34;
    hSem = CreateSemaphore(NULL,0,10,NULL);

 
    hThread1 = CreateThread(0,
                            0,
                            (LPTHREAD_START_ROUTINE) read_stdin,
                            0,
                            0,
                            &tid1);

/*    hThread2 = CreateThread(0,
                            0,
                            (LPTHREAD_START_ROUTINE) write_stdout,
                            0,
                            0,
                            &tid2); */
    
    write_stdout();
    //WaitForSingleObject(hThread2, INFINITE);
    CloseHandle(hSem);
    CloseHandle(hThread1);
//    CloseHandle(hThread2);
#endif    
    if( file_log )
        fclose(file_log);
    return 0;
}

const char *log_time()
{
    static char buf[1024];
    time_t t = time(NULL);
    struct tm *p = localtime(&t);
    const char *s = asctime(p);
    strcpy( buf, s );
    if( strchr(buf,'\n') )
        *strchr(buf,'\n') = '\0';
    return buf;
}

// Case insensitive pattern match
// Return NULL if no match
// Return ptr into string beyond matched part of string
// Only if more is true, can the string be longer than the matching part
// So if more is false, and NULL is not returned, the returned ptr always points at trailing '\0'
const char *str_pattern( const char *str, const char *pattern, bool more=false );
const char *str_pattern( const char *str, const char *pattern, bool more )
{
    bool match=true;
    int c, d;
    while( match && *str && *pattern )
    {
        c = *str++;
        d = *pattern++;
        if( c != d )
        {
            match = false;
            if( isascii(c) && isascii(d) && toupper(c)==toupper(d) )
                match = true;
            else if( c=='\t' && d==' ' )
                match = true;
        }
        if( match && (c==' '||c=='\t') )
        {
            while( *str == ' ' )
                str++;
        }
    }
    if( !match )
        str = NULL;
    else if( *pattern )
        str = NULL;
    else if( !more && *str )
        str = NULL;
    if( more && str )
    {
        while( *str == ' ' )
            str++;
    }
    return str;
}

bool process( const char *buf )
{
    bool quit=false;
    const char *parm;
    const char *rsp = NULL;
    if( str_pattern(buf,"quit") )
        quit = true;
    else if( str_pattern(buf,"uci") )
        rsp = cmd_uci();
    else if( str_pattern(buf,"isready") )
        rsp = cmd_isready();
    else if( str_pattern(buf,"stop") )
        rsp = cmd_stop();
    else if( str_pattern(buf,"go infinite") )
        rsp = cmd_go_infinite();
    else if( NULL != (parm=str_pattern(buf,"go",true)) )
        rsp = cmd_go( parm );
    else if( NULL != (parm=str_pattern(buf,"setoption name MultiPV value",true)) )
        cmd_multipv( parm );
    else if( NULL != (parm=str_pattern(buf,"position",true)) )
        rsp = cmd_position( parm );
    if( rsp )
    {
        DebugPrintfInner( "rsp[%s]>%s", log_time(), rsp );
        fprintf( stdout, "%s", rsp );
        fflush( stdout );
    }
    return quit;
}


const char *cmd_uci()
{
    const char *rsp=
    "id name " ENGINE_NAME " " VERSION "\n"
    "id author Bill Forster\n"
 // "option name Hash type spin min 1 max 4096 default 32\n"
    "option name MultiPV type spin default 1 min 1 max 4\n"
    "uciok\n";
    return rsp;
}

const char *cmd_isready()
{
    const char *rsp = "readyok\n";
    return rsp;
}

// Measure elapsed time, nodes    
static unsigned long base_time;
extern int DIAG_make_move_primary;	
static int base_nodes;
static char stop_rsp_buf[128];
const char *cmd_stop()
{
    return stop_rsp_buf;
}

static int MULTIPV=1;
void cmd_multipv( const char *cmd )
{
    MULTIPV = atoi(cmd);
    if( MULTIPV < 1 )
        MULTIPV = 1;
    if( MULTIPV > 4 )
        MULTIPV = 4;
}

void ReportOnProgress
(
    bool    init,
    int     multipv,
    vector<Move> &pv,
    int     score_cp,
    int     depth
)

{
    static ChessPosition pos;
    if( init )
    {
        pos = chess_engine;
        base_time  = GetTickCount();
        base_nodes = DIAG_make_move_primary;
    }
    else
    {
        ChessRules ce = pos;
        int score_overide;
        static char buf[1024];
        static char buf_pv[1024];
        static char buf_score[128];
        bool done=false;
        bool have_move=false;
        Move bestmove_so_far;
        bestmove_so_far.Invalid();
        unsigned long now_time = GetTickCount();	
        int nodes = DIAG_make_move_primary-base_nodes;
        unsigned long elapsed_time = now_time-base_time;
        buf_pv[0] = '\0';
        char *s;
        bool overide = false;
        for( unsigned int i=0; i<pv.size(); i++ )
        {
            bool okay;
            Move move=pv[i];
            ce.PlayMove( move );
            have_move = true;
            if( i == 0 )
                bestmove_so_far = move;    
            s = strchr(buf_pv,'\0');
            sprintf( s, " %s", move.TerseOut().c_str() );
            TERMINAL score_terminal;
            okay = ce.Evaluate( score_terminal );
            if( okay )
            {
                if( score_terminal == TERMINAL_BCHECKMATE ||
                    score_terminal == TERMINAL_WCHECKMATE )
                {
                    overide = true;
                    score_overide = (i+2)/2;    // 0,1 -> 1; 2,3->2; 4,5->3 etc 
                    if( score_terminal == TERMINAL_WCHECKMATE )
                        score_overide = 0-score_overide; //negative if black is winning
                }
                else if( score_terminal == TERMINAL_BSTALEMATE ||
                         score_terminal == TERMINAL_WSTALEMATE )
                {
                    overide = true;
                    score_overide = 0;
                }
            }
            if( !okay || overide )
                break;
        }    
        if( pos.white )
        {
            if( overide ) 
            {
                if( score_overide > 0 ) // are we mating ?
                    sprintf( buf_score, "mate %d", score_overide );
                else if( score_overide < 0 ) // are me being mated ?
                    sprintf( buf_score, "mate -%d", (0-score_overide) );
                else if( score_overide == 0 ) // is it a stalemate draw ?
                    sprintf( buf_score, "cp 0", score_overide );
            }
            else
            {
                sprintf( buf_score, "cp %d", score_cp );
            }
        }
        else
        {
            if( overide ) 
            {
                if( score_overide < 0 ) // are we mating ?
                    sprintf( buf_score, "mate %d", 0-score_overide );
                else if( score_overide > 0 ) // are me being mated ?        
                    sprintf( buf_score, "mate -%d", score_overide );
                else if( score_overide == 0 ) // is it a stalemate draw ?
                    sprintf( buf_score, "cp 0", score_overide );
            }
            else
            {
                sprintf( buf_score, "cp %d", 0-score_cp );
            }
        }
        if( elapsed_time == 0 )
            elapsed_time++;
        if( have_move )
        {
            if( multipv )
                sprintf( buf, "info multipv %d depth %d score %s hashfull 0 time %lu nodes %lu nps %lu pv%s\n",
                            multipv,
                            depth+1,
                            buf_score,
                            (unsigned long) elapsed_time,
                            (unsigned long) nodes,
                            1000L * ((unsigned long) nodes / (unsigned long)elapsed_time ),
                            buf_pv );
            else
                sprintf( buf, "info depth %d score %s hashfull 0 time %lu nodes %lu nps %lu pv%s\n",
                            depth+1,
                            buf_score,
                            (unsigned long) elapsed_time,
                            (unsigned long) nodes,
                            1000L * ((unsigned long) nodes / (unsigned long)elapsed_time ),
                            buf_pv );
            fprintf( stdout, buf );
            fflush( stdout );
            DebugPrintfInner( "rsp[%s]>%s", log_time(), buf );
        }
    }
}

const char *cmd_go( const char *cmd )
{
    stop_rsp_buf[0] = '\0';
    int depth=5;
    static char buf[128];
    #define BALANCE 4

    // Work out our time and increment
    // eg cmd ="wtime 30000 btime 30000 winc 0 binc 0"
    const char *stime;
    const char *sinc;
    int ms_time   = 0;
    int ms_inc    = 0;
    int ms_budget = 0;
    if( chess_engine.white )
    {
        stime = "wtime";
        sinc  = "winc";
    }
    else
    {
        stime = "btime";
        sinc  = "binc";
    }
    const char *q = strstr(cmd,stime);
    if( q )
    {
        q += 5;
        while( *q == ' ' )
            q++;
        ms_time = atoi(q);
    }
    q = strstr(cmd,sinc);
    if( q )
    {
        q += 4;
        while( *q == ' ' )
            q++;
        ms_inc = atoi(q);
    }

    // Allocate an amount of time based on how long we've got
    DebugPrintfInner( "Input ms_time=%d, ms_inc=%d\n", ms_time, ms_inc );
    if( ms_time == 0 )
        ms_budget = 0;
    else if( ms_inc == 0 )
        ms_budget = ms_time/40;
    else if( ms_time > 100*ms_inc )
        ms_budget = ms_time/30;
    else if( ms_time > 50*ms_inc )
        ms_budget = ms_time/20;
    else if( ms_time > 10*ms_inc )
        ms_budget = ms_time/10;
    else if( ms_time > ms_inc )
        ms_budget = ms_time/5;
    else
        ms_budget = ms_time/2;

    // When time gets really short get serious
    if( ms_budget < 50 )
        ms_budget = 0;
    DebugPrintfInner( "Output ms_budget%d\n", ms_budget );
    Move bestmove;
    int score_cp=0;
    bestmove.Invalid();
    vector<Move> pv;
    ReportOnProgress
    (
        true,
        1,
        pv,
        0,
        0
    );
    bool new_game = is_new_game();
    bool have_move = chess_engine.CalculateNextMove( new_game, pv, bestmove, score_cp, ms_time, ms_budget, BALANCE, depth );

/*
    // Public interface to version for repitition avoidance
    bool CalculateNextMove( vector<Move> &pv, Move &bestmove, int &score_cp,
                            unsigned long ms_time, int balance,
                            int &depth, unsigned long &nodes  );
 */

/*  // Commented out - let the function report its own progress
    ReportOnProgress
    (
        false,
        1,
        pv,
        score_cp,
        depth
    );  */

    sprintf( buf, "bestmove %s\n", bestmove.TerseOut().c_str() );
    return buf;
}

const char *cmd_go_infinite()
{
    stop_rsp_buf[0] = '\0';
    static char buf[1024];
    #define BALANCE 4
    ChessPosition pos = chess_engine;
    bool done=false;
    Move bestmove_so_far;
    bestmove_so_far.Invalid();
    vector<Move> pv;
    ReportOnProgress
    (
        true,
        1,
        pv,
        0,
        0
    );
    for( int depth=0; !done && depth<20; depth++ )
    {
        for( int multi=1; multi<=MULTIPV; multi++ )
        {
            chess_engine = pos;
            int score;
            bestmove.Invalid();
            bool have_move = chess_engine.CalculateNextMove( score, bestmove, BALANCE, depth, multi==1 );

            /*
                // A version for Multi-PV mode, called repeatedly, removes best move from
                //  movelist each time
                bool CalculateNextMove( int &score, Move &move, int balance, int depth, bool first );
            */

            if( gbl_stop || (multi==1 && !have_move) )
            {
                done = true;
                break;
            }
            if( !have_move )
                break;
            if( multi==1 && bestmove.Valid() )
                bestmove_so_far = bestmove;
            int score_cp = (score*10)/BALANCE;  // convert to centipawns
            if( score_cp > 30000 )
                score_cp = 30000 + (score_cp-30000)/10000;
            else if( score < -30000 )
                score_cp = -30000 + (score_cp+30000)/10000;
            chess_engine.GetPV( pv );
            ReportOnProgress
            (
                false,
                multi,
                pv,
                score_cp,
                depth
            );
        }
    } 
    if( gbl_stop && bestmove_so_far.Valid() )
    {
        unsigned long now_time = GetTickCount();	
        int nodes = DIAG_make_move_primary-base_nodes;
        unsigned long elapsed_time = now_time-base_time;
        if( elapsed_time == 0 )
            elapsed_time++;
        sprintf( stop_rsp_buf, "info time %lu nodes %lu nps %lu\n" 
                               "bestmove %s\n",
                               (unsigned long) elapsed_time,
                               (unsigned long) nodes,
                               1000L * ((unsigned long) nodes / (unsigned long)elapsed_time ),
                               bestmove_so_far.TerseOut().c_str() ); 
    }
    return NULL;
}

static bool different_game;
static bool is_new_game()
{
    return different_game;
}

const char *cmd_position( const char *cmd )
{
    static ChessPosition prev_position;
    static Move last_move, last_move_but_one;
    different_game = true;
    DebugPrintfInner( "cmd_position(): Setting different_game = true\n" );
    ChessEngine tmp;
    chess_engine = tmp;    //init
    const char *s, *parm;
    bool look_for_moves = false;
    if( NULL != (parm=str_pattern(cmd,"fen",true)) )
    {
        chess_engine.Forsyth(parm);
        look_for_moves = true;
    }
    else if( NULL != (parm=str_pattern(cmd,"startpos",true)) )
    {
        ChessEngine tmp;
        chess_engine = tmp;    //init
        look_for_moves = true;
    }

    // Previous position
    ChessRules temp=prev_position;

    // Previous position updated
    prev_position = chess_engine;
    if( look_for_moves )
    {
        s = strstr(parm,"moves ");
        if( s )
        {
            last_move_but_one.Invalid();
            last_move.Invalid();
            s = s+5;
            for(;;)
            {
                while( *s == ' ' )
                    s++;
                if( *s == '\0' )
                    break;
                Move move;
                bool okay = move.TerseIn(&chess_engine,s);
                s += 5;
                if( !okay )
                    break;
                chess_engine.PlayMove( move );
                last_move_but_one = last_move;
                last_move         = move;
            }

            // Previous position updated with latest moves
            prev_position = chess_engine;

            // Maybe this latest position is the old one with two new moves ?
            if( last_move_but_one.Valid() && last_move.Valid() )
            {
                temp.PlayMove( last_move_but_one );
                temp.PlayMove( last_move );
                if( prev_position == temp )
                {
                    // Yes it is! so we are still playing the same game
                    different_game = false;
                    DebugPrintfInner( "cmd_position(): Setting different_game = false\n" );
                }
            }
        }
    }
    return NULL;
}

