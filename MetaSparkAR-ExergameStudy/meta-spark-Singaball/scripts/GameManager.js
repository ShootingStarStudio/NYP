import Time from 'Time'
import Patches from 'Patches'
import Reactive from 'Reactive'
import Diagnostics from 'Diagnostics'

const Scene = require('Scene');

const targetFrames = 20;
const timeInterval = 1 / targetFrames;

let currState = 0;     //compare internal script state to the patch state, if different
let s_GameState = Reactive.val(0);

async function getGameState() {
  s_GameState = (Patches.outputs.getScalarOrFallback('setGameState'));
  //if currstate not gamestate then send a pulse to the patch 
  if (s_GameState.pinLastValue() != currState) {
    currState = s_GameState.pinLastValue();
    await Patches.inputs.setPulse('p_ResetTime', Reactive.once());       // Send a pulse to indicate the state change
    Diagnostics.log(`Game State: ${currState}`);
  }
  //output the state regardless
  await Patches.inputs.setScalar('s_GameState', s_GameState.pinLastValue());
}

async function convertTimeToStar() {
    //using hard coded thresholds, return a rating to light up the stars
    let totalTime = (Patches.outputs.getScalarOrFallback("getTime"));
    let rating = 0;

    //0 stars
    if (totalTime.pinLastValue() <= 0) { rating = 0; }
    //1 stars
    else if (totalTime.pinLastValue() <= 20) { rating = 1; }
    //2 stars
    else if (totalTime.pinLastValue() <= 45) { rating = 2; }
    //3 stars
    else { rating = 3; }

    Patches.inputs.setScalar('StarRating', rating)
}

async function GetGameTime() {
    let totalTime = (Patches.outputs.getScalarOrFallback("getTime"));
    let minutes = Math.floor(totalTime.pinLastValue() / 60);
    let seconds = Math.floor(totalTime.pinLastValue() % 60);
    // Format minutes and seconds into MM:SS
    let formattedTime = `${String(minutes).padStart(2, '0')}:${String(seconds).padStart(2, '0')}`;
    Patches.inputs.setString('GameTime', formattedTime);
}

async function IsEnerygyFull() {
    let EnergyFull = (Patches.outputs.getBooleanOrFallback('FullEnergy'));
    await Patches.inputs.setBoolean('StopChargingEnergy', EnergyFull.pinLastValue());
}

(async function () {  // Enables async/await in JS [part 1]
    function CaughtBall() {
        Patches.inputs.setPulse('EnergyUsed', Reactive.once());
    }
    let PlayerCatchBall = (Patches.outputs.getPulseOrFallback('PlayerCatchBall'));
    let PlayerCatchFireball0 = (Patches.outputs.getPulseOrFallback('PlayerCatchFireball0'));
    let PlayerCatchFireball1 = (Patches.outputs.getPulseOrFallback('PlayerCatchFireball1'));
    let PlayerCatchFireball2 = (Patches.outputs.getPulseOrFallback('PlayerCatchFireball2'));
    PlayerCatchBall.subscribe(CaughtBall);
    PlayerCatchFireball0.subscribe(CaughtBall);
    PlayerCatchFireball1.subscribe(CaughtBall);
    PlayerCatchFireball2.subscribe(CaughtBall);

    async function Update() {
        getGameState();
        IsEnerygyFull();

        s_GameState = (Patches.outputs.getScalarOrFallback('setGameState'));
        if (s_GameState.pinLastValue() == 1) {
            GetGameTime();          //update result's ingame time text
            convertTimeToStar();    //dont change the star rating during results
        }
    }

    const intervalTimer = Time.setInterval(Update, timeInterval * 1000);
})(); 
