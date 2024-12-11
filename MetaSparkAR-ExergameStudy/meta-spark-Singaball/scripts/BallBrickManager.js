import Time from 'Time'
import Patches from 'Patches'
import Reactive from 'Reactive'
import Diagnostics from 'Diagnostics'

const Scene = require('Scene');

const targetFrames = 20;
const timeInterval = 1 / targetFrames;

//ball 
async function checkcatchball() {
    let playercatch = (Patches.outputs.getBooleanOrFallback('Player_catch'));
    if (playercatch.pinLastValue() == true) {
        let BallProg = (Patches.outputs.getScalarOrFallback('BallProg'));
        if (BallProg.pinLastValue() == 1) {
            await Patches.inputs.setPulse('PlayBallAnimation', Reactive.once());
            await Patches.inputs.setPulse('ReverseBallDirection', Reactive.once());
        }
    }
}

async function RebounceWall() {
    let resetBallProgress = (Patches.outputs.getBooleanOrFallback('ResetBallProgress'));
    if (resetBallProgress.pinLastValue() == true) {
        await Patches.inputs.setPulse('PlayBallAnimation', Reactive.once());
        let BallProg = (Patches.outputs.getScalarOrFallback('BallProg'));
        if (BallProg.pinLastValue() == 0) {
            await Patches.inputs.setPulse('UnReverse', Reactive.once());
        }
    }
}

//brick
async function GeneratePosition() {
    let GeneratePosition = (Patches.outputs.getBooleanOrFallback('BGenerateRandomPos'))
    if (GeneratePosition.pinLastValue() == true) {
        await Patches.inputs.setPulse('PF_GeneratePosition', Reactive.once());
    }
}

async function updateBrickTarget() {
    let isIncrementing = (Patches.outputs.getBooleanOrFallback('BIncrementBrickTarget'));
    if (isIncrementing.pinLastValue() == true) {
        await Patches.inputs.setPulse('PF_IncrementBrickTarget', Reactive.once());
    }

    let InactiveBrick = (Patches.outputs.getBooleanOrFallback('BInactiveBrick'));
    if (InactiveBrick.pinLastValue() == true) {
        await Patches.inputs.setPulse('PF_IncrementBrickTarget', Reactive.once());
    }
}

async function UpdateBrickLives() {
    let BrickLives = (Patches.outputs.getScalarOrFallback('N_BrickLives'));
    await Patches.inputs.setScalar("NF_BrickLives", BrickLives.pinLastValue());
}

async function IncreaseLevel() {
    let LevelIncreasing = (Patches.outputs.getBooleanOrFallback('BIncreaseLevel'));
    if (LevelIncreasing.pinLastValue() == true) {
        await Patches.inputs.setPulse('PF_IncreaseLevel', Reactive.once());
    }
}

(async function () {
    async function Update() {
        if ((Patches.outputs.getScalarOrFallback('setGameState')).pinLastValue() == 1) {
            //ball
            checkcatchball();
            RebounceWall();
            //brick
            UpdateBrickLives();
            updateBrickTarget();

            GeneratePosition();
            IncreaseLevel();
        }
    }

    const intervalTimer = Time.setInterval(Update, timeInterval * 1000);
})();
