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
debug:
	.string "(%lld, cell %llu, %llu cells)\n"
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
	lea (,%r14,8), %rdx
	call memmove
	sub %r13, %r12 # get index of r12
	shrq $3, %r12
	lea (,%r12,8), %r12
	add %rax, %r12 
	mov %r13, %rdi
	push %rax
	call free
	pop %rax
	mov %rax, %r13
	lea (,%r14,2), %r14 
	lea (%r13,%r14,8), %rax
	cmp %rax, %r12
	jb .Lxrealloc
	call xrealloc
.Lxrealloc:
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
debug_print:
	mov $0, %rax
	lea debug(%rip), %rdi
	movq (%r12), %rsi
	movq %r12, %rdx
	subq %r13, %rdx
	shrq $3, %rdx
	movq %r14, %rcx
	call printf
	ret
main:
	mov $4, %rdi
	call xmalloc
	mov %rax, %r12
	mov %rax, %r13
	mov $4, %r14
	mov %rax, %rdi
	mov $0, %rsi
	mov $4, %rdx
	call memset

	addq $5, (%r12)

	add $8, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls0
	call xrealloc
.Ls0:

	addq $3, (%r12)

	sub $8, %r12
	cmp %r13, %r12
	cmovl %r13, %r12

	jmp .L0e
.L0:

	add $8, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls1
	call xrealloc
.Ls1:

	movq (%r12), %rax
	addq %rax, 8(%r12)
	addq %rax, 16(%r12)
	movq $0, (%r12)

	add $16, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls2
	call xrealloc
.Ls2:

	movq (%r12), %rax
	addq %rax, -16(%r12)
	movq $0, (%r12)

	sub $16, %r12
	cmp %r13, %r12
	cmovl %r13, %r12

	jmp .L1e
.L1:

	add $8, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls3
	call xrealloc
.Ls3:

	movq (%r12), %rax
	addq %rax, 8(%r12)
	addq %rax, 16(%r12)
	movq $0, (%r12)

	add $16, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls4
	call xrealloc
.Ls4:

	movq (%r12), %rax
	addq %rax, -16(%r12)
	movq $0, (%r12)

	sub $24, %r12
	cmp %r13, %r12
	cmovl %r13, %r12

	subq $1, (%r12)
	movq (%r12), %rcx
	movq $0, %rax
	cmovsq %rax, %rcx
	movq %rcx, (%r12)

.L1e:
	cmpq $0, (%r12)
	jne .L1

	add $16, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls5
	call xrealloc
.Ls5:

	movq (%r12), %rax
	addq %rax, -16(%r12)
	movq $0, (%r12)

	sub $16, %r12
	cmp %r13, %r12
	cmovl %r13, %r12

	add $8, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls6
	call xrealloc
.Ls6:

	movq $0, (%r12)

	sub $16, %r12
	cmp %r13, %r12
	cmovl %r13, %r12

	subq $1, (%r12)
	movq (%r12), %rcx
	movq $0, %rax
	cmovsq %rax, %rcx
	movq %rcx, (%r12)

.L0e:
	cmpq $0, (%r12)
	jne .L0

	call debug_print

	add $8, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls7
	call xrealloc
.Ls7:

	call debug_print

	add $8, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls8
	call xrealloc
.Ls8:

	movq $0, (%r12)

	call debug_print

	add $8, %r12
	lea (%r13, %r14,8), %rax
	cmpq %rax, %r12
	jb .Ls9
	call xrealloc
.Ls9:

	call debug_print
	mov %r13, %rdi
	call free
	xor %rax, %rax
	ret
