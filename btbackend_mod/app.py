from flask import Flask, jsonify
from flask_cors import CORS
from flask_socketio import SocketIO, emit

import cx_Oracle

app = Flask(__name__)
CORS(app)
socketio = SocketIO(app, cors_allowed_origins="*")

# Database connection configuration
dsn_tns = cx_Oracle.makedsn('localhost', 1521, 'xe')
con = cx_Oracle.connect('c##jump', 'embeddedc', dsn_tns)

# Helper function to execute queries and return JSON response
def execute_query(query):
    print("Executing query:", query)
    cur = con.cursor()
    cur.execute(query)
    data = cur.fetchall()
    cur.close()
    return data

@app.route("/")
def index():
    return "Server is running!"

@app.route("/data")
def get_data():
    query = "SELECT horizontal, vertical FROM your_table_name ORDER BY data_id DESC"
    print("Fetching data for route /data")
    return jsonify(execute_query(query))

# Define WebSocket event handlers for additional routes
@socketio.on('get_data')
def handle_get_data():
    query = "SELECT horizontal, vertical FROM your_table_name ORDER BY data_id DESC"
    print("Received 'get_data' event from client")
    data = execute_query(query)
    print("Emitting 'data_response' event with data:", data)
    emit('data_response', data)


if __name__ == "__main__":
    socketio.run(app)
