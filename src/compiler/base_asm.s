  .section .data
fdin:
  .long 0
fdout:
  .long 1

  .section .rodata
fail_alloc:
  .string "\nFailed to allocate memory! (malloc/realloc returned nullptr)\n"
  .section .text
  .globl main
xmalloc:
  mov $1, %rsi
  call calloc
  cmp $0, %rax
  je .Lxmerr
  ret
.Lxmerr:
  lea fail_alloc(%rip), %rdi
  call puts
  mov $1, %rax
  call exit
xrealloc:
  lea (,%r14,2), %rdi
  call xmalloc
  mov %rax, %rdi
  mov %r13, %rsi
  mov %r14, %rdx
  call memmove
  sub %r13, %r12 # get index of r12
  add %rax, %r12 
  mov %r13, %rdi
  push %rax
  call free
  pop %rax
  mov %rax, %r13
  lea (,%r14,2), %r14 
  ret
main:
  

  mov $0, %rax
  ret
