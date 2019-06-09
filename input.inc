%define BUFFER_SIZE 32

section .data
	char_diff: db 0x00
	buf : times 256 db 0

section .text

string_length:
    xor rax, rax ; 0 -> rax
    .loop:
      cmp  byte[rdi + rax], 0 ; counting address , setting flags
      je   .exit ; if flag equals 0 end the loop
      inc  rax ; if 1 going on
      jmp  .loop
    .exit:
    ret

print_string:
    call string_length ; putting the lenght into rax
    ; preparing registers for syscall sys_write
    mov rsi, rdi ; from
    mov rdi, 1 ; where
    mov rdx, rax ; how much
    mov rax, 1 ; sys_write
    syscall
    ret

print_char:
    push rdi ; saving symbol on stack
    mov rsi, rsp ; addres of the stack point
    mov rdi, 1 ; where
    mov rdx, 1 ; how much
    mov rax, 1 ; sys_write
    syscall
    pop rdi ; symbol from stack
    ret

print_newline:
    mov rdi, 10 
    jmp print_char 

string_copy:
    call string_length
    mov  rcx, rax ; length -> rcx
    inc  rcx ; we need to go to 0
    xchg rsi, rdi ; swap
    mov  byte[rdi + rax], 0 
    rep  movsb 
    ret

print_uint:
    push rbp ; saving rbp
    mov  rbp, rsp ; stack point -> rbp
    mov  rax, rdi ; saving to rax
    mov  rdi, 10 
    sub  rsp, BUFFER_SIZE ; making buffer
    dec  rbp 
    mov  byte[rbp], 0 
      .loop:
        dec  rbp 
        xor  rdx, rdx 
        div  rdi 
        add  rdx, '0' 
        mov  byte[rbp], dl
        test rax, rax
        jnz  .loop 
    mov rdi, rbp 
    call print_string 
    add rsp, BUFFER_SIZE
    pop rbp
    ret

print_int:
  test rdi, rdi
  jns  .unsigned 
  push rdi 
  mov  rdi, '-'  
  call print_char 
  pop  rdi
  neg  rdi 
  .unsigned:
  jmp  print_uint 

string_equals:
  call string_length
  mov  rcx, rax 
  xchg rdi, rsi 
  call string_length
  cmp  rax, rcx 
  jne  .not_equals
  repe cmpsb
  jne  .not_equals 
  mov rax, 1 
  ret
  .not_equals:
    mov rax, 0 
    ret

read_char:
	push 0
	mov rsi, rsp
	mov rdx, 1
	xor rdi, rdi
	xor rax, rax
	syscall
	pop rax
	ret

read_word:
	push r14
  push r15
  xor r14, r14
  mov r15, rsi
  dec r15

  .read_first:
	  push rdi
	  call read_char
	  pop rdi
	  cmp al, ' '
	  je .read_first
	  cmp al, 10
	  je .read_first
	  cmp al, 13
	  je .read_first
	  cmp al, 9
	  je .read_first
	  test al, al
	  jz .read_success

  .read_next:
	  mov byte [rdi + r14], al
	  inc r14

	  push rdi
	  call read_char
	  pop rdi
	  cmp al, ' '
	  je .read_success
	  cmp al, 10
	  je .read_success
	  cmp al, 13
	  je .read_success
	  cmp al, 9
	  je .read_success
	  test al, al
	  jz .read_success
	  cmp r14, r15
	  je .read_err
	  jmp .read_next

  .read_success:
	  mov byte [rdi + r14], 0
	  mov rax, rdi
	  mov rdx, r14
	  pop r15
	  pop r14
	  ret

  .read_err:
	  xor rax, rax
		xor rdx, rdx
	  pop r15
	  pop r14
	  ret

parse_uint:
  call string_length
  mov rcx, rax 
  mov rsi, rdi 
  xor rdx, rdx 
  xor rax, rax 
  .pars:
    xor  rdi, rdi 
    mov  dil, byte[rsi + rdx] 
    cmp  dil, '0' 
    jb   .exit 
    cmp  dil, '9'
    ja   .exit 
    sub  dil, '0' 
    imul rax, 10 
    add  rax, rdi 
    inc  rdx 
    dec  rcx 
    jnz  .pars 
  .exit:
  ret

parse_int:
  cmp byte[rdi], '-' 
  je .minus
  jmp parse_uint 
  .minus:
    inc rdi 
    call parse_uint 
    test rdx, rdx
    jz .null 
    neg rax 
    inc rdx 
    ret
  .null:
    xor eax, eax 
    ret
