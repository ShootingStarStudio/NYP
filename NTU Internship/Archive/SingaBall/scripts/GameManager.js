import Time from 'Time'
import Patches from 'Patches'
import Reactive from 'Reactive';
// How to load in modules
const Scene = require('Scene');

// Use export keyword to make a symbol available in scripting debug console
export const Diagnostics = require('Diagnostics');

async function GetGameTime()
{
    let Minutes = (Patches.outputs.getScalarOrFallback('N_Minutes'));
    let Seconds = (Patches.outputs.getScalarOrFallback('N_Seconds'));
    let TotalSeconds = (Patches.outputs.getScalarOrFallback('N_TotalSeconds'));

    if (Minutes.pinLastValue() == 0)
    {
        if (Seconds.pinLastValue() < 10)
        {
            Patches.inputs.setString('GameTime','00 : 0' + Seconds.pinLastValue());
        }
        else
        {
            Patches.inputs.setString('GameTime','00 : ' + Seconds.pinLastValue());
        }
    }
    else if (Seconds.pinLastValue() < 10)
    {
        Patches.inputs.setString('GameTime',Minutes.pinLastValue() + ' : 0' + Seconds.pinLastValue());
    }
    else
    {
        Patches.inputs.setString('GameTime',Minutes.pinLastValue() + ' : ' + Seconds.pinLastValue());
    }
    
    //  0 stars full game duration
    if (TotalSeconds.pinLastValue() <= 0)
    {
        Patches.inputs.setScalar('StarRating',0)
    }
    // 1 star 70-90 secs
    else if (TotalSeconds.pinLastValue() > 0 && TotalSeconds.pinLastValue() <= 20)
    {
        Patches.inputs.setScalar('StarRating',1);
    }
    // 2 star 46-70 secs
    else if (TotalSeconds.pinLastValue() > 20 && TotalSeconds.pinLastValue() <= 44)
    {
        Patches.inputs.setScalar('StarRating',2);
    }
    // 3 star 45 secs and below 
    else if(TotalSeconds.pinLastValue() >= 45)
    {
        Patches.inputs.setScalar('StarRating',3);
    }
  }

async function GetTime()
{
    let time = Patches.outputs.getScalarOrFallback("GetTime");
    await Patches.inputs.setScalar("timer",time.pinLastValue());
}

(async function() {  // Enables async/await in JS [part 1]

    const GameoverInterval = 0.5; 
    const trackInterval = 1;
    async function Update()
    {
        GetTime();
        let gameover = (Patches.outputs.getBooleanOrFallback('GameOver'));
        if (gameover.pinLastValue() == true)
          {
              Patches.inputs.setBoolean('GameIsOver',true);
              GetGameTime();
          }
    }

    const intervalTimer = Time.setInterval(Update,GameoverInterval);
})(); 
