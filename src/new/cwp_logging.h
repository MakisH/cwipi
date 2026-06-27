/*
 * \file
 */

#ifndef __CWP_LOGGING_H__
#define __CWP_LOGGING_H__

/*-----------------------------------------------------------------------------*/

/* Standard C library headers */
#include <stdio.h>
#include <stdbool.h>
/*-----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#if 0
} /* Fake brace to force Emacs auto-indentation back to column 0 */
#endif
#endif /* __cplusplus */

/*============================================================================
 * Public types
 *============================================================================*/

/*============================================================================
 * Public function prototypes
 *============================================================================*/

typedef enum {

  CWP_LOG_TRACE,
  CWP_LOG_DEBUG,
  CWP_LOG_INFO,
  CWP_LOG_WARN,
  CWP_LOG_ERROR,
  CWP_LOG_FATAL

} CWP_Log_t;

#define CWP_Log(level, ...) _cwp_log(level, __func__, __FILE__, __LINE__, __VA_ARGS__)
#define CWP_Log_assert(condition, ...) _cwp_log_assert(condition, __func__, __FILE__, __LINE__, __VA_ARGS__)
#define CWP_Log_fail(...) _cwp_log_fail(__func__, __FILE__, __LINE__, __VA_ARGS__)

void CWP_Log_set_level(int level);
void CWP_Log_set_quiet(int enable);
void CWP_Log_set_quiet_console(int enable);
void CWP_Log_set_quiet_logfile(int enable);

void _cwp_log(int level, const char* func, const char *file, int line, const char *fmt, ...);
void _cwp_log_va(int level, bool add_line_break, const char* func, const char *file, int line, const char *fmt, va_list args);
void _cwp_log_assert(bool condition, const char* func, const char *file, int line, const char *fmt, ...);
void _cwp_log_fail(const char* func, const char *file, int line, const char *fmt, ...);

/**
 *
 * \brief Pretty print of array in log
 *
 * \param [in]    level        Log level
 * \param [inout] array        Array to print
 * \param [in]    larray       Array length
 * \param [inout] header       First line of log
 *
 */
void
CWP_Log_array_int
(
 const int   level,
 const int*  array,
 const int   larray,
 const char* header
);

/**
 *
 * \brief Pretty print of array in log
 *
 * \param [in]    level        Log level
 * \param [inout] array        Array to print
 * \param [in]    larray       Array length
 * \param [inout] header       First line of log
 *
 */
void
CWP_Log_array_double
(
 const int     level,
 const double* array,
 const int     larray,
 const char*   header
);

/**
 *
 * \brief Pretty print of array in log
 *
 * \param [in]    level        Log level
 * \param [inout] array        Array to print
 * \param [in]    larray       Array length
 * \param [inout] header       First line of log
 *
 */
void
CWP_Log_array_size_t
(
 const int     level,
 const size_t *array,
 const int     larray,
 const char   *header
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CWP_LOGGING_H__ */
