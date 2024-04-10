#include "include/uart.h"

int kernel_main()
{
#ifdef RUN_TESTS
    // Run tests
    return 0;
#endif
    init_uart();
    write_string("Hello, world!\n");
    while (1) {
        continue;
    }
    return 0;
}
