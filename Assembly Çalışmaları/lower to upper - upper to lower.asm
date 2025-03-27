org 100h

mov si, offset messg
mov bl, [B]
mov cl, [K]
mov dl, [S]

label1:      
    mov al, [si]
    
    cmp al, 35   ;#
    je label4  ;kare ise sonlanacak
    
    cmp al, 097   ;a
    jb label2  ;buyuk harfler icin label2, kucukler icin devam
    
    ;burda kucukleri buyuk yapmaliyiz:
    cmp al, 122   ;z
    jg labelOzel  
    
    inc cl        ;K sayaci 
    
    sub al, 32
    mov [si], al
    jmp label3  
    

label2: ;burda buyukler kucuk yapilacak
    cmp al, 090   ;Z
    jg labelOzel
    
    cmp al, 065   ;A
    jb labelOzel
    
    inc bl   ;B sayaci
    
    add al, 32 
    mov [si],al  
    
label3:
    inc si
    jmp label1  

labelOzel:
    cmp al, ' '
    je label3
    
    inc dl ;S sayaci 
    jmp label3

label4:    
    mov [B], bl
    mov [K], cl
    mov [S], dl
    
    ret


messg db 'aDINIZ sOYADINIZ oGR. nUMARANIZ aSSG@2 MICROPROCESSOR 2025#'
B db 0
K db 0
S db 0

;NOT


;"mov [si], al-32" seklinde dogrudan bellege yazilmaz. Once bir register uzerinde sub/add islemi yapilir
;ardindan bellege yazilir.

