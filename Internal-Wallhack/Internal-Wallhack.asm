; An internal wallhack that hooks CoD's syscall function and adjusts the render flag of entities
; that pass through for rendering.
;
; Originally written 2008/12/20 by attilathedud.

; System descriptors.
.386
.model flat,stdcall
option casemap:none

VirtualAlloc proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
VirtualProtect proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
VirtualFree proto stdcall :DWORD, :DWORD, :DWORD

includelib \masm32\lib\kernel32.lib
	
.code
	main:
		jmp @F
			ori_syscall dd 4654c6h
		@@:
		; Save the current state of the stack.
		push ebp
		mov ebp,esp

		; Ensure our dll was loaded validily.
		mov eax,dword ptr ss:[ebp+0ch]
		cmp eax,1
		jnz @returnf

		; Allocate memory for the old protection type.
		; Store this location in ebx.
		push eax
		push 40h
		push 1000h
		push 4h
		push 0
		call VirtualAlloc 
		mov ebx,eax

		; Unprotect the memory at 4654c0h-4654c5h
		push ebx
		push 40h
		push 5h
		push 4654c0h
		call VirtualProtect

		; Create a codecave in the syscall routine that will jump to our hook function.
		; e9h is the opcode to jump, with the address of the jump being calculated by subtracting
		; the address of the function to jump to from our current location.
		mov byte ptr ds:[4654c0h],0e9h
		lea ecx,@cg_syscall
		sub ecx,4654c5h
		mov dword ptr ds:[4654c1h],ecx

		; Since our patched instruction is 6 bytes, nop out the remaining byte.
		mov byte ptr ds:[4654c5h],90h

		; Reprotect the memory we just wrote.
		push 0
		push dword ptr ds:[ebx]
		push 5h
		push 4654c0h
		call VirtualProtect 

		; Free the memory we allocated for our protection type.
		push 4000h
		push 4h
		push ebx
		call VirtualFree 

		; Restore eax and the stack
		pop eax
		@returnf:
			leave
			retn 0ch
			
		; The syscall function in Quake3 engines is used to handle a variety of system calls,
		; including rendering entities (3dh). 
		@cg_syscall:
			; Save the base pointer and move the stack pointer into it.
			push ebp
			mov ebp,esp

			; Compare the second parameter to the instruction for rendering entities.
			mov eax,dword ptr ss:[esp+8h]
			cmp eax,3dh
			jnz @ori

			; If the current ref structure is our player, skip this to avoid a crash.
			mov eax,dword ptr ss:[esp+0ch]
			cmp eax,249f960h
			jz @ori

			; At this point, eax holds arg[ 0 ], which is a *refEntity_t.
			; eax+4h is r->renderfx.
			mov ebx,dword ptr ds:[eax+4h]

			; r->renderfx |= 8h, which sets the player to display through all.
			or ebx,8h

			; Set r->renderfx to the new renderer type.
			mov dword ptr ds:[eax+4],ebx

			; The original instruction and jump back.
			@ori:
				pop ebp
				mov ecx,dword ptr ds:[1460268h]
				jmp ori_syscall
				retn
			
	end main