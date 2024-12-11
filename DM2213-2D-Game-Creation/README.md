# 2D Game Engine
 Simple 2D tile based game engine 

A brave archeologist stumbles onto a map that points to treasure,
deep in the jungle. Full of treasure, but equally dangerous.
Can she survive and retreive the treasure?

"A" & "D" to move left and right
"W" & "S" to climb up and down
"Space" to Jump
"E" to interact with items, ie levers/doors with locks
"B" to build. uses sticks from inventory to build climbable scaffolding at player position, works the same way as ladder/vine
"C" for melee, breaks blocks
"V" for ranged, if bow collected, deals damage at range

features
swamp slows down player
vines/ladders/scaffolding allow player to climb up and down
mushroom allows higher jump height
Bamboo Spikes damage you, made by the ancient builders who made the temple
breaking shrubs and mango leaves drop sticks and mango.
herbs heal you for 25% of health
bow and arrow allows players to hit enemies, or targer tiles, similar to a button. 
use machete to cut shrubs to get sticks
and harvest mangoes. 

pick up key to open jail cell
press levers/hit targets to open doors

first level, gather sticks to rekindle the campfire and harvest mangoes for dinner.
return to campfire to get to next level.

2nd level: entrance to temple
use of scaffolding to get to new areas
use of bow and arrow to hit target blocks, which opens door

3rd level: collect treasure
collect key to open chest
use of lever open door

interacting with the key spawns a golem boss
    plan to implement more types of activation, ie lever, chest and what not

you win when you exit last level

========================================
tile list

ground 100
brick 101
herb 2
bamboo 20
campfire 99

swamp 98
vine 97
scaffold 96
mushroom 110

bow 3
quiver 4
Trunk 111
Leaves 112
MangoLeaves 113
Shrub 114
Mango 5
Stick 6

left arrow 10
right arrow 11
target 115
lever off 12
lever on 13
door closed 116
door open 14

key 15
treasure chest 16
open chest 17

new player atlas 64x64
13x7
832x448
idleL 0 1
idleR 13 14
walkL 0 8
walkR 13 21
climb 26 34
slashL 39 44
slashR 45 50
shootL 52 64
shootR 65 77
death 78 83

golem spawns as defense state, enters idle and moves during LOS
attacks with laser(disabled)
when health is low, it will go into defense mode, stopping movement and restricting back into a rock. it cannot take damage and will heal a small amount before going back to offensive mode

flaws
    player animation is inconsistent with movement and action
        breaking objects and shooting arrow happens instantly, while animation takes abit longer
    menu control.
        does not have proper win or lose screens
        plans to include proper settings and sound management. 
        pause menu is not yet implemented
    enemies are lacklustre
        plan to add more enemies
            minions that spawn during boss fight
    
    uniqueness is poor, 
        plan to add more content and darkness system
            torch to be picked up, which illuminates the area around the player. 
            however, the entire map will be pitch black. the other use for the torch is to show enemies
    
    better shooting system
        charge meter, not limited to left and right, ie diagonals
        can improve to use grappling hook
    
    
