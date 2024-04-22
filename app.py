# app.py
from flask import Flask, request, jsonify
import json
from flask import render_template
from flask_sqlalchemy import SQLAlchemy
from flask_apscheduler import APScheduler

class Config(object):
    SCHEDULER_API_ENABLED = True
app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'cockroachdb://mehul:5Np79ayHOZEuvdOfektdyQ@vision-show-8951.8nk.gcp-asia-southeast1.cockroachlabs.cloud:26257/iot?sslmode=require'
# app.config['SQLALCHEMY_DATABASE_URI'] = 'postgresql://mehul:5Np79ayHOZEuvdOfektdyQ@vision-show-8951.8nk.gcp-asia-southeast1.cockroachlabs.cloud:26257/iot?sslmode=require'
# app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)
app.config.from_object(Config())

scheduler = APScheduler()
scheduler.init_app(app)
scheduler.start()

class Data(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    Username = db.Column(db.String(255), nullable=False)
    Time_stamp = db.Column(db.Integer, nullable=False)
    Gate = db.Column(db.Integer, nullable=False)
    Door = db.Column(db.Integer, nullable=False)
    Doorlock = db.Column(db.Integer, nullable=False)
    Window1 = db.Column(db.Integer, nullable=False)
    def to_dict(self):
        return {
            'id': self.id,
            'Username': self.Username,
            'Time_stamp': self.Time_stamp,
            'Gate': self.Gate,
            'Door': self.Door,
            'Doorlock': self.Doorlock,
            'Window1': self.Window1
        }

with app.app_context():
    db.create_all()


# load data to currentData periodically
currentData = None
# def fetch_data():
#     global currentData
#     currentData = Data.query.order_by(Data.Time_stamp.desc()).first()
#     return currentData

@scheduler.task('interval', id='do_fetch_data', seconds=0.5, misfire_grace_time=900)
def fetch_data():
    with app.app_context():
        global currentData
        currentData = Data.query.order_by(Data.Time_stamp.desc()).first()
        return currentData
@app.route('/')
def home():
    with app.app_context():
        data = fetch_data()
    if data is None:
        data = Data(Username='default', Time_stamp=0, Gate=0, Door=0, Doorlock=0, Window1=0)
        db.session.add(data)
        db.session.commit()
    return render_template('dashboard.html', data = currentData.to_dict())


# Update the database with the new data
@app.route('/updateDatabase', methods=['POST'])
def update_data():
    data = request.get_json()
    db_data = Data.query.first()
    if 'Username' in data:
        db_data.Username = data['Username']
    if 'Time_stamp' in data:
        db_data.Time_stamp = data['Time_stamp']
    if 'Gate' in data:
        db_data.Gate = data['Gate']
    if 'Door' in data:
        db_data.Door = data['Door']
    if 'Doorlock' in data:
        db_data.Doorlock = data['Doorlock']
    if 'Window1' in data:
        db_data.Window1 = data['Window1']
    db.session.commit()
    currentData = db_data
    print("Changed data\n", db_data)
    return jsonify({'Username': db_data.Username, 'Time_stamp': db_data.Time_stamp, 'Gate': db_data.Gate, 'Door': db_data.Door, 'Doorlock': db_data.Doorlock, 'Window1': db_data.Window1})



# Fetch data from database and return it
@app.route('/fetch', methods=['GET'])
def sendCurrentData():
    with app.app_context():
        data = fetch_data()
    return jsonify({'Username': data.Username, 'Time_stamp': data.Time_stamp, 'Gate': data.Gate, 'Door': data.Door, 'Doorlock': data.Doorlock, 'Window1': data.Window1})

if __name__ == '__main__':
    app.run(debug=True)