# db.py - Manages exchange between microcontroller and database.
#         Also sends the activation signal to begin the readout.

import serial
import time
import gspread
import pandas as pd
from oauth2client.service_account import ServiceAccountCredentials

#define the scope
scope = ['https://spreadsheets.google.com/feeds','https://www.googleapis.com/auth/drive']

# add credentials to the account
creds = ServiceAccountCredentials.from_json_keyfile_name('C:/Users/Lein/Desktop/temperature-scanner-342921-17ef87090809.json', scope)

# authorize the clientsheet 
client = gspread.authorize(creds)

# get the instance of the Spreadsheet
sheet = client.open('Temperature Database')

# get the primary subsheet of the Spreadsheet
sheet_instance = sheet.get_worksheet(0)

# set up serial on COM4 (Cameron's laptop) and 9600 baud
ser = serial.Serial('COM4', 9600, timeout = 1)
time.sleep(2)

# init row update vars
rows = sheet_instance.row_count
rowCnt = rows

while True:
    # update worksheet to compare current row
    sheet_instance = sheet.get_worksheet(0)
    rows = sheet_instance.row_count
    print("Current ", rows)
    print("Old ",rowCnt)
    time.sleep(1)

    if (rows != rowCnt):
        # get user-preferred unit (deg F or C)
        unitRead = sheet_instance.cell(col=2,row=rows).value
        ser.write(unitRead.encode())

        # wait for temp scanner to finish
        time.sleep(8)
        
        # read, decode, and export temp to database
        temp = ser.read(4)
        output = temp.decode("utf-8")
        sheet_instance.update_cell(rows, 3, output)

        # increment row count
        rowCnt = rows