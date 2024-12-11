import Patches, { outputs } from 'Patches';
import Reactive from 'Reactive';
import Time from 'Time';

const Scene = require('Scene');

// Use export keyword to make a symbol available in scripting debug console
export const Diagnostics = require('Diagnostics');

// Enemy info
async function UpdateEnemy1Pos() {
    let pos = (Patches.outputs.getPoint2DOrFallback('SetEnemy1Pos'));
    let value = pos.pinLastValue();
    await Patches.inputs.setPoint2D('Enemy1Pos', value);
}

async function UpdateEnemy2Pos() {
    let pos = (Patches.outputs.getPoint2DOrFallback('SetEnemy2Pos'));
    let value = pos.pinLastValue();
    await Patches.inputs.setPoint2D('Enemy2Pos', value);
}

// Projectile Info

async function GetProjSize() {
    let result = (Patches.outputs.getScalarOrFallback('SetProjSize'));
    await Patches.inputs.setScalar('ProjSize', result.pinLastValue());
}

// Player Info
async function GetPlayerInfo() {
    let fWidth = (Patches.outputs.getScalarOrFallback('SetPlayerWidth'));
    await Patches.inputs.setScalar('PlayerWidth', fWidth.pinLastValue());

    let vPos = (Patches.outputs.getPoint2DOrFallback('SetPlayerPos'));
    await Patches.inputs.setPoint2D('PlayerPos', vPos.pinLastValue());
}

async function GotHit() {
    let hit = (Patches.outputs.getBooleanOrFallback('GotHit'));
    if (hit.pinLastValue() == true)
        await Patches.inputs.setPulse('TriggerStun', Reactive.once());
}

async function GetTime() {
    let time = (Patches.outputs.getScalarOrFallback('GetTime'));
    await Patches.inputs.setScalar('UpdateTime', time.pinLastValue());
}

(async function () {  // Enables async/await in JS [part 1]
    // Set Player Pos & Size

    // Update() called every Time Interval
    const TimeInterval = 0.5;

    function Update() {
        UpdateEnemy1Pos();
        UpdateEnemy2Pos();

        GetPlayerInfo();
        GetProjSize();
        GetTime();

        GotHit();
    }

    const intervalTimer = Time.setInterval(Update, TimeInterval)
})(); // Enables async/await in JS [part 2]
