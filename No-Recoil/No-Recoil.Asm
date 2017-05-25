; A no-recoil hack for CoD single-player that works by removing the instructions for aim being punched up
; while shooting.
;
; Originally written 2008/09/20 by attilathedud.

; System descriptors
.386
.model flat,stdcall
option casemap:none

VirtualAlloc proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
VirtualProtect proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
VirtualFree proto stdcall :DWORD, :DWORD, :DWORD

includelib \masm32\lib\kernel32.lib

.code
	main:
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

		; Unprotect the memory at 3003a313h-3003a318h
		push ebx
		push 40h
		push 5h
		push 3003a313h
		call VirtualProtect 

		; Move 3003a313h into ecx and then iterate until 3003a318h, noping every instruction
		mov ecx,3003a313h
		@nop:
			mov byte ptr ds:[ecx],90h
			inc ecx
			cmp ecx,3003a317h
			jl @nop

		; Reprotect the memory we just wrote.
		push 0
		push dword ptr ds:[ebx]
		push 5h
		push 3003a313h
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
	end main