from wifi import Cell, Scheme
WlanCell = []

def getAllWifi():
    global WlanCell
    WlanCell = Cell.all('wlan0')
    return WlanCell

def connectToWifi(ssid,passd):
    try:
        scheme = Scheme.for_cell('wlan0', ssid, ssid,passd)
        scheme.save()
        scheme.activate()
        print("Connected To Wifi " + ssid)
        return True
    except Exception:
        print("Error by Connecting with " + ssid)
        return False