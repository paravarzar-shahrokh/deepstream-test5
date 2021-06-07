import json
import pprint
import websocket
from websocket import create_connection

websocket.enableTrace(True)
def on_open(ws):
    result = ws.recv()
    print(result)


def on_close(ws):
    print("### closed ###")

if __name__ == "__main__":
    ws = websocket.WebSocketApp("ws://localhost:3007/'", on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()