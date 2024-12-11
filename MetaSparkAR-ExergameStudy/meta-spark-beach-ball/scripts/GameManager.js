import Patches from 'Patches';
import Reactive from 'Reactive';
import Time from 'Time';
const Scene = require('Scene');
const Diagnostics = require('Diagnostics');

const targetFrames = 20;

let currState = 0;     //compare internal script state to the patch state, if different
let s_gameState = Reactive.val(0);

async function getGameState() {
    s_gameState = (Patches.outputs.getScalarOrFallback('setGameState'));
    //if currstate not gamestate then send a pulse to the patch 
    if (s_gameState.pinLastValue() != currState) {
        currState = s_gameState.pinLastValue();
        await Patches.inputs.setPulse('p_ResetTime', Reactive.once());       // Send a pulse to indicate the state change
        Diagnostics.log(`Game State: ${currState}`);
    }
    //output the state regardless
    await Patches.inputs.setScalar('s_GameState', s_gameState.pinLastValue());
}

async function getGameInfo() {
    let fWidth = (Patches.outputs.getScalarOrFallback('SetPlayerWidth'));
    await Patches.inputs.setScalar('PlayerWidth', fWidth.pinLastValue());

    let size = (Patches.outputs.getScalarOrFallback('SetProjSize'));
    await Patches.inputs.setScalar('ProjSize', size.pinLastValue());
}

//collision response, trigger stun
async function GotHit() {
    let hit = (Patches.outputs.getBooleanOrFallback('GotHit'));
    if (hit.pinLastValue() == true)
        await Patches.inputs.setPulse('TriggerStun', Reactive.once());
}

//fix this, remove game over state and replace with end game, ie complete all levels before time runs out
async function isGameOver(){
    let isOver = (Patches.outputs.getBooleanOrFallback('gameOver'));
    await Patches.inputs.setBoolean('b_EndGame', isOver.pinLastValue());
    if (isOver.pinLastValue() == true) {                                        //debug, delete when complete
        Diagnostics.log("stop game early, exit exception");
    }
}

// General encapsulation of the various functions required to run every frame
async function update(deltaTime) {        // Main update function called every frame, does not run directly
    isGameOver();
    getGameState();
    getGameInfo();
    GotHit();
}

//proper loop update
// Set up a loop to call the update function every frame
const timeInterval = 1 / targetFrames;
Time.setInterval(() => {
    update(timeInterval);
}, timeInterval * 1000); // Convert to milliseconds
