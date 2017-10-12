#define FUNC(name) \
	.global name; \
	.type name, %function; \
name:

.section .text.start
.arm

_text_start:
	ldr pc, =hook_farcOpen

FUNC(jmp_hook_farcLoadAll)
	ldr pc, =hook_farcLoadAll

FUNC(orig_farcLoadAll)
	push {r4-r8, lr}
	b 0x2438EC

FUNC(wcscmp)
	push {lr}
	bl 0x22CB8C
	pop {pc}

FUNC(memcpy)
	b 0x206868
