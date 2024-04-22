# app.py
from flask import Flask, request, jsonify
import json
from flask import render_template
from flask_sqlalchemy import SQLAlchemy
from flask_apscheduler import APScheduler
import pandas as pd
import csv
from plyer import notification

class Config(object):
    SCHEDULER_API_ENABLED = True
app = Flask(__name__)
app.config.from_object(Config())

scheduler = APScheduler()
scheduler.init_app(app)
scheduler.start()

class Data:
    def __init__(self, Username, Time_stamp, Gate, Door, Window1, Alerts):
        self.Username = Username
        self.Time_stamp = Time_stamp
        self.Gate = Gate
        self.Door = Door
        self.Window1 = Window1
        self.Alerts = Alerts

    def to_dict(self):
        return {
            'Username': self.Username,
            'Time_stamp': self.Time_stamp,
            'Gate': self.Gate,
            'Door': self.Door,
            'Window1': self.Window1,
            'Alerts': self.Alerts
        }

    def save_to_csv(self, filename):
        with open(filename, 'a', newline='') as csvfile:
            fieldnames = ['Username', 'Time_stamp', 'Gate', 'Door', 'Window1', 'Alerts']
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

            # Write the header if the file is empty
            if csvfile.tell() == 0:
                writer.writeheader()

            writer.writerow(self.to_dict())

def send_desktop_notification(title, message):
    notification.notify(
        title=title,
        message=message,
        timeout=10  # Display the notification for 10 seconds
    )
# function to fetch last_row from the csv file
def fetch_latest_data():
    global currentData
    df = pd.read_csv('data.csv')
    currentData = df.iloc[-1].to_dict()  # Get the last row of the DataFrame
    return currentData
# load currentData when the app starts
currentData = fetch_latest_data()

# code to allow esp to get data
@app.route('/data', methods=['GET'])
def get_data():
    return jsonify(currentData), 200

# code to fetch last 10 rows from the csv file
@app.route('/history', methods=['GET'])
def get_history():
    df = pd.read_csv('data.csv')
    history = df.tail(10).to_dict(orient='records')
    return jsonify(history), 200

# code to update the csv file
@app.route('/update', methods=['POST'])
def update_data():
    data = request.get_json()
    new_data = Data(
        Username=data['Username'],
        Time_stamp=data['Time_stamp'],
        Gate=data['Gate'],
        Door=data['Door'],
        Window1=data['Window1'],
        Alerts=data['Alerts']
    )
    new_data.save_to_csv('data.csv')
    global currentData
    currentData = new_data.to_dict()

    # # If an alert is triggered, send a desktop notification
    # if data['Alerts']:
    #     send_desktop_notification("Alert", "An alert was triggered!")

    return jsonify(currentData), 200

@app.route('/')
def home():
    return render_template('dashboard.html', data=currentData)

if __name__ == '__main__':
    app.run(debug=True)