from wifi import Cell, Scheme
WlanCell = []

def getAllWifi():
	global WlanCell
	WlanCell = Cell.all('wlan0')
	return  WlanCell