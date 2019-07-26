#ifndef __FOC__TIME_CALC__
#define __FOC__TIME_CALC__

#include <GameOptions.h>

#define REAL_MS( x )               ( (x) * GameOpt.TimeMultiplier / 1000 )
#define REAL_SECOND( x )           ( (x) * GameOpt.TimeMultiplier )
#define REAL_MINUTE( x )           ( (x) * GameOpt.TimeMultiplier * 60 )
#define REAL_HOUR( x )             ( (x) * GameOpt.TimeMultiplier * 3600 )
#define REAL_DAY( x )              ( (x) * GameOpt.TimeMultiplier * 86400 )
#define REAL_WEEK( x )             ( (x) * GameOpt.TimeMultiplier * 604800 )
#define REAL_MONTH( x )            ( (x) * GameOpt.TimeMultiplier * 2592000 )
#define REAL_YEAR( x )             ( (x) * GameOpt.TimeMultiplier * 31536000 )

#define GAME_SECOND( x )           ( (x) )
#define GAME_MINUTE( x )           ( (x) * 60 )
#define GAME_HOUR( x )             ( (x) * 3600 )
#define GAME_DAY( x )              ( (x) * 86400 )
#define GAME_WEEK( x )             ( (x) * 604800 )
#define GAME_MONTH( x )            ( (x) * 2592000 )
#define GAME_YEAR( x )             ( (x) * 31536000 )

#define REPLICATION_SECOND( x )    GAME_SECOND( x )
#define REPLICATION_MINUTE( x )    GAME_MINUTE( x )
#define REPLICATION_HOUR( x )      GAME_HOUR( x )
#define REPLICATION_DAY( x )       GAME_DAY( x )
#define REPLICATION_WEEK( x )      GAME_WEEK( x )
#define REPLICATION_MONTH( x )     GAME_MONTH( x )
#define REPLICATION_YEAR( x )      GAME_YEAR( x )

#define AFTER( x )                 (GameOpt.FullSecond + (x) )

#endif // __FOC__TIME_CALC__ //
