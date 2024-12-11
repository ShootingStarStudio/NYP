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

//collision result
async function handleCollision() {
  let collided = Patches.outputs.getBooleanOrFallback('collisionTrigger1');
  if (collided.pinLastValue() == true) {
    Patches.inputs.setPulse('p_hit1', Reactive.once());
    Diagnostics.log(`Apple 1: Hit`);
  };
  let collided2 = Patches.outputs.getBooleanOrFallback('collisionTrigger2');
  if (collided2.pinLastValue() == true) {
    Patches.inputs.setPulse('p_hit2', Reactive.once());
    Diagnostics.log(`Apple 2: Hit`);
  };
  let collided3 = Patches.outputs.getBooleanOrFallback('collisionTrigger3');
  if (collided3.pinLastValue() == true) {
    Patches.inputs.setPulse('p_hit3', Reactive.once());
    Diagnostics.log(`Apple 3: Hit`);
  };
  let collided4 = Patches.outputs.getBooleanOrFallback('collisionTrigger4');
  if (collided4.pinLastValue() == true) {
    Patches.inputs.setPulse('p_hit4', Reactive.once());
    Diagnostics.log(`Apple 4: Hit`);
  };
}

// General encapsulation of the various functions required to run every frame
function update(deltaTime) {        // Main update function called every frame, does not run directly, 
  getGameState();
  handleCollision();
}

//proper loop update
// Set up a loop to call the update function every frame
const timeInterval = 1 / targetFrames;
Time.setInterval(() => {
  update(timeInterval);
}, timeInterval * 1000); // Convert to milliseconds
