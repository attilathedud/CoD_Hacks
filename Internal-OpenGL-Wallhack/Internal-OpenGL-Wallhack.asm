; A OpenGL wallhack that works via hooking CoD's internal call to glDrawElements while displaying players
; and modifies the call to disable depth testing.
;
; Originally written 2008/09/24 by attilathedud.

; System descriptors.
.386
.model flat,stdcall
option casemap:none

VirtualAlloc proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
VirtualProtect proto stdcall :DWORD, :DWORD, :DWORD, :DWORD
VirtualFree proto stdcall :DWORD, :DWORD, :DWORD

glDepthFunc proto stdcall :DWORD
glDrawElements proto stdcall :DWORD, :DWORD, :DWORD, :DWORD

includelib \masm32\lib\opengl32.lib
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

		; Unprotect the memory at 518d5ch-518d6ch
        push ebx
        push 40h
        push 10h
        push 518d5ch
        call VirtualProtect

		; Create a codecave in the draw_elements routine that will call to our hook function.
		; e8h is the opcode to call, with the address of the jump being calculated by subtracting
		; the address of the function to jump to from our current location.
		mov byte ptr ds:[518d5ch],0e8h
		lea ecx,@wallhack
		sub ecx,518d61h
		mov dword ptr ds:[518d5dh],ecx

		; Since we are replacing 10h bytes with 5h, we need to nop out the rest of the instructions.
		mov ecx,518d61h
		@nop:
			mov byte ptr ds:[ecx],90h
			inc ecx
			cmp ecx,518d72h
			jl @nop

		; Reprotect the memory we just wrote.
		push 0
	    push dword ptr ds:[ebx]
	    push 10h
	    push 518d5ch
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

		; Our wallhack works by calling glDepthFunc(GL_ALWAYS), then the original glDrawElements call,
		; and then resetting everything by calling glDepthFunc(GL_LEQUAL) so that only entities are drawn
		; without respect to depth.
		@wallhack:
			; Save all the registers and set depth to GL_ALWAYS
			pushad
			push 0207h
			call glDepthFunc
			popad

			; Original instructions
			push ecx
			push 1403h
			push eax
			inc ebp
			push 4
			mov dword ptr ds:[199a2b8h],ebp
			call glDrawElements 

			; Save all the registers and set depth to GL_LEQUAL
			pushad
			push 0203h
			call glDepthFunc
			popad
			retn
			
	end main