


Pebble.addEventListener("ready", function() {
	console.log("Ready");
	var msg = { 'count': 30 };
	Pebble.sendAppMessage(msg);
});

Pebble.addEventListener("appmessage", function(e) {
	console.log("Received:" + e.payload.index);		
});

