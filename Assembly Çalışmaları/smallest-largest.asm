org 100h

lea si,arr
mov al,[si] ; al su anda en kucuk varsayalim, dizinin ilk elemani
mov cx,9  
inc si   

label2:
    cmp al,[si]
    jbe label1 
    mov al, [si] 
    mov bx, si  ; en kucuk deger su an si, onun adresini bx'de tuttum
    

label1:  
    inc si
    loop label2   

lea di,arr
mov ah,[di] ; ah su anda en buyuk varsayalim, dizinin ilk elemani
mov cx, 9
inc di

label3:
    cmp ah,[di]
    jae label4
    mov ah,[di]
    mov dx, di; en buyuk deger su an di, onun adresini dx'de tuttum

label4:
    inc di
    loop label3    
    
           
mov di, dx
mov si, bx
    
ret 
arr db 3, 5, 6, 9, 11, 1, 2, 14, 12, 7   ; arr[0] 0012Dh ile basladi




