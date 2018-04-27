var wsUrl = document.getElementById("wsUrl");
var currentWsUrl = "";
var ws;
function sendEvent(id, args)
{
	if(wsUrl.value != currentWsUrl)
	{
		ws = new WebSocket(wsUrl.value);
		currentWsUrl = wsUrl.value;

		ws.onopen = function()
		{
			sendEvent(id, args);
		};
		return;
	}

	var littleEndian = true; //XXX: this is raspberry pi LE mode specific

	var buff = new ArrayBuffer(16 + args.length * 4);
	var data = new DataView(buff);

	var now = Date.now();
	if(littleEndian) //TODO this is broken, pls fix
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
		data.setInt32(16 + i * 4, args[i], littleEndian);
	}

	var dump = "";
	for(var i = 0; i < data.byteLength; i++)
	{
		dump += " " + ("00" + data.getUint8(i).toString(16)).slice(-2);
	}
	console.log(id, events[id], args, dump.trim());

	if(ws.readyState == ws.OPEN)
		ws.send(buff);
}

var eventId = document.getElementById("eventId");
var eventArgs = document.getElementById("eventArgs");
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

function sendEventFromForm()
{
	var id = eventId.selectedIndex;
	var args = eventArgs.value.split(" ");

	if(args.length == 1 && args[0].trim() == "")
		args = [];

	for(var i = 0; i < args.length; i++)
		args[i] = parseInt(args[i]);

	sendEvent(id, args);
}

var joystick = document.getElementById("joystick");
//this value defines how many pixels refer to 100% power of a motor
//bigger values => need to move the joystick further for max power
var jostickSensitivity = document.getElementById("joystickSensitivity");
var draw = joystick.getContext('2d');
var joystickActive = false;
var originX = 0;
var originY = 0;
var sensitivity = jostickSensitivity.value;
draw.fillStyle = "red";
draw.strokeStyle = "black";
draw.lineWidth = 5;

function clamp(val, min, max)
{
	return Math.max(min, Math.min(val, max));
}
function redraw(x, y)
{
	draw.clearRect(0, 0, joystick.width, joystick.height);

	if(!ws || ws.readyState != ws.OPEN)
		draw.strokeStyle = "red";
	else
		draw.strokeStyle = "black";
	draw.strokeRect(0, 0, joystick.width, joystick.height);
	draw.strokeStyle = "black";

	if(joystickActive)
	{
		draw.beginPath();
		draw.arc(originX, originY, 100, 0, Math.PI, true);
		draw.lineTo(originX + 100 + draw.lineWidth / 2, originY);
		draw.stroke();

		draw.beginPath();
		draw.arc(originX, originY, 10, 0, 2 * Math.PI);
		draw.fill();

		draw.beginPath();
		draw.arc(x, y, 10, 0, 2 * Math.PI);
		draw.fill();
	}
}
redraw();

joystick.addEventListener("mousedown", function(ev)
{
	joystickActive = true;
	sensitivity = jostickSensitivity.value;

	originX = ev.offsetX;
	originY = ev.offsetY;
	redraw(ev.offsetX, ev.offsetY);
});
joystick.addEventListener("mouseup", function()
{
	joystickActive = false;
	redraw();
});
joystick.addEventListener("mousemove", function(ev)
{
	if(joystickActive)
	{
		redraw(ev.offsetX, ev.offsetY);

		var speed = (originY - ev.offsetY) * 1000 / sensitivity;
		sendEvent(1, [clamp(speed, 0, 1000)]);

		var steering = (originX - ev.offsetX) * 1000 / sensitivity + 500;
		sendEvent(2, [clamp(steering, 0, 1000)]);
	}
});