import websocket
import thread
import time
import json

def on_message(ws, message):
    msg_json= json.loads(message)
    if "lat" in msg_json:
        lat = msg_json["lat"]
        alt = msg_json["alt"]
        lng = msg_json["lng"]
        print("the altitude {}".format(lat))
        print("the altitude {}".format(alt))
        print("the altitude {}".format(lng))
        print("---------------------------------------")
    else:
        pass
def on_error(ws, error):
    print error

def on_close(ws):
    print ("### closed ###")

if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://localhost:3007/",
                                on_message = on_message,
                                on_error = on_error,
                                on_close = on_close)
    ws.run_forever()
    