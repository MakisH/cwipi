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

typedef void (*log_lock_fn_cwp)(void *udata, int lock);


// Log levels
#define LVL_FATAL 200
#define LVL_WARNING 150
#define LVL_BASE 100
#define LVL_INFO 50
#define LVL_INFO2 20
#define LVL_DEBUG 10
#define LVL_DEBUG2 5
#define cwp_logger(level, ...) _cwp_log(level, __func__, __FILE__, __LINE__, __VA_ARGS__)
#define cwp_assert(condition, ...) _cwp_assert(condition, __func__, __FILE__, __LINE__, __VA_ARGS__)
#define cwp_fail(...) _cwp_fail(__func__, __FILE__, __LINE__, __VA_ARGS__)

void cwp_log_set_udata(void *udata);
void cwp_log_set_lock(log_lock_fn_cwp fn);
void cwp_log_set_fp(FILE *fp);
void cwp_log_set_level(int level);

void _cwp_log(int level, const char* func, const char *file, int line, const char *fmt, ...);
void _cwp_log_va(int level, bool add_line_break, const char* func, const char *file, int line, const char *fmt, va_list args);
void _cwp_assert(bool condition, const char* func, const char *file, int line, const char *fmt, ...);
void _cwp_fail(const char* func, const char *file, int line, const char *fmt, ...);

void cwp_log_set_active_logging_on_rank(bool);
void cwp_log_set_active_file_logging_on_rank(bool);
void cwp_log_set_active_console_logging_on_rank(bool);

const char* filename_without_path(const char*);

void center_text(char *buffer, size_t buffer_size, int width, const char *text);

// All functions begin with "cwp_" to ensure they are made accessible outside of CWIPI

/**
 *
 * \brief Pretty print of array in trace_log
 *
 * \param [in]    level        Log level
 * \param [inout] array        Array to print
 * \param [in]    larray       Array length
 * \param [inout] header       First line of log
 *
 */
void cwp_log_array_int(int level, const int* array, const int larray, const char* header);

/**
 *
 * \brief Pretty logging of array of doubles
 *
 * \param [in]    level        Log level
 * \param [inout] array        Array to print
 * \param [in]    larray       Array length
 * \param [inout] header       First line of log
 *
 */
void cwp_log_array_double(int level, const double* array, const int larray, const char* header);


/**
 *
 * \brief Pretty logging of array of "size_t" type
 *
 * \param [in]    level        Log level
 * \param [inout] array        Array to print
 * \param [in]    larray       Array length
 * \param [inout] header       First line of log
 *
 */
void cwp_log_array_size_t(int level, const size_t *array, const int larray, const char *header);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CWP_LOGGING_H__ */
