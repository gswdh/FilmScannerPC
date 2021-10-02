import sys
from PyQt5 import QtCore
from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import QApplication, QLineEdit, QSlider, QComboBox, QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton
from PyQt5.QtGui import QIcon, QPixmap
import paho.mqtt.client as mqtt
import json

class App(QWidget):

	json_packet = ""
	rig_moving = False

	def mqtt_on_connect(self, client, userdata, flags, rc):
	    client.subscribe("angle_rig/stream")

	def mqtt_on_message(self, client, userdata, msg):
	    try:
	    	self.json_packet = json.loads(msg.payload)
	    except:
	    	pass

	def timer_update_data(self):
		try:
			self.l_cont_tick.setText(str(round(self.json_packet["tick"] / 1e6, 3)))
			self.l_moving.setText(str(self.json_packet["moving"]))
			self.l_sen_tick.setText(str(round(self.json_packet["dg"]["tick"] / 1e3, 3)))
			self.l_x_angle.setText(str(round(self.json_packet["dg"]["g_x"], 3)))
			self.l_y_angle.setText(str(round(self.json_packet["dg"]["g_y"], 3)))
			self.l_z_angle.setText(str(round(self.json_packet["dg"]["g_y"], 3)))
			self.l_temp.setText(str(round(self.json_packet["dg"]["temp"], 3)))
			self.l_pres_angle.setText(str(round(self.json_packet["dg"]["angle"], 3)))

			if "false" in str(self.json_packet["moving"]):
				rig_moving = False
			else:
				rig_moving = True
		except:
			pass

	def __init__(self):
		super().__init__()

		# MQTT
		self.client = mqtt.Client()
		self.client.on_connect = self.mqtt_on_connect
		self.client.on_message = self.mqtt_on_message
		self.client.username_pw_set("jtm", password="rbjtm0805")
		self.client.connect("mb.jackthemaker.co", 1883, 60)
		self.client.loop_start()

		# Window init
		self.setWindowTitle('Zone OS Angle Rig Controller')
		self.setGeometry(10, 10, 400, 600)

		# GUI init

		# Rig status
		self.l_cont_tick = QLabel(" ")
		self.l_moving = QLabel(" ")
		self.l_sen_tick = QLabel(" ")
		self.l_x_angle = QLabel(" ")
		self.l_y_angle = QLabel(" ")
		self.l_z_angle = QLabel(" ")
		self.l_temp = QLabel(" ")
		self.l_pres_angle = QLabel(" ")
		
		# Move button
		self.b_move = QPushButton('Move')
		self.b_move.clicked.connect(self.b_move_clicked)
		self.l_move_feedback = QLabel(" ")
		
		# Scan parameters
		self.t_move_angle = QLineEdit(self)

		# Layout
		vbox = QVBoxLayout()
		vbox.setAlignment(QtCore.Qt.AlignTop)
		vbox.addWidget(QLabel('Angle Rig Control'))
		vbox.addWidget(QLabel('Controller tick (s)'))
		vbox.addWidget(self.l_cont_tick)
		vbox.addWidget(QLabel('Rig moving (bool)'))
		vbox.addWidget(self.l_moving)
		vbox.addWidget(QLabel('Sensor Tick (s)'))
		vbox.addWidget(self.l_sen_tick)
		vbox.addWidget(QLabel('ADXL Angle X (°)'))
		vbox.addWidget(self.l_x_angle)
		vbox.addWidget(QLabel('ADXL Angle Y (°)'))
		vbox.addWidget(self.l_y_angle)
		vbox.addWidget(QLabel('ADXL Angle Z (°)'))
		vbox.addWidget(self.l_z_angle)
		vbox.addWidget(QLabel('ADXL Temperature (°)'))
		vbox.addWidget(self.l_temp)
		vbox.addWidget(QLabel('Electrolytic Angle (°)'))
		vbox.addWidget(self.l_pres_angle)
		vbox.addWidget(QLabel('Angle to move (°)'))
		vbox.addWidget(self.t_move_angle)
		vbox.addWidget(self.b_move)
		vbox.addWidget(self.l_move_feedback)

		self.setLayout(vbox)

		# Timer for data update
		self.timer = QTimer(self)
		self.timer.timeout.connect(self.timer_update_data)
		self.timer.start(20)

		# Show
		self.show()

	def b_move_clicked(self):

		if self.rig_moving:
			self.l_move_feedback.setText("Rig is already moving.")
			return
		try:
			angle = float(self.t_move_angle.text())
		except:
			self.l_move_feedback.setText(f'Input angle is not valid.')
		else:
			self.l_move_feedback.setText(f'Going to move {angle}°.')

		self.move_timer = QTimer(self)
		self.move_timer.timeout.connect(self.l_move_timer)
		self.move_timer.start(3000)

	def l_move_timer(self):
		self.l_move_feedback.setText('')
		self.move_timer.stop()

if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = App()
	sys.exit(app.exec_())