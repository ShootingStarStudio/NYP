import Patches from 'Patches';

// How to load in modules
const Scene = require('Scene');

// Use export keyword to make a symbol available in scripting debug console
export const D = require('Diagnostics');

(async function () {  // Enables async/await in JS [part 1]

    //let fWidth = (Patches.outputs.getScalarOrFallback('SetPlayerWidth'));
    //let widthValue = fWidth.pinLastValue();
    //await Patches.inputs.setScalar('PlayerWidth', widthValue);
    //D.log(fWidth);

})(); // Enables async/await in JS [part 2]
