#include <stddef.h>

// collapsed from header
long sys_write(int fd, const void* buf, size_t len);
__attribute__((noreturn)) void sys_exit(int code);

int main(void) {
    const char msg[] = "Hello world!\n";
    long status = sys_write(1, msg, sizeof(msg) - 1);
    sys_exit(status < 0 ? 1 : 0);
}
