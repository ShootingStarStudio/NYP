import Patches from 'Patches';
import Reactive from 'Reactive';
import Time from 'Time';

const Scene = require('Scene');

// Use export keyword to make a symbol available in scripting debug console
export const D = require('Diagnostics');

async function ShowGameOver() {

    await Patches.inputs.setPulse('stopGame', Reactive.once());
}

async function ResetDisplay() {

    await Patches.inputs.setPulse('reset', Reactive.once());
}

(async function () {  // Enables async/await in JS [part 1]
    const TimeInterval = 0.5;

    function resetGame()
    {
        let isOver = (Patches.outputs.getBooleanOrFallback('SetIsOver'));
        if (isOver.pinLastValue() == true) {
            ShowGameOver();
        }
        else {
            ResetDisplay();
        }
    }

    const intervalTimer = Time.setInterval(resetGame, TimeInterval);

})(); // Enables async/await in JS [part 2]