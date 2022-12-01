# Gladiator

## <h2 id="top">Summary</h2>
---
-[Currently implemented](#implements)   
-[Controls](#ctrl)  
-[Screenshots](#screens)  
-[Additionnal notes](#notes)  

## <h2 id="implements">Currently implemented</h2>
---
- Event based UI
- GameState and GameMode to switch between in-game and game over  
- Enemy AI controlled using a behavior tree and an AI director  
    - Enemies place themselves in a radius around the player to have a line of sight
    - Enemies attack one by one and replace themselves when done
    - Enemies dodge when player approaches

- Sound when attacking  
- Camera boom ignoring enemies but not terrain
- Player blocks in a cone in front of him
- When a character gets hit, it blinks in red
- Attacks are done in a small AOE in front of the caster
- Player has a backdash with a small cooldown
- Attacking and blocking prevents movement  

## <h2 id="ctrl">Controls</h2>
---
- Use WASD (Qwerty) to move the player
- Pres leftShift to backdash
- Press left click to attack
- Press right click to block  
- Press K/L to damage/heal yourself (debug)

## <h2 id="screens">Screenshots</h2>
---
Example of the player blocking
![Blocking](./ReadmeResources/blocking.gif)  
Example of the player attacking
![Attacking](./ReadmeResources/attack.gif)  
Example of the backdash
![Backdash](./ReadmeResources/backdash.gif)  
Gameplay loop, combining every element of the game
![Gameplay](./ReadmeResources/gameplay.gif)  

## <h2 id="notes">Additionnal notes</h2>
---
**Recommended strategy:**  
Enemies will try to avoid you, rushing backwards faster than you can go, however, enemies stop moving for a short time after attacking, making it a perfect opening for an attack. You can also try to time your attack and your backdash to try and hit an enemy before it hits you.  

**Useful informations:**  
Attacking while blocking will lower your shield for the duration of the attack, raising it again once it's finished.  
The backdash's cooldown is represented by a light blue bar under the health bar, only showing up when the cooldown is active.  
Enemies can't go up the stairs by themselves.  

[Top of the page](#top)