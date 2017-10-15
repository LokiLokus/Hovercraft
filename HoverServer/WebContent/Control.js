var eventId = document.getElementById("eventId");
var eventArgs = document.getElementById("eventArgs");
var eventLittleEndian = document.getElementById("littleEndian");
var wsUrl = document.getElementById("wsUrl");

var currentWsUrl = "";
var ws;

var events = [
	//control related events
	"hover", //sets the speed of the hover motor
	"forward", //sets the speed of the manouvering motor
	"steering", //sets the steering
	"stopall", //stops all motors
	"turbo", //enable/disable the turbo
	"autopilot", //enable/disable the autopilot

	//network related events
	"connect", //emitted when a new main client connects
	"disconnect", //emitted when the main client disconnects
	"ping", //sent by a client to ping the sever
	"pong", //response to ping

	//misc events
	"init", //used to tell a client if he is in control
	"photo" //used to send a photo to clients (periodically?)
];

for(var i = 0; i < events.length; i++)
{
	var opt = document.createElement("option");
	opt.value = i;
	opt.innerHTML = events[i];
	eventId.appendChild(opt);
}

/*setInterval(function()
{
	var val = parseInt(eventArgs.value);
	if(val >= 500)
		val = -400;
	else if(val < 500)
		val = 1400;

	eventArgs.value = val;
	sendEvent();
}, 600);*/

function sendEvent()
{
	if(wsUrl.value != currentWsUrl)
	{
		ws = new WebSocket(wsUrl.value);
		currentWsUrl = wsUrl.value;

		ws.onopen = sendEvent;
		return;
	}

	var id = eventId.selectedIndex;
	var args = eventArgs.value.split(" ");
	var littleEndian = eventLittleEndian.checked;

	if(args.length == 1 && args[0].trim() == "")
		args = [];

	var buff = new ArrayBuffer(16 + args.length * 4);
	var data = new DataView(buff);

	var now = Date.now();
	if(littleEndian)
	{
		data.setUint32(0, now & 0xFFFFFFFF, true);
		data.setUint32(4, now >> 32, true);
	}
	else
	{
		data.setUint32(0, now >> 32, false);
		data.setUint32(4, now & 0xFFFFFFFF, false);
	}

	data.setUint32(8, id, littleEndian);
	data.setUint32(12, args.length * 4, littleEndian);

	for(var i = 0; i < args.length; i++)
	{
		data.setInt32(16 + i * 4, parseInt(args[i]), littleEndian);
		//data.setUint8(16 + i, parseInt(args[i]));
	}

	var dump = "";
	for(var i = 0; i < data.byteLength; i++)
	{
		dump += " " + ("00" + data.getUint8(i).toString(16)).slice(-2);
	}
	console.log(id, args, dump.trim());

	ws.send(buff);
}
