import Time from 'Time'
import Reactive from 'Reactive'
import Patches from 'Patches'

const Scene = require('Scene');

// Use export keyword to make a symbol available in scripting debug console
export const Diagnostics = require('Diagnostics');


async function IsEnerygyFull()
{
    let EnergyFull = (Patches.outputs.getBooleanOrFallback('FullEnergy'));
    if (EnergyFull.pinLastValue() == true)
    {
        await Patches.inputs.setBoolean('StopChargingEnergy',true);
    }
    else if (EnergyFull.pinLastValue() == false)
    {
        await Patches.inputs.setBoolean('StopChargingEnergy',false);
    }
}




(async function() {  // Enables async/await in JS [part 1]

  const Interval = 0.5; 
  function CaughtBall(){
    Patches.inputs.setPulse('EnergyUsed',Reactive.once());
    
  }
  let PlayerCatchBall = (Patches.outputs.getPulseOrFallback('PlayerCatchBall'));
  let PlayerCatchFireball0 = (Patches.outputs.getPulseOrFallback('PlayerCatchFireball0'));
  let PlayerCatchFireball1 = (Patches.outputs.getPulseOrFallback('PlayerCatchFireball1'));
  let PlayerCatchFireball2 = (Patches.outputs.getPulseOrFallback('PlayerCatchFireball2'));
  PlayerCatchBall.subscribe(CaughtBall);
  PlayerCatchFireball0.subscribe(CaughtBall);
  PlayerCatchFireball1.subscribe(CaughtBall);
  PlayerCatchFireball2.subscribe(CaughtBall);
  async function Update()
  {
      IsEnerygyFull();
  }

  const intervalTimer = Time.setInterval(Update,Interval);
})(); 

