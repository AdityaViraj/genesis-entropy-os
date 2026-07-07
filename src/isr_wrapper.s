; Expose the wrappers
global isr32_wrapper
global isr33_wrapper

; Import the C handle functions 
extern timer_handler
extern keyboard_handler

isr32_wrapper:
    pusha
    cld 
    call timer_handler
    popa 
    iret

isr33_wrapper:
    pusha
    cld 
    call keyboard_handler
    popa 
    iret


