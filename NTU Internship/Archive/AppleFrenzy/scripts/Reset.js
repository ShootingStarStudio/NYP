import Reactive from 'Reactive'
import Patches from 'Patches';
import Time from 'Time'

const Diagnostics = require('Diagnostics');
const Random = require('Random');

//var minDuration:ScalarSignal;
//var maxDuration:ScalarSignal;
;
async function regenerate(){
 
    await Patches.inputs.setPulse('resetAnimation', Reactive.once());
    Time.setTimeout(async () => {
        await regenerate();
    }, Random * 1000);
}

(async function () {

    const appleFallTimer = 1;
    function appleCollide()
    {
        let collided = (Patches.outputs.getBooleanOrFallback('collisionCheck'));
        if(collided.pinLastValue()== true){

            regenerate();
        }
	}
    
    const intervalTimer = Time.setInterval(appleCollide,appleFallTimer )
        
})();