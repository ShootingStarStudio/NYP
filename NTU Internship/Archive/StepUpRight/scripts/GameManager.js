import Patches from 'Patches';
import Reactive from 'Reactive';
import Time from 'Time';

const Scene = require('Scene');

export const Diagnostics = require('Diagnostics');

//let pos = (Patches.outputs.getPoint2DOrFallback('SetPlayerPos'));
//await Patches.inputs.setPoint2D('PlayerPos', pos.pinLastValue());

//await Patches.inputs.setPulse('resetGreenAnimation', Reactive.once());
//let collided = (Patches.outputs.getBooleanOrFallback('GreenCollisionCheck'));

// Level Info
async function GetLevelInfo() {
  let lvl = (Patches.outputs.getScalarOrFallback('SetLevelNum'));
  await Patches.inputs.setScalar('LevelNum', lvl.pinLastValue());

  let steps = (Patches.outputs.getScalarOrFallback('SetSteps'));
  await Patches.inputs.setScalar('Steps', steps.pinLastValue());
}

async function GetGameState() {
  let state = (Patches.outputs.getScalarOrFallback('SetState'));
  await Patches.inputs.setScalar('State', state.pinLastValue());
}

(async function () {
  const TimeInterval = 0.5;

  function Update() {
    GetLevelInfo();
    GetGameState();
  }

  const intervalTimer = Time.setInterval(Update, TimeInterval)
})();
