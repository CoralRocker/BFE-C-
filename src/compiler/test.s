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
	mov %r14, %rdx
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
	mov $1024, %rdi
	call xmalloc
	mov %rax, %r12
	mov %rax, %r13
	mov $1024, %r14
	mov %rax, %rdi
	mov $0, %rsi
	mov $1024, %rdx
	call memset
	mov %r13, %rdi
	call free
	xor %rax, %rax
	ret
