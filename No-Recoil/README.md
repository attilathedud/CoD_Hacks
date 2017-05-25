A no-recoil hack for CoD single-player that works by removing the instructions for aim being punched up while shooting.

The hack was written in mASM and needs to be linked as a dll. To do this:
```
\masm32\bin\ml /c /coff No-Recoil.asm
\masm32\bin\Link /SUBSYSTEM:WINDOWS /DLL No-Recoil.obj
```

Originally written 2008/09/20 by attilathedud.
