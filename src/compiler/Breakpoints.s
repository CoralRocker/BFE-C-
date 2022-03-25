	.section .data
fdin:
	.long 0
fdout:
	.long 1
	.section .rodata
fail_alloc:
	.string "Failed to allocate memory! (malloc/realloc returned nullptr)"
fail_write:
	.string "Failed to write byte to fdout! (call write returned 0)"
fail_read:
	.string "Failed to read byte from fdin! (call read returned 0)"
print_num:
	.string "%llu"
	.section .text
	.globl main
xmalloc:
	lea (,%rdi,8), %rdi
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
	lea (,%r12,8), %r12
	add %rax, %r12 
	mov %r13, %rdi
	push %rax
	call free
	pop %rax
	mov %rax, %r13
	lea (,%r14,2), %r14 
	ret
err_read:
	lea fail_read(%rip), %rdi
	call puts
	mov $2, %rax
	call exit
err_write:
	lea fail_write(%rip), %rdi
	call puts
	mov $2, %rax
	call exit
main:
	mov $1024, %rdi
	call xmalloc
	mov %rax, %r12
	mov %rax, %r13
	mov $1024, %r14
	mov %rax, %rdi
	mov $0, %rsi
	mov $1024, %rdx
	call memset
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	mov fdout(%rip), %edi
	mov %r12, %rsi
	mov $1, %rdx
	call write
	cmp $0, %rax
	je err_write
	jmp .L0e
.L0:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
	addq $8, %r12
	cmpq (%r13,%r14,8), %r12
	jl .Ls0
	call xrealloc
.Ls0:
	incq (%r12)
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
.L0e:
	cmpb $0, (%r12)
	jne .L0
	addq $8, %r12
	cmpq (%r13,%r14,8), %r12
	jl .Ls1
	call xrealloc
.Ls1:
	mov fdout(%rip), %edi
	mov %r12, %rsi
	mov $1, %rdx
	call write
	cmp $0, %rax
	je err_write
	jmp .L1e
.L1:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
.L1e:
	cmpb $0, (%r12)
	jne .L1
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	mov fdout(%rip), %edi
	mov %r12, %rsi
	mov $1, %rdx
	call write
	cmp $0, %rax
	je err_write
	jmp .L2e
.L2:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
.L2e:
	cmpb $0, (%r12)
	jne .L2
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	jmp .L3e
.L3:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
.L3e:
	cmpb $0, (%r12)
	jne .L3
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	jmp .L4e
.L4:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
	addq $8, %r12
	cmpq (%r13,%r14,8), %r12
	jl .Ls2
	call xrealloc
.Ls2:
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
.L4e:
	cmpb $0, (%r12)
	jne .L4
	addq $8, %r12
	cmpq (%r13,%r14,8), %r12
	jl .Ls3
	call xrealloc
.Ls3:
	jmp .L5e
.L5:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incq (%r12)
	addq $8, %r12
	cmpq (%r13,%r14,8), %r12
	jl .Ls4
	call xrealloc
.Ls4:
.L5e:
	cmpb $0, (%r12)
	jne .L5
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	mov fdout(%rip), %edi
	mov %r12, %rsi
	mov $1, %rdx
	call write
	cmp $0, %rax
	je err_write
	jmp .L6e
.L6:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
.L6e:
	cmpb $0, (%r12)
	jne .L6
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	mov fdout(%rip), %edi
	mov %r12, %rsi
	mov $1, %rdx
	call write
	cmp $0, %rax
	je err_write
	jmp .L7e
.L7:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
.L7e:
	cmpb $0, (%r12)
	jne .L7
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	jmp .L8e
.L8:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
.L8e:
	cmpb $0, (%r12)
	jne .L8
	addq $8, %r12
	cmpq (%r13,%r14,8), %r12
	jl .Ls5
	call xrealloc
.Ls5:
	jmp .L9e
.L9:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
.L9e:
	cmpb $0, (%r12)
	jne .L9
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	jmp .L10e
.L10:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
	addq $8, %r12
	cmpq (%r13,%r14,8), %r12
	jl .Ls6
	call xrealloc
.Ls6:
.L10e:
	cmpb $0, (%r12)
	jne .L10
	addq $8, %r12
	cmpq (%r13,%r14,8), %r12
	jl .Ls7
	call xrealloc
.Ls7:
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	jmp .L11e
.L11:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	addq $8, %r12
	cmpq (%r13,%r14,8), %r12
	jl .Ls8
	call xrealloc
.Ls8:
.L11e:
	cmpb $0, (%r12)
	jne .L11
	subq $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12
	mov fdout(%rip), %edi
	mov %r12, %rsi
	mov $1, %rdx
	call write
	cmp $0, %rax
	je err_write
	jmp .L12e
.L12:
	decq (%r12)
	mov (%r12), %rcx
	mov $0, %rax
	cmovsq %rax, %rcx
	mov %rcx, (%r12)
.L12e:
	cmpb $0, (%r12)
	jne .L12
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	incq (%r12)
	mov fdout(%rip), %edi
	mov %r12, %rsi
	mov $1, %rdx
	call write
	cmp $0, %rax
	je err_write
	mov $0, %rax
	ret
