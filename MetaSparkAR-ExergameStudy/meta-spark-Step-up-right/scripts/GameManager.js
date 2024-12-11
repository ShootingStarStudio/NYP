import Reactive from 'Reactive';                //handles react code structure
import Patches from 'Patches';                  //links with meta spark studio patches
import Diagnostics from 'Diagnostics';          //console log and debug features
import Time from 'Time';                        //loops, time reference and runtime allowance

//initialize const variables
const Scene = require('Scene');                 //requests scene hierarchy
const targetFrames = 20;

//init logic values from patch
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

// General encapsulation of the various functions required to run every frame
function update(deltaTime) {        // Main update function called every frame, does not run directly  
  getGameState();
}

//proper loop update
// Set up a loop to call the update function every frame
const timeInterval = 1 / targetFrames;
Time.setInterval(() => {
  update(timeInterval);
}, timeInterval * 1000); // Convert to milliseconds
