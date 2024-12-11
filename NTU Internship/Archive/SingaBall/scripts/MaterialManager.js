
const Scene = require('Scene');
const Patches = require('Patches');
const Material = require('Materials');
const reactive = require('Reactive');
const Time = require('Time');
const Block = require('Blocks');
export const Diagnostics = require('Diagnostics');


async function ChangeBrick0Material()
{
  var matname = Patches.outputs.getStringOrFallback("matname");

  var mat = matname.pinLastValue()+''.toString();
  Scene.root.findFirst("Brick0").material= Material.getAll(mat);
  Diagnostics.log(matname.pinLastValue());
}


(async function () { 

    const TimeInterval = 0.5;
    async function Update()
    {
      //ChangeBrick0Material();
    }
    const intervalTimer = Time.setInterval(Update,TimeInterval);
})();
