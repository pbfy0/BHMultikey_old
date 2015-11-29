# BHMultikey
Multi-keyboard support for Brawlhalla

This program allows using multiple keyboards on one computer in Brawlhalla.

## Instructions
This program depends on the Visual C++ runtime. You'll need to download it from [here](https://www.microsoft.com/en-us/download/details.aspx?id=48145). Make sure to download the same version (32 or 64 bit) as the program.  
By default, this program supports both standard keyboard control schemes (`WASD HJKL` and `???? VCXZ`). The keys can be rebound on a per-keyboard basis.  
To use, replace BrawlhallaAir.swf and Game.swz from the Brawlhalla directory with the ones in the archive. BHMultikey.exe must be running to actually enable multi-keyboard functionality. Make sure to launch BHMultikey.exe before leaving the main menu.

## Extras
- `;` toggles jumping with the up key (default is off)
- `` ` `` (backtick) can be used to rebind controls. Press `` ` ``, then press the desired controls in this order:
  - up, left, down, right, jump, throw, light, heavy, dodge
- Press `` ` `` again to disable remapped controls or cancel remapping.

## Caveats
- Tooltips will show controller button names instead of keys while the program is active
- In-game chat will not work, and things typed into steam chat will also be interpreted as inputs
- The whole program is a bit hacked together, so crashes may happen. Error reporting is disabled because any issues that occur are much more likely my fault than BMG's
- You'll have to rebind controls every time you launch BHMultikey.exe
