native 'or', or
  pop rax
  or [rsp], rax
  jmp next


native 'and', and
  pop rdx
  pop rax
  and rax, rdx
  push rax
  jmp next

native 'not', not
  pop rax
  test rax, rax
  jz .setone
  xor rax, rax
  push rax
  jmp next
  .setone:
    xor rax, rax
    mov rax, 1
    push rax
    jmp next

native '=', equals
  pop rdx
  pop rax
  cmp rax, rdx
  jne .nequals
  xor rax, rax
  mov rax, 1
  push rax
  jmp next
  .nequals:
  xor rax, rax
  push rax
  jmp next

native 'lor', lor
  pop rax
  pop rdx
  test rax, rax
  jnz .yes
  push rdx
  jmp next
  .yes:
    push rax
    jmp next

native "land", land
  pop rax
  pop rdx
  test rax, rax
  jz .no
  push rdx
  jmp next
  .no:
    push rax
    jmp next

native '+', plus
  pop rdx
  pop rax
  add rax, rdx
  push rax
  jmp next

native '-', minus
  pop rdx
  pop rax
  sub rax, rdx
  push rax
  jmp next

native '*', mul
  pop rdx
  pop rax
  imul rdx
  push rax
  jmp next

native '/', div
  pop rax
  pop rcx
  cqo
  idiv rcx
  push rax
  jmp next

native "%", mod
  pop rcx
  pop rax
  cqo
  idiv rcx
  push rdx
  jmp next

native '<', less
  pop rdx
  pop rax
  cmp rax, rdx
  setl al
  movzx rax, al
  push rax
  jmp next

