# app.py
from flask import Flask, request, jsonify,session, session
import json
from flask import render_template
from flask_sqlalchemy import SQLAlchemy
from flask_apscheduler import APScheduler
import pandas as pd
import csv
from plyer import notification
import os

class Config(object):
    SCHEDULER_API_ENABLED = True
app = Flask(__name__)
app.config.from_object(Config())
app.secret_key = 'your_secret_key'
USERS_CSV = 'users.csv'
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
# PATH NAME CHANGED FROM data to currentData
@app.route('/currentData', methods=['GET'])
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

def check_user_in_csv(username, password):
    with open(USERS_CSV, mode='r') as file:
        csv_reader = csv.DictReader(file)
        for row in csv_reader:
            if row['Username'] == username and row['Password'] == password and row['Status']=='1':
                return True
    return False

def store_user_in_csv(username, password):
    with open(USERS_CSV, mode='a', newline='') as file:
        fieldnames = ['Username', 'Password','Status']
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writerow({'Username': username, 'Password': password,'Status':'0'})

USERS_CSV = 'users.csv'

def check_user_in_csv(username, password):
    with open(USERS_CSV, mode='r') as file:
        csv_reader = csv.DictReader(file)
        for row in csv_reader:
            if row['Username'] == username and row['Password'] == password and row['Status']=='1':
                return True
    return False

def store_user_in_csv(username, password):
    with open(USERS_CSV, mode='a', newline='') as file:
        fieldnames = ['Username', 'Password','Status']
        writer = csv.DictWriter(file, fieldnames=fieldnames)
        writer.writerow({'Username': username, 'Password': password,'Status':'0'})

@app.route('/')
def home():
    if 'username' in session:
        if session['username'] == "Admin":
            return render_template('home_admin.html')
        else:
            return render_template('home.html')
    else:
        return render_template('index.html')

@app.route('/login', methods=['POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if username == "Admin" and password == "Admin":
            session['username'] = username
            return render_template('login_admin.html')
        if check_user_in_csv(username, password):
            session['username'] = username
            return render_template('login_successful.html')
        else:
            return render_template('Invalid_User.html')
    return render_template('login.html')

@app.route('/register', methods=['POST'])
def register():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if check_user_in_csv(username, password):
            return render_template('redirect_signup.html')
        store_user_in_csv(username, password)
        session['username'] = username
        return render_template('Invalid_User.html')
    return render_template('register.html')


@app.route('/update-csv', methods=['POST'])
def update_csv():
    index = int(request.json['index'])
    updated_row_data = request.json['rowData']
    with open('users.csv', 'r') as file:
        lines = file.readlines()
    lines[index] = updated_row_data + '\n'
    with open('users.csv', 'w') as file:
        file.writelines(lines)
    return 'CSV row updated successfully', 200

@app.route('/get-csv-users')
def get_csv_users():
    with open('users.csv', 'r') as file:
        csv_data = file.read()
    return csv_data

@app.route('/get-csv-data')
def get_csv_data():
    with open('data.csv', 'r') as file:
        csv_data = file.read()
    return csv_data

@app.route('/log')
def log():
    return render_template('login.html')
@app.route('/sign')
def sign():
    return render_template('register.html')

@app.route('/home3')
def home3():
    return render_template('home.html')

@app.route('/home_admin2')
def home_admin2():
    return render_template('home_admin.html')

@app.route('/logout')
def logout():
    session.pop('username', None)
    return render_template('index.html')

@app.route('/Dashboard')
def Dashboard():
    return render_template('dashboard.html')

@app.route('/data')
def data():
    return render_template('data.html')

@app.route('/Users')
def Users():
    return render_template('Users.html')
    if 'username' in session:
        if session['username'] == "Admin":
            return render_template('home_admin.html')
        else:
            return render_template('home.html')
    else:
        return render_template('index.html')

@app.route('/login', methods=['POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if username == "Admin" and password == "Admin":
            session['username'] = username
            return render_template('login_admin.html')
        if check_user_in_csv(username, password):
            session['username'] = username
            return render_template('login_successful.html')
        else:
            return render_template('Invalid_User.html')
    return render_template('login.html')

@app.route('/register', methods=['POST'])
def register():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if check_user_in_csv(username, password):
            return render_template('redirect_signup.html')
        store_user_in_csv(username, password)
        session['username'] = username
        return render_template('Invalid_User.html')
    return render_template('register.html')


@app.route('/update-csv', methods=['POST'])
def update_csv():
    index = int(request.json['index'])
    updated_row_data = request.json['rowData']
    with open('users.csv', 'r') as file:
        lines = file.readlines()
    lines[index] = updated_row_data + '\n'
    with open('users.csv', 'w') as file:
        file.writelines(lines)
    return 'CSV row updated successfully', 200

@app.route('/get-csv-users')
def get_csv_users():
    with open('users.csv', 'r') as file:
        csv_data = file.read()
    return csv_data

@app.route('/get-csv-data')
def get_csv_data():
    with open('data.csv', 'r') as file:
        csv_data = file.read()
    return csv_data

@app.route('/log')
def log():
    return render_template('login.html')
@app.route('/sign')
def sign():
    return render_template('register.html')

@app.route('/home3')
def home3():
    return render_template('home.html')

@app.route('/home_admin2')
def home_admin2():
    return render_template('home_admin.html')

@app.route('/logout')
def logout():
    session.pop('username', None)
    return render_template('index.html')

@app.route('/Dashboard')
def Dashboard():
    return render_template('dashboard.html')

@app.route('/data')
def data():
    return render_template('data.html')

@app.route('/Users')
def Users():
    return render_template('Users.html')


if __name__ == '__main__':
    if not os.path.exists(USERS_CSV):
        with open(USERS_CSV, mode='w', newline='') as file:
            fieldnames = ['Username', 'Password','Status']
            writer = csv.DictWriter(file, fieldnames=fieldnames)
            writer.writeheader()
    app.run(debug=True)