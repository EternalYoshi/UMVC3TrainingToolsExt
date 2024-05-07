A tool designed to incrase and improve quality of life of Ultimate Marvel vs Capcom 3's training mode.
This tool adds more options that weren't present before.
There's 3 pages right now.

Explanations:
================================================================================================================================
Extra:
Restart To Intros:
Plays the match the intros once more.

Restart to Ready:
Restarts the Match to the point where the announcer yells READY!!!

Restart Training mode:
Same as before.

Positioning:
Changes the start position of the players. The farthest left is -800 and the farthest right is 800. Below are preset buttons 
that set the positions to specific common scenarios.

Etc: Endless X Factor Toggles.
================================================================================================================================
Status:

Character HP: Should set Life Recovery off in the pause menu.

The Health value gauges are a multiplier. 1.000 is full health, while 0.5 is half health. The Minimum is 0.01.

Hyper Meter. Each level of meter is 10,000. Max is 50,000 while minimum is 0.

Jamming Bomb/Reversed Controls

Should speak for itself, has no visuals but it works.

================================================================================================================================
Character:

FRANK WEST
You can set Frank West's Level, and his Prestige Points will be set to the minimum required for that level.
lvl 1 -   0PP
lvl 2 -   5PP
lvl 3 -  20PP
lvl 4 -  50PP
lvl 5 - 100PP

PHOENIX WRIGHT
You can choose both Mr. Wright's Evidence and have a Turnabout Toggle. Iconography is off but functionally works.

MODOK
Can Control level of Understanding. Affects his Hyper Psionic Blaster and the Big Barrier.

DORMAMMU
Can set level of spells and will lock it to level. Set both to 0 to return behavior to normal.

DEADPOOL
Can set Teleport Counter and can lock it to that number.

Also included is an Endless Install toggle that should work with all installs from the default cast excluding Vale of Mist.

Lastly there's File->Reset to Default which will change all the above settings back to their default values.

================================================================================================================================
Notes:
Dark Phoenix lacks the low wind sound when using the Dark Phoenix Toggle.
Mr. Wright lacks the Gold Aura when using the Turnabout Toggle.
Frank West will keep the level 5 aura when changing his level from 5 to <5. That's not something that can happen
normally in game so there's no real code to handle that. Just restart Training Mode at that point.
MODOK's head flame won't update visuals when setting Level of Understanding.
The Endless Install/X Factor locks the timers at a silly high value, so turning it off WILL allow the timer to decrement, but the timer
is so large it may be wiser to just restart training mode.

Thanks to HKHaan, Sheep, Techs, and Ermaccer for their contributions that made this and the original git of this version possible.

Features will be added to the next release when they are confirmed working problem free.
