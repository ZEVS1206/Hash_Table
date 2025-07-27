section .text
    global my_strcasecmp

my_strcasecmp:
    xor eax, eax            
    xor ecx, ecx            
    xor edx, edx            

.compare_loop:
    mov cl, [rdi]           
    mov dl, [rsi]           
    
    
    cmp cl, 'A'
    jb .no_change1
    cmp cl, 'Z'
    ja .no_change1
    add cl, 32              
.no_change1:
    
    cmp dl, 'A'
    jb .no_change2
    cmp dl, 'Z'
    ja .no_change2
    add dl, 32              
.no_change2:
    
    test cl, cl             
    jz .check_second_end    
    
    cmp cl, dl              
    jne .return_diff        
    
    inc rdi                 
    inc rsi                 
    jmp .compare_loop

.check_second_end:
    test dl, dl             
    jz .return_equal        
    
.return_diff:
    movzx eax, cl           
    movzx edx, dl
    sub eax, edx
    ret

.return_equal:
    xor eax, eax            
    ret