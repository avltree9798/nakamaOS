.section .text
.global start

start:
    mrs x0, mpidr_el1
    and x0, x0, #3
    cmp x0, #0 // Just use 1 CPU core for now.
    beq kernel_entry

end:
    b end

kernel_entry:
    mov sp, #0x80000
    bl kernel_main
    b end
