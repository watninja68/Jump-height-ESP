from flask import Flask, render_template
import serial
import time

app = Flask(__name__)

port = "COM7"
bluetooth = serial.Serial(port, 115200, timeout=1)
bluetooth.flushInput()

@app.route("/")
def home():
    bluetooth_data = []
    for i in range(5):
        bluetooth.write(b"BOOP " + str.encode(str(i)))
        input_data = bluetooth.readline().decode().strip()
        print(input_data)
        bluetooth_data.append(input_data)
        bluetooth.write(b"BOOP " + str.encode(str(i)))
        input_data = bluetooth.readline().decode().strip()
        print(input_data)
        bluetooth_data.append(input_data)
        time.sleep(0.1)

    return render_template("index.html", bluetooth_data=bluetooth_data)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000, debug=True, use_reloader=False)