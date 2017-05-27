An internal ESP that works by hooking the game loop responsible for calling world-to-screen and loads a custom vm_main function that prints an "X" over the current location before rendering the entity.

The hack was written in mASM and needs to be linked as a dll. To do this:
```
\masm32\bin\ml /c /coff Esp.asm
\masm32\bin\Link /SUBSYSTEM:WINDOWS /DLL Esp.obj
```

![Hack Screenshot](screenshot.jpg?raw=true "Screenshot Hack")

Originally written 2010/03/21 by attilathedud.
