A OpenGL wallhack that works via hooking CoD's internal call to glDrawElements while displaying players and modifies the call to disable depth testing.

The hack was written in mASM and needs to be linked as a dll. To do this:
```
\masm32\bin\ml /c /coff Internal-OpenGL-Wallhack.asm
\masm32\bin\Link /SUBSYSTEM:WINDOWS /DLL Internal-OpenGL-Wallhack.obj
```

![Hack Screenshot](screenshot.jpg?raw=true "Screenshot Hack")

Originally written 2008/09/24 by attilathedud.
