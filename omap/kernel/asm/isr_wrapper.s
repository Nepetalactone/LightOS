/* filename: isr_wrapper.s */
.globl   isr_wrapper
.align   4
 
isr_wrapper:
    pushad
    call interrupt_handler
    popad
    iret