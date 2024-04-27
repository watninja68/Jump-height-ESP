import serial
import cx_Oracle

dsn_tns = cx_Oracle.makedsn('localhost', 1521, 'xe')
con = cx_Oracle.connect('c##jump', 'embeddedc', dsn_tns)

def execute_query(query):
    print("Executing query:", query)
    cur = con.cursor()
    cur.execute(query)
    con.commit()
    cur.close()

port = "COM10"
bluetooth = serial.Serial(port, 115200, timeout=1)
bluetooth.flushInput()
print(2)

    
while 1:
    # Read data from Bluetooth
    input_data = bluetooth.readline().decode().strip()
    if input_data:
        horizontal = input_data[:4]
        vertical = input_data[4:]       
        horizontal=float(horizontal)
        vertical=float(vertical)  
        print(horizontal) 
        query = f"INSERT INTO your_table_name (DATA_ID,HORIZONTAL, VERTICAL) VALUES (your_sequence.nextval,{horizontal}, {vertical})"
        execute_query(query)

    
       

