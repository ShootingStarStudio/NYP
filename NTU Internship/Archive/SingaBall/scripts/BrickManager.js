import Time from 'Time'
import Patches from 'Patches'
import reactive from 'Reactive'
import { debug } from 'console';

// How to load in modules
const Scene = require('Scene');

// Use export keyword to make a symbol available in scripting debug console
export const Diagnostics = require('Diagnostics');


async function GeneratePosition()
{ 
    let GeneratePosition = (Patches.outputs.getBooleanOrFallback('BGenerateRandomPos'))
    if (GeneratePosition.pinLastValue() == true)
    {
      await Patches.inputs.setPulse('PF_GeneratePosition',reactive.once());
    }
}

async function IncrementBrickTarget()
{
    //let IsIncrementing = (Patches.outputs.getBooleanOrFallback('PIncrementBrickTarget'));
    //let IsPulsing = (Patches.outputs.getPulseOrFallback('PIncrementBrickTarget'));
    
    //if (IsPulsing.pinLastValue() == reactive.val(true))
    //{   
    //    await Patches.inputs.setPulse('BP_IncrementBrickTarget',reactive.once());
    //}
    let isIncrementing = (Patches.outputs.getBooleanOrFallback('BIncrementBrickTarget'));

    if (isIncrementing.pinLastValue() == true)
    {
        await Patches.inputs.setPulse('PF_IncrementBrickTarget',reactive.once());
    }

    
}

async function InactiveBrickIncrement()
{
    let InactiveBrick = (Patches.outputs.getBooleanOrFallback('BInactiveBrick'));

    if (InactiveBrick.pinLastValue() == true)
    {
        await Patches.inputs.setPulse('PF_IncrementBrickTarget',reactive.once());
    }
}

async function IncreaseLevel()
{
    let LevelIncreasing = (Patches.outputs.getBooleanOrFallback('BIncreaseLevel'));
    if (LevelIncreasing.pinLastValue() == true)
    {
        await Patches.inputs.setPulse('PF_IncreaseLevel',reactive.once());
    }
}

async function UpdateBrickLives()
{
    let BrickLives = (Patches.outputs.getScalarOrFallback('N_BrickLives'));
    await Patches.inputs.setScalar("NF_BrickLives",BrickLives.pinLastValue());
}


(async function () {  
    const TimeInterval = 0.5;
    const FastInterval = 0.1;
    async function Update()
    {
        let gameover = (Patches.outputs.getBooleanOrFallback('GameOver'));
        if (gameover.pinLastValue() == true)
            return;
        GeneratePosition();
        IncrementBrickTarget();
        IncreaseLevel();
        InactiveBrickIncrement();

    }

    async function FastUpdate()
    {
        let gameover = (Patches.outputs.getBooleanOrFallback('GameOver'));
        if (gameover.pinLastValue() == true)
            return;
        UpdateBrickLives();
    }
    const intervalTimer = Time.setInterval(Update,TimeInterval);
    const FastintervalTimer = Time.setInterval(FastUpdate,FastInterval);
})();
