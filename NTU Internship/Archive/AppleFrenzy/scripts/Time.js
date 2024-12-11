import Patches from 'Patches';
import Reactive from 'Reactive';
import Time from 'Time';

const Scene = require('Scene');
const Diagnostics = require('Diagnostics');

async function GetTime() {
  let time = (Patches.outputs.getScalarOrFallback('SetTime'));
  await Patches.inputs.setScalar('Time', time.pinLastValue());
}

(async function () {
  const TimeInterval = 1;

  function Update() {
    GetTime();
  }

  const intervalTimer = Time.setInterval(Update, TimeInterval)
})();
