#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void __free_clean_up(void * const * ptr)
{
  if ( *ptr ) {
    printf("Cleaning up: %p\n", *ptr);
    free(*ptr);
  }
}

#define SCOPED_DATA void * const __attribute__((cleanup(__free_clean_up)))

void myFunc(const char *str)
{
    const size_t buf_size = strlen(str) + 1;
    SCOPED_DATA buffer = malloc(buf_size); //... buffer is cleanued up when myFunc ends
    strcpy(buffer, str);
    puts(buffer);
}

// Fancier things are possible with some macro magic
#define SCOPED_ALLOC(type, name, len) \
    SCOPED_DATA __scopeGuard__LINE__ = malloc(len); \
    type const name = (type)__scopeGuard__LINE__;

void myFunc2(const char *str)
{
    const size_t buf_size = 8;
    SCOPED_ALLOC(char*, buffer, buf_size); //... buffer is cleanued up when myFunc ends

    size_t n = strlen(str);
    if (n >= buf_size) {
      n = buf_size - 1;
    }

    strncpy(buffer, str, n);

    puts(buffer);
}

int main(int argc, const char * argv[]) {
    myFunc("scoped allocation of heap memory buffers");
    myFunc2("hello world!");
}