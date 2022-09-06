// In order to use the the logger, you have to define __YAL_ON__ flag.
// If the flag is not defined, there will be no logs.

#ifndef __YAL_LOG__
#define __YAL_LOG__

#include <stdio.h>

struct __yal_logger_vars
{
  static int   dbg_lvl;
  static FILE *output;
};

#define __yal_init_logger()                                                    \
  do                                                                           \
  {                                                                            \
    __yal_set_output (stderr);                                                 \
  } while (0)

#define __yal_set_output(out) (__yal_logger_vars::output = out)

#define __yal_set_log_file(file) (__yal_logger_vars::output = fopen (file, "a"))
#define __yal_close_log_file() (fclose (__yal_logger_vars::output))

#define __yal_set_lvl_err() (__yal_logger_vars::dbg_lvl = __YAL_ERROR)
#define __yal_set_lvl_warn() (__yal_logger_vars::dbg_lvl = __YAL_WARN)
#define __yal_set_lvl_dbg() (__yal_logger_vars::dbg_lvl = __YAL_DBG)
#define __yal_set_lvl_info() (__yal_logger_vars::dbg_lvl = __YAL_INFO)

#define __YAL_ERROR 1
#define __YAL_WARN 2
#define __YAL_DBG 3
#define __YAL_INFO 4


#ifdef __YAL_ON__
#define __yal_logging(lvl) (__yal_logger_vars::dbg_lvl >= lvl)
#define __yal_log(lvl, ...)                                                    \
  do                                                                           \
  {                                                                            \
    if (__yal_logger_vars::dbg_lvl >= lvl)                                     \
      fprintf (__yal_logger_vars::output, __VA_ARGS__);                        \
  } while (0)
#else
#define __yal_logging(lvl) 0
#define __yal_log(...)                                                         \
  {                                                                            \
  }
#endif

#endif    // __YAL_LOG
