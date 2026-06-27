/*============================================================================
 * Small logging adapted from "pdm_logging"
 *============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "cwp_logging.h"
#include "mpi.h"

#ifdef __cplusplus
extern "C" {
#if 0
} /* Fake brace to force Emacs auto-indentation back to column 0 */
#endif
#endif /* __cplusplus */

// Rank-wise global variables to store the logging parameters of the current rank
static FILE* logging_file = NULL;
static int i_rank;
static bool is_active = true;
static bool console_logging_activated = true;
static bool file_logging_activated = true;
static bool log_detail = true;

// Control logging on current rank
void cwp_log_set_active_logging_on_rank(bool val){
  is_active = val;
}

// Control file logging on current rank
void cwp_log_set_active_file_logging_on_rank(bool val){
  file_logging_activated = val;
}

// Control console logging on current rank
void cwp_log_set_active_console_logging_on_rank(bool val){
  console_logging_activated = val;
}

// Close logging file
static void free_logging_file(void)
{
  if(logging_file != NULL)
    fclose(logging_file);
}

static struct {
  void *udata;
  log_lock_fn_cwp lock;
  FILE *fp;
  int level;
  int quiet;
} L;

// Acquire lock
static void cwp_lock(void)   {
  if (L.lock) {
    L.lock(L.udata, 1);
  }
}

// Release lock
static void cwp_unlock(void) {
  if (L.lock) {
    L.lock(L.udata, 0);
  }
}

void cwp_log_set_udata(void *udata) {
  L.udata = udata;
}

void cwp_log_set_lock(log_lock_fn_cwp fn) {
  L.lock = fn;
}

// Set a common log file for all ranks
void cwp_log_set_fp(FILE *fp) {
  L.fp = fp;
}

void cwp_log_set_level(int level) {
  L.level = level;
}

const char* filename_without_path(const char* path) {
    const char* result = path;
    while(*path != '\0')
        if(*path++ == '/') result = path;

    return result;
}

// Automatically center a text within a given width
void center_text(char *buffer, size_t buffer_size, int width, const char *text) {
  if (!buffer || buffer_size == 0 || !text) return;

  size_t len = strlen(text);

  if (width < 0) width = 0;
  if ((size_t)width >= buffer_size) width = (int)buffer_size - 1;

  size_t pos = 0;

  if (len >= (size_t)width) {
      snprintf(buffer, buffer_size, "%.*s", width, text);
      return;
  }

  int left_padding  = (width - (int)len) / 2;
  int right_padding = width - (int)len - left_padding;

  for (int i = 0; i < left_padding && pos < buffer_size - 1; i++)
      buffer[pos++] = ' ';

  for (size_t i = 0; i < len && pos < buffer_size - 1; i++)
      buffer[pos++] = text[i];

  for (int i = 0; i < right_padding && pos < buffer_size - 1; i++)
      buffer[pos++] = ' ';

  buffer[pos] = '\0';
}

// If condition is false, an error is raised with the remaining arguments interpreted
// as a printf call.
void _cwp_assert(bool condition, const char* func, const char *file, int line, const char *fmt, ...){
  if (condition) return;
  log_detail = true;
  _cwp_log(LVL_FATAL, func, file, line, "Assertion error in \"%s\" (file %s, line %d)", func, filename_without_path(file), line);
  va_list args;
  va_start(args, fmt);
  _cwp_log_va(LVL_FATAL, true, func, file, line, fmt, args);
  va_end(args);
  fflush(stdout);
  fflush(stderr);
  exit(1);
}

void _cwp_fail(const char* func, const char *file, int line, const char *fmt, ...){
  log_detail = true;
  _cwp_log(LVL_FATAL, func, file, line, "Failure in \"%s\" (file %s, line %d)", func, filename_without_path(file), line);
  va_list args;
  va_start(args, fmt);
  _cwp_log_va(LVL_FATAL, true, func, file, line, fmt, args);
  va_end(args);
  fflush(stdout);
  fflush(stderr);
  exit(1);
}



/*
* Log to console / common file / rank-specific file a message, with ability to
* filter messages based on a criticality level.
*/
void _cwp_log_va(int level, bool add_line_break, const char* func, const char *file, int line, const char *fmt, va_list args){

  const char* filename = filename_without_path(file);

  if(file_logging_activated){

    /* Log to a specific common file for all ranks */
    if (L.fp) {
      /* Acquire lock */
      cwp_lock(); // only for the shared logging file ?
      // fprintf(L.fp, "-- %s -- | ", level_names[level]);
      if (log_detail) fprintf(L.fp, "rank %d (%4d) %10s:%4d:%s -", i_rank, level, filename, line, func);
      vfprintf(L.fp, fmt, args);
      if (add_line_break) fprintf(L.fp, "\n");
      fflush(L.fp);
      /* Release lock */
      cwp_unlock();
    }

    // Log to rank-specific file
    if(logging_file == NULL){
      // Create log file if not yet available
      char filepath[50];
      MPI_Comm_rank(MPI_COMM_WORLD, &i_rank);
      sprintf(filepath, "cwp_%d.log", i_rank);
      logging_file = fopen(filepath, "w");
      atexit(free_logging_file);
    }
    // fprintf(logging_file, "-- %s -- | ", level_names[level]);
    if (log_detail) fprintf(logging_file, "(%-4.4d) %-15.15s :%-4.4d:%-20.20s: ", level, filename, line, func);
    vfprintf(logging_file, fmt, args);
    if (add_line_break) fprintf(logging_file, "\n");
    fflush(logging_file);
  }

  if (console_logging_activated){
    // also print to console
    // if (log_detail) fprintf(stdout, "rank %d - (%4d) %s:%d -", i_rank, level, func, line);
    vfprintf(stdout, fmt, args);
    if (add_line_break) fprintf(stdout, "\n");
    fflush(stdout);
  }
}

/*
* Log to console / common file / rank-specific file a message, with ability to
* filter messages based on a criticality level.
*/
void _cwp_log(int level, const char* func, const char *file, int line, const char *fmt, ...) {
  //if (level<LVL_FATAL){
    if ( !is_active || (level < L.level) ){
      return;
    }
    // else we enforce logging to ensure all error messages are passed
  //}

  bool add_line_break=false;
  size_t len = strlen(fmt);
  if (len==0){
    add_line_break=true;
  }
  else if (fmt[len - 1] != '\n') {
    add_line_break=true;
  }

  va_list args;
  va_start(args, fmt);
  _cwp_log_va(level, add_line_break, func, file, line, fmt, args);
  va_end(args);
}

void cwp_log_array_int(int level, const int* array, const int larray, const char* header)
{
  cwp_logger(level, header);
  cwp_logger(level, "(%i) -> ", larray);
  for(int i = 0; i < larray; ++i){
    cwp_logger(level, "%d ", array[i]);
  }
  cwp_logger(level, "\n");
}

void cwp_log_array_double(int level, const double* array, const int larray, const char* header)
{
  cwp_logger(level, header);
  cwp_logger(level, "(%i) -> ", larray);
  for(int i = 0; i < larray; ++i){
    //cwp_logger(level, "%12.5e ", array[i]);
    cwp_logger(level, "%20.16e ", array[i]);
  }
  cwp_logger(level, "\n");
}

void cwp_log_array_size_t(int level, const size_t *array, const int larray, const char *header)
{
  cwp_logger(level, header);
  for(int i = 0; i < larray; ++i){
    cwp_logger(level, "%lu ", array[i]);
  }
  cwp_logger(level, "\n");
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
