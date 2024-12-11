import reactive from 'Reactive'
import Patches from 'Patches'
import Time from 'Time'
import { time } from 'console';
import { on } from 'process';

// How to load in modules
const Scene = require('Scene');

// Use export keyword to make a symbol available in scripting debug console
export const Diagnostics = require('Diagnostics');



async function checkcatchball(_Gameover)
{
    let playercatch = (Patches.outputs.getBooleanOrFallback('Player_catch'));
    if (playercatch.pinLastValue() == true)
    {
        let BallProg = (Patches.outputs.getScalarOrFallback('BallProg'));
        if (BallProg.pinLastValue() == 1)
        {
            await Patches.inputs.setPulse('PlayBallAnimation',reactive.once());
            await Patches.inputs.setPulse('ReverseBallDirection',reactive.once());
        }
    }

    //if (PlayerCatchBall.pinLastValue() == on)
    //{
    //    Diagnostics.log('Catch Pulse Logged');
    //}
}

async function RebounceWall()
{
    let resetBallProgress = (Patches.outputs.getBooleanOrFallback('ResetBallProgress'));
    if (resetBallProgress.pinLastValue() == true)
    {
        //await Patches.inputs.setPulse('ReverseBallDirection',reactive.once());
        await Patches.inputs.setPulse('PlayBallAnimation',reactive.once());
        let BallProg = (Patches.outputs.getScalarOrFallback('BallProg'));
        if (BallProg.pinLastValue() == 0)
            {
                await Patches.inputs.setPulse('UnReverse',reactive.once());
            }
            //Diagnostics.log("Reset Ball progress");
    }
}


(async function() {  // Enables async/await in JS [part 1]

    const TimeInterval = 0.1;
    async function Update()
    {
        let gameover = (Patches.outputs.getBooleanOrFallback('GameOver'));
        if (gameover.pinLastValue() == true)
            return;
        checkcatchball();
        RebounceWall();
    }
    const intervalTimer = Time.setInterval(Update,TimeInterval);
    
})(); 
