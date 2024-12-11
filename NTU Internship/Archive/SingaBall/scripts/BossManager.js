import Time from 'Time'
import Reactive from 'Reactive'
import Patches from 'Patches'


const Scene = require('Scene');

// Use export keyword to make a symbol available in scripting debug console
export const Diagnostics = require('Diagnostics');

async function StartBossFight()
{
    let startbossfight = (Patches.outputs.getBooleanOrFallback('BStartBossFight'));
    if (startbossfight.pinLastValue() == true)
    {
        Patches.inputs.setBoolean('BF_StopBossCountdown',true);
        Patches.inputs.setPulse('BF_RandomizeBossDestination',Reactive.once());
        Patches.inputs.setPulse('MoveBoss',Reactive.once());
    }
}

async function CheckBossProgress()
{
    let BossProgress = (Patches.outputs.getScalarOrFallback('BossProgress'));
    let FinishedAttacking = (Patches.outputs.getBooleanOrFallback('BossFinishedAttacking'));

    if (BossProgress.pinLastValue() == 1 && FinishedAttacking.pinLastValue() == true)
    {
        Patches.inputs.setPulse('ResetBossAnimation',Reactive.once());
        Patches.inputs.setPulse('BF_RandomizeBossDestination',Reactive.once());
        Patches.inputs.setPulse('MoveBoss',Reactive.once());
    }
}

async function CatchFireBalls()
{
    let fireballstatus0 = (Patches.outputs.getBooleanOrFallback('CatchFireBall_0'));
    let fireballstatus1 = (Patches.outputs.getBooleanOrFallback('CatchFireBall_1'));
    let fireballstatus2 = (Patches.outputs.getBooleanOrFallback('CatchFireBall_2'));

    let fireballprog0 = (Patches.outputs.getScalarOrFallback('FireBallprog0'));
    let fireballprog1 = (Patches.outputs.getScalarOrFallback('FireBallprog1'));
    let fireballprog2 = (Patches.outputs.getScalarOrFallback('FireBallprog2'));
    
    if (fireballstatus0.pinLastValue() == true && fireballprog0.pinLastValue() == 1)
    {
        Patches.inputs.setPulse('ReverseFireBall_0',Reactive.once());
        Patches.inputs.setPulse('PlayFireBall_0',Reactive.once());
    }

    if (fireballstatus1.pinLastValue() == true && fireballprog1.pinLastValue() == 1)
    {
        Patches.inputs.setPulse('ReverseFireBall_1',Reactive.once());
        Patches.inputs.setPulse('PlayFireBall_1',Reactive.once());
    }

    if (fireballstatus2.pinLastValue() == true && fireballprog2.pinLastValue() == 1)
    {
        Patches.inputs.setPulse('ReverseFireBall_2',Reactive.once());
        Patches.inputs.setPulse('PlayFireBall_2',Reactive.once());
    }
}

async function UpdateBossHealth()
{
    let N_BossHealth = (Patches.outputs.getScalarOrFallback('N_BossHealth'));
    Patches.inputs.setScalar('FBossHealth',N_BossHealth.pinLastValue());
}


(async function () {  
    const TimeInterval = 0.1;
    async function Update()
    {
        let gameover = (Patches.outputs.getBooleanOrFallback('GameOver'));
        if (gameover.pinLastValue() == true)
            return;
        StartBossFight();
        CheckBossProgress();
        CatchFireBalls();
        UpdateBossHealth();
    }
    const intervalTimer = Time.setInterval(Update,TimeInterval);
  })(); 
