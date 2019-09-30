#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void __free_malloc_block(void *ptr) {
  if ( ptr != NULL ) {
    free(ptr);
    printf("%s: Cleaning up: %p\n", __func__, ptr);
  }
}

void __free_clean_up(void * const * ptr) {
  __free_malloc_block(*ptr);
}

#define SCOPED_DATA void * const __attribute__((cleanup(__free_clean_up)))

// Fancier things are possible with some macro magic
#define SCOPED_CALLOC(type, nbr, name) \
    SCOPED_DATA __scopeGuard__LINE__ = malloc(sizeof(type) * (nbr)); \
    type *const name = (type *const)__scopeGuard__LINE__


void __char_buffer_cleanup(char * const *ptr) {
  __free_malloc_block(*ptr);
}

#define RAII(cleanup_func) __attribute__((cleanup(cleanup_func)))


void myFunc1(const char *str) {
  const size_t buf_size = strlen(str) + 1;
  SCOPED_DATA buffer = malloc(buf_size); //... buffer is cleaned up when myFunc1 ends
  strcpy(buffer, str);
  puts(buffer);
}

void myFunc2(const char *str) {
  const size_t buf_size = 8;
  SCOPED_CALLOC(char, buf_size, buffer); //... buffer is cleaned up when myFunc2 ends
  size_t n = strlen(str);
  if (n >= buf_size) {
    n = buf_size - 1;
  }
  strncpy(buffer, str, n);
  puts(buffer);
}

void myFunc3(const char *str) {
  const size_t buf_size = strlen(str) + 1;
  RAII(__char_buffer_cleanup) char * const buffer = malloc(buf_size);
  strcpy(buffer, str);
  puts(buffer);
}

int main(int argc, const char *argv[]) {
  myFunc1("scoped allocation of heap memory buffers");
  myFunc2("hello world!");
  myFunc3("this works out too and has the cleanest syntax look");
}