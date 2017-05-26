An internal wallhack that hooks CoD's syscall function and adjusts the render flag of entities that pass through for rendering.

The hack was written in mASM and needs to be linked as a dll. To do this:
```
\masm32\bin\ml /c /coff Internal-Wallhack.asm
\masm32\bin\Link /SUBSYSTEM:WINDOWS /DLL Internal-Wallhack.obj
```

![Hack Screenshot](screenshot.jpg?raw=true "Screenshot Hack")

Originally written 2008/09/24 by attilathedud.
