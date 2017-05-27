; An internal ESP that works by hooking the game loop responsible for calling world-to-screen
; and loads a custom vm_main function that prints an "X" over the current location before rendering
; the entity.
;
; Originally written 2010/03/21 by attilathedud.

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
		jmp @F
			ori_hook dd 4688efh
			ori_vmmain dd 30021650h
			eng_textout dd 4e3f00h

			; White in CoD engine
			colour dd 3f800000h,0,0,3f800000h
			text db "X",0
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

		; Unprotect the memory at 4688e6h-4688ebh
		push ebx
		push 40h
		push 8h
		push 4688e6h
		call VirtualProtect 

		; Create a codecave in the rendering routine that will jump to our hook function.
		; e9h is the opcode to jump, with the address of the jump being calculated by subtracting
		; the address of the function to jump to from our current location.
		mov byte ptr ds:[4688e6h],0e9h
		lea ecx,@hook
		sub ecx,4688ebh
		mov dword ptr ds:[4688e7h],ecx

		; Since our patched instruction is 8h bytes, loop through and nop out the remaining bytes.
		mov ecx,4688ebh
		@nop:
			mov byte ptr ds:[ecx],90h
			inc ecx
			cmp ecx,4688eeh
			jl @nop

		; Reprotect the memory we just wrote.
		push 0
		push dword ptr ds:[ebx]
		push 8h
		push 4688e6h
		call VirtualProtect 

		; Free the memory we allocated for our protection type.
		push 4000h
		push 4h
		push ebx
		call VirtualFree 

		; Restore eax and the stack.
		pop eax
		@returnf:
			leave
			retn 0ch
			
		; Hook in the world-to-screen function. At this point, eax holds the calling function,
		; so we compare eax against the address for vm_main. If vm_main is the caller, load our
		; custom function into eax for execution.
		@hook:
			pushfd
			cmp eax,30021650h
			jnz short @ncgame
			lea eax,@vm_main
			@ncgame:
				; The original instructions.
				popfd
				test eax,eax
				mov edx,dword ptr ss:[esp+164h]
				jmp ori_hook
			
		; Our custom vm_main. At this point, ebp+8h holds the player structure to be rendered
		; along with x/y coordinates in world. By loading this into the call for textout, our
		; "X" will be rendered on top of the player.
		@vm_main:
			; Save the base pointer and load the stack pointer into it.
			push ebp
			mov ebp,esp

			; Call the original vm_main to take care of rendering the player.
			push dword ptr ss:[ebp+38h]
			push dword ptr ss:[ebp+34h]
			push dword ptr ss:[ebp+30h]
			push dword ptr ss:[ebp+2ch]
			push dword ptr ss:[ebp+28h]
			push dword ptr ss:[ebp+24h]
			push dword ptr ss:[ebp+20h]
			push dword ptr ss:[ebp+1ch]
			push dword ptr ss:[ebp+18h]
			push dword ptr ss:[ebp+14h]
			push dword ptr ss:[ebp+10h]
			push dword ptr ss:[ebp+0ch]
			push dword ptr ss:[ebp+8h]
			call dword ptr ds:[ori_vmmain]
			add esp,34h

			; Save eax so we don't corrupt it.
			push eax

			; Move the player struct into eax and then compare it to an active player.
			mov eax,dword ptr ss:[ebp+8h]
			cmp eax,4h
			jnz short @nactive

			; eng_textout( unknown, unknown, 0, unknown, color_3f, text, unknown, 0, 0)
			; eax holds a reference to the player struct.
			push 0
			push 0
			push 40e00000h
			lea eax,text
			push eax
			lea eax,colour
			push eax
			push 3f800000h
			push 0
			push 42c80000h
			push 42c80000h
			call dword ptr ds:[eng_textout]
			add esp,24h
			
			; Restore eax and the stack.
			@nactive:
				pop eax
				pop ebp
				retn
			
end main