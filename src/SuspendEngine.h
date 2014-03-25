/****************************************************************************
 * Objects of this type will automatically suspend and resume the UCI engine
 *  as they come into and out of scope.
 *  Author:  Bill Forster
 *  Licence: See licencing information in ChessPosition.cpp
 *  Copyright 2010, Triple Happy Ltd.  All rights reserved.
 ****************************************************************************/
#ifndef SUSPEND_ENGINE_H
#define SUSPEND_ENGINE_H
#include "Objects.h"
#include "Rybka.h"

class SuspendEngine
{
public:

    // Constructor - suspend if appropriate
    SuspendEngine()
    {
        suspended=false;
        if( objs.rybka && !objs.rybka->IsSuspended() )
        {
            objs.rybka->SuspendResume(false);
            suspended = true;
        }
    }

    // Destructor - resume if we suspended
    ~SuspendEngine()
    {
        if( objs.rybka && suspended )
            objs.rybka->SuspendResume(true);
    }

private:
    bool suspended;
};

#endif // SUSPEND_ENGINE_H
