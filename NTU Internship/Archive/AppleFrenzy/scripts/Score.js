const Scene = require('Scene');
const Patches = require('Patches');

const Diagnostics = require('Diagnostics');

Promise.all([
	Scene.root.findFirst('scoreText'),
	Patches.outputs.getScalar('Score')
]).then(function(results){
	var counter = results[0];
	var scorestring = results[1];
	
	counter.text = scorestring.toString();
});


