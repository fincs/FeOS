@ Fake placeholder C++ personality routine
.text
.align 2
.global __gxx_personality_v0
.hidden __gxx_personality_v0
.type __gxx_personality_v0, %function
__gxx_personality_v0:
	.word 0xF7F000F0 @ Magic identifier (undefined opcode)
