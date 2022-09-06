#include "yal_log.h"

#ifdef __YAL_LOG__

int   __yal_logger_vars::dbg_lvl = 0;
FILE *__yal_logger_vars::output  = stderr;

#endif    // __YAL_LOG
