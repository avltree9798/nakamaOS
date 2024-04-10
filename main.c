#include "include/stdio.h"

int kernel_main()
{
#ifdef RUN_TESTS
    // Run tests
    return 0;
#endif
    // init_uart();
    uint64_t value = 0x1234567890ABCDEF;
    printk("Hello, %s\n", "world!");
    printk("1-2=%d\n", -1);
    printk("Test number %d\r\n", value);
    printk("Test number %x\r\n", value);
    while (1) {
        continue;
    }
    return 0;
}
