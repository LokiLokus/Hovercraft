from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
from cgi import parse_header, parse_multipart
from urlparse import parse_qs
import os.path,string,WiFi


ADDR = "0.0.0.0"
PORT = 800
WEBCONT = "WebContent/"
httpd = None


def getWifiOption():
    wifiList = WiFi.getAllWifi()
    result = ""
    for i in range(0, len(wifiList)):
        result += '<option value="' + wifiList[i].ssid + '">' + wifiList[i].ssid + '</option>'
    return result

class RequestHandler(BaseHTTPRequestHandler):
    def _set_headers(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_HEAD(self):
        self._set_headers()

    def parse_POST(self):
        ctype, pdict = parse_header(self.headers['content-type'])
        if ctype == 'multipart/form-data':
            postvars = parse_multipart(self.rfile, pdict)
        elif ctype == 'application/x-www-form-urlencoded':
            length = int(self.headers['content-length'])
            postvars = parse_qs(self.rfile.read(length),keep_blank_values=1)
        else:
            postvars = {}
        return postvars

    def do_GET(self,req=None,ssid=None):

        if req is None:
            self._set_headers()
            reqFile = WEBCONT + self.path[1:]
            if reqFile == '' or not os.path.isfile(reqFile):
                reqFile = WEBCONT + 'Index.html'
            f = open(reqFile, "r")
            self.wfile.write(string.replace(f.read(),'@WLAN_DATA',getWifiOption()))
        else:
            f = open(WEBCONT + req, "r")
            self.wfile.write(string.replace(f.read(), '@WifiSSID', ssid))

    def do_POST(self):
        self._set_headers()
        postvars = self.parse_POST()
        if WiFi.connectToWifi(postvars['selectedWiFi'][0],postvars['wiFiPassword'][0]):
            self.do_GET(req="Connected.html",ssid=postvars['selectedWiFi'][0])
        else:
            self.do_GET(req="Error.html",ssid=postvars['selectedWiFi'][0])

def start_WebServer(port="800"):
    global httpd
    httpd = HTTPServer((ADDR, PORT), RequestHandler)
    print("Webserver started on Port " + port)
    httpd.serve_forever()

def stop_WebServer():
    global  httpd
    httpd.shutdown()
    print("WebServer closed")