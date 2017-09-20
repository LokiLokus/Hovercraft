from wifi import Cell, Scheme
WlanCell = []

def getAllWifi():
    global WlanCell
    try:
        WlanCell = Cell.all('wlan0')
        return WlanCell
    except Exception:
        return None

def connectToWifi(ssid,passd):
    try:
        for elm in WlanCell:
            if elm.ssid == ssid:
                tmp = Scheme.find('wlan0',ssid)
                if tmp is not None:
                    tmp.delete()
                scheme = Scheme.for_cell('wlan0', ssid, elm,passd)
                scheme.save()
                scheme.activate()
            print("Connected To Wifi " + ssid)
            return True
        return False
    except Exception:
        print("Error by Connecting with " + ssid)
        return False