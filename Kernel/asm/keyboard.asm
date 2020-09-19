GLOBAL keyboardActivated
GLOBAL getKeyboardScancode
section .text

;--------------------------------------
;keyboardActivated - detecta si hay un codigo de teclado en el buffer
;--------------------------------------
;return: 1 de haber codigo en el buffer, 0 si no
;---------------------------------------

keyboardActivated:
    mov rax, 0
    in al, 64h 
    and al, 01h
    ret

;-------------------------------------
;getKeyboardChar - devuelve el codigo de teclado del buffer
;----------------------------------------
;return: codigo del teclado
;--------------------------------------
getKeyboardScancode: 
    mov rax, 0
    in al, 60h
    ret



