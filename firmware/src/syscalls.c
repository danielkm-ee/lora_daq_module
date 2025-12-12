/**
 * Minimal system call stubs for newlib
 * Required when using standard C library functions like printf, snprintf, etc.
 */

#include <errno.h>
#include <sys/stat.h>

// Increase heap pointer
void *_sbrk(int incr) {
  extern char _end;    // Defined by linker
  extern char _estack; // Defined by linker
  static char *heap_end = 0;
  char *prev_heap_end;

  if (heap_end == 0) {
    heap_end = &_end;
  }

  prev_heap_end = heap_end;

  if (heap_end + incr > &_estack) {
    // Out of memory
    errno = ENOMEM;
    return (void *)-1;
  }

  heap_end += incr;
  return (void *)prev_heap_end;
}

// Exit program (not used in embedded)
void _exit(int status) {
  (void)status;
  while (1)
    ;
}

// Write to file descriptor (used by printf)
int _write(int file, char *ptr, int len) {
  (void)file;
  (void)ptr;
  (void)len;
  // You can implement UART output here if needed
  return len;
}

// Close file (not used)
int _close(int file) {
  (void)file;
  return -1;
}

// Send signal (not used)
int _kill(int pid, int sig) {
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}

// Get process ID (not used)
int _getpid(void) { return 1; }

// Read from file (not used)
int _read(int file, char *ptr, int len) {
  (void)file;
  (void)ptr;
  (void)len;
  return 0;
}

// Seek file (not used)
int _lseek(int file, int ptr, int dir) {
  (void)file;
  (void)ptr;
  (void)dir;
  return 0;
}

// Get file status (not used)
int _fstat(int file, struct stat *st) {
  (void)file;
  st->st_mode = S_IFCHR;
  return 0;
}

// Check if file is terminal (not used)
int _isatty(int file) {
  (void)file;
  return 1;
}