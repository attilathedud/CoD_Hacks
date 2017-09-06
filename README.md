# COD_Hacks

A collection of hacks for Call of Duty v1.5.

## Desk
Desk.dll is a multi-hack for Call of Duty 1. It features:

- 3 state wallhack (all entities, flashing player models, and off). (Numpad 1)
- 6 state chams (red, blue, green, pink/violet, black, and off). (Numpad 2)
- Behind wall detection (turn on flashing models wallhack and chams, solid colors will indicate model is visible).
- Night-mode. (Numpad 3)
- No fog. (Numpad 4)
- White-walls/no textures. (Numpad 5)

Undetected by PB from 2007/08/22 to ~2009.

![Hack Screenshot](Desk/screenshot.jpg?raw=true "Screenshot Hack")

## Esp
An internal ESP that works by hooking the game loop responsible for calling world-to-screen and loads a custom vm_main function that prints an "X" over the current location before rendering the entity.

![Hack Screenshot](Esp/screenshot.jpg?raw=true "Screenshot Hack")

## idkmybffCoD
A multihack for CoD1 that features a wallhack, chams, full-bright, no-scope, no-fog and internal ui.

![Hack Screenshot](idkmybffCoD/screenshot.jpg?raw=true "Screenshot Hack")

## Internal-OpenGL-Wallhack
A OpenGL wallhack that works via hooking CoD's internal call to glDrawElements while displaying players and modifies the call to disable depth testing.

![Hack Screenshot](Internal-OpenGL-Wallhack/screenshot.jpg?raw=true "Screenshot Hack")

## Internal-Wallhack
An internal wallhack that hooks CoD's syscall function and adjusts the render flag of entities that pass through for rendering.

![Hack Screenshot](Internal-Wallhack/screenshot.jpg?raw=true "Screenshot Hack")

## No-Recoil
A no-recoil hack for CoD single-player that works by removing the instructions for aim being punched up while shooting.

## OpenGL-CountMonitor
A utility to log the counts of displayed entities to help isolate specific counts of player models.

## OpenGL-Wallhack
A wallhack that uses the glTexCoordPointer method to achieve great results without logging counts.

Undetected by PB from 2007/12/23 to ~2008/02.

![Hack Screenshot](OpenGl-Wallhack/screenshot.jpg?raw=true "Screenshot Hack")

## Some notes

Some various reversing notes for v1.5.

### Chat-Out
```
00437C3C   8D4424 10        LEA EAX,DWORD PTR SS:[ESP+10]
00437C40   50               PUSH EAX            ; text
00437C41   6A 00            PUSH 0				; format(push 1 places middle of screen, etc.)
00437C4B   E8 F0FBFFFF      CALL CoDMP.00437840
```

![Hack Screenshot](screenshot_chat.jpg?raw=true "Screenshot Hack")

### Drawing Entities
```
004E025E     E8 AD5C0200    CALL CoDMP.00505F10    ;draw world
004E0263  |> E8 E8010000    CALL CoDMP.004E0450    
004E0268  |. E8 83FEFFFF    CALL CoDMP.004E00F0	   ;draw entities
```

Draw entities function can be found via the following string:
```
004E0190     68 EC435800    PUSH CoDMP.005843EC                      ;  ASCII "R_GetShaderByHandle: out of range hShader '%d'
"
004E0195     6A 02          PUSH 2
004E0197     FF15 000B9B01  CALL DWORD PTR DS:[19B0B00]              ;  CoDMP.00412960
```

### Drawing Bullets, Smoke, and Particles
```
004E01AE     6A 00          PUSH 0
004E01B0     6A 00          PUSH 0
004E01B2     6A 00          PUSH 0
004E01B4     8BD0           MOV EDX,EAX
004E01B6     A1 480A9B01    MOV EAX,DWORD PTR DS:[19B0A48]
004E01BB     68 B8215B00    PUSH CoDMP.005B21B8
004E01C0     E8 6BFDFFFF    CALL CoDMP.004DFF30
004E01C5     83C4 10        ADD ESP,10
```

### Drawing Player Structures
```
004E01D1   > 50             PUSH EAX                                 ; /Arg1; Case 1 of switch 004E0164
004E01D2   . E8 E9970300    CALL CoDMP.005199C0                      ; \CoDMP.005199C0
004E01D7   . 83C4 04        ADD ESP,4
```

![Hack Screenshot](screenshot_player.jpg?raw=true "Screenshot Hack")

### Drawing Cars, Foliage, and Dynamic Entities
```
004E01DC   > E8 2F3B0300    CALL CoDMP.00513D10                      ;  Case 2 of switch 004E0164
```

![Hack Screenshot](screenshot_entities.jpg?raw=true "Screenshot Hack")

### Drawing Switch Case
```
004E01E3   > 68 6CBD5800    PUSH CoDMP.0058BD6C                      ;  ASCII 15,"R_AddEntitySurfaces: "; Default case of switch 004E0164
```

### OpenGL Stuff
Addresses:
```
00575BF6     FF15 E0BE9901  CALL DWORD PTR DS:[199BEE0]              ;  opengl32.glDisable
00575C0D     FF15 3CC69901  CALL DWORD PTR DS:[199C63C]              ;  opengl32.glDrawElements
00575C19     FF15 68C49901  CALL DWORD PTR DS:[199C468]              ;  opengl32.glEnable
```

Drawing calls:
```
00508153   CALL DWORD PTR DS:[199C63C]               opengl32.glDrawElements	;draw the sky
005145CC   CALL DWORD PTR DS:[199C63C]               opengl32.glDrawElements	;draw model statics
00518D6C   CALL DWORD PTR DS:[199C63C]               opengl32.glDrawElements	;models, compass, etc.
0051A39F   CALL DWORD PTR DS:[199C63C]               opengl32.glDrawElements	;gun models
```
