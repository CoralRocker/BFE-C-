  .section .rodata
fail_alloc:
  .string "Failed to allocate memory! (malloc/realloc returned nullptr)"
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
# PREPARE MEMORY
	mov $1024, %rdi
	call xmalloc
	mov %rax, %r12
	mov %rax, %r13
	mov $1024, %r14
	mov %rax, %rdi
	mov $0, %rsi
	mov $1024, %rdx
	call memset
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L0e
.L0:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls0
	call xrealloc
.Ls0:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L0e:
	cmpb $0, (%r12)
	jne .L0
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls1
	call xrealloc
.Ls1:
	movzb (%r12), %rdi
	call putchar
	jmp .L1e
.L1:
	decb (%r12)
.L1e:
	cmpb $0, (%r12)
	jne .L1
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L2e
.L2:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls2
	call xrealloc
.Ls2:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L2e:
	cmpb $0, (%r12)
	jne .L2
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls3
	call xrealloc
.Ls3:
	incb (%r12)
	movzb (%r12), %rdi
	call putchar
	jmp .L3e
.L3:
	decb (%r12)
.L3e:
	cmpb $0, (%r12)
	jne .L3
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L4e
.L4:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls4
	call xrealloc
.Ls4:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L4e:
	cmpb $0, (%r12)
	jne .L4
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls5
	call xrealloc
.Ls5:
	movzb (%r12), %rdi
	call putchar
	movzb (%r12), %rdi
	call putchar
	jmp .L5e
.L5:
	decb (%r12)
.L5e:
	cmpb $0, (%r12)
	jne .L5
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L6e
.L6:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls6
	call xrealloc
.Ls6:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L6e:
	cmpb $0, (%r12)
	jne .L6
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls7
	call xrealloc
.Ls7:
	incb (%r12)
	movzb (%r12), %rdi
	call putchar
	jmp .L7e
.L7:
	decb (%r12)
.L7e:
	cmpb $0, (%r12)
	jne .L7
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L8e
.L8:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls8
	call xrealloc
.Ls8:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L8e:
	cmpb $0, (%r12)
	jne .L8
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls9
	call xrealloc
.Ls9:
	movzb (%r12), %rdi
	call putchar
	jmp .L9e
.L9:
	decb (%r12)
.L9e:
	cmpb $0, (%r12)
	jne .L9
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L10e
.L10:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls10
	call xrealloc
.Ls10:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L10e:
	cmpb $0, (%r12)
	jne .L10
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls11
	call xrealloc
.Ls11:
	decb (%r12)
	movzb (%r12), %rdi
	call putchar
	jmp .L11e
.L11:
	decb (%r12)
.L11e:
	cmpb $0, (%r12)
	jne .L11
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L12e
.L12:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls12
	call xrealloc
.Ls12:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L12e:
	cmpb $0, (%r12)
	jne .L12
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls13
	call xrealloc
.Ls13:
	incb (%r12)
	movzb (%r12), %rdi
	call putchar
	jmp .L13e
.L13:
	decb (%r12)
.L13e:
	cmpb $0, (%r12)
	jne .L13
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L14e
.L14:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls14
	call xrealloc
.Ls14:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L14e:
	cmpb $0, (%r12)
	jne .L14
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls15
	call xrealloc
.Ls15:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	movzb (%r12), %rdi
	call putchar
	jmp .L15e
.L15:
	decb (%r12)
.L15e:
	cmpb $0, (%r12)
	jne .L15
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L16e
.L16:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls16
	call xrealloc
.Ls16:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L16e:
	cmpb $0, (%r12)
	jne .L16
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls17
	call xrealloc
.Ls17:
	movzb (%r12), %rdi
	call putchar
	jmp .L17e
.L17:
	decb (%r12)
.L17e:
	cmpb $0, (%r12)
	jne .L17
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	jmp .L18e
.L18:
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls18
	call xrealloc
.Ls18:
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	incb (%r12)
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decb (%r12)
.L18e:
	cmpb $0, (%r12)
	jne .L18
	inc %r12
	cmpq (%r14,%r13,), %r12
	jl .Ls19
	call xrealloc
.Ls19:
	movzb (%r12), %rdi
	call putchar
	jmp .L19e
.L19:
	decb (%r12)
.L19e:
	cmpb $0, (%r12)
	jne .L19
	dec %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	mov $0, %rax
	ret
