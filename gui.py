import sys
from PyQt5 import QtCore
from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import QApplication, QLineEdit, QSlider, QComboBox, QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton
from PyQt5.QtGui import QIcon, QPixmap
import paho.mqtt.client as mqtt
import json
import numpy as np

class App(QWidget):

	# Controller data
	cont_tick = 0
	cont_moving = False
	cont_angle_cngr = 0

	# Sensor data
	sensor_tick = 0
	sensor_adxl_x = 0
	sensor_adxl_y = 0
	sensor_adxl_temp = 0
	sensor_tilt_angle = 0

	def mqtt_on_connect(self, client, userdata, flags, rc):
		client.subscribe("angle_rig/stream/controller")
		client.subscribe("angle_rig/stream/sensor")

	def mqtt_on_message(self, client, userdata, msg):
		try:
			self.json_packet = json.loads(msg.payload)
		except:
			pass
		else:
			if msg.topic in "angle_rig/stream/sensor":
				try:
					self.sensor_adxl_x = np.rad2deg(np.arctan(self.json_packet["g_x"] / self.json_packet["g_z"]))
					self.sensor_adxl_y = np.rad2deg(np.arctan(self.json_packet["g_y"] / self.json_packet["g_z"]))
				except:
					pass
				self.sensor_tilt_angle = self.json_packet["angle"]
			if msg.topic in "angle_rig/stream/controller":
				self.cont_tick = self.json_packet["tick"]
				self.cont_moving = self.json_packet["moving"]
				self.cont_angle_cngr = self.json_packet["rotation_count"]

	def timer_update_data(self):
		try:
			self.l_cont_tick.setText(str(round(self.cont_tick / 1e6, 3)))
			self.l_moving.setText(str(self.cont_moving))
			self.l_angle_cntr.setText(str(self.cont_angle_cngr))
			#self.l_sen_tick.setText(str(round(self.json_packet["dg"]["tick"] / 1e3, 3)))
			self.l_x_angle.setText(str(round(self.sensor_adxl_x, 3)))
			self.l_y_angle.setText(str(round(self.sensor_adxl_y, 3)))
			#self.l_z_angle.setText(str(round(angle_z, 3)))
			#self.l_temp.setText(str(round(self.json_packet["dg"]["temp"], 3)))
			self.l_pres_angle.setText(str(round(self.sensor_tilt_angle, 3)))
		except:
			pass

	def __init__(self):
		super().__init__()

		# MQTT
		self.client = mqtt.Client()
		self.client.on_connect = self.mqtt_on_connect
		self.client.on_message = self.mqtt_on_message
		#self.client.username_pw_set("gui", password="angle_rig")
		self.client.connect("192.168.1.11", 1883, 60)
		self.client.loop_start()

		# Window init
		self.setWindowTitle('Zone OS Angle Rig Controller')
		self.setGeometry(10, 10, 250, 300)
		self.setFixedSize(300, 700)

		# GUI init

		# Rig status
		self.l_cont_tick = QLabel(" ")
		self.l_moving = QLabel(" ")
		self.l_angle_cntr = QLabel(" ")
		self.l_sen_tick = QLabel(" ")
		self.l_x_angle = QLabel(" ")
		self.l_y_angle = QLabel(" ")
		self.l_z_angle = QLabel(" ")
		self.l_temp = QLabel(" ")
		self.l_pres_angle = QLabel(" ")
		
		# Buttons
		self.b_move_abs = QPushButton('Move Absolute')
		self.b_move_abs.clicked.connect(self.b_move_abs_clicked)
		self.b_move_rel = QPushButton('Move Relative')
		self.b_move_rel.clicked.connect(self.b_move_rel_clicked)
		self.b_move_hme = QPushButton('Move Home')
		self.b_move_hme.clicked.connect(self.b_move_hme_clicked)
		self.b_zero = QPushButton('Set Home')
		self.b_zero.clicked.connect(self.b_zero_clicked)
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
		vbox.addWidget(QLabel('Rotation Counter (n)'))
		vbox.addWidget(self.l_angle_cntr)
		vbox.addWidget(QLabel('Sensor Tick (s)'))
		vbox.addWidget(self.l_sen_tick)
		vbox.addWidget(QLabel('ADXL Angle X (°)'))
		vbox.addWidget(self.l_x_angle)
		vbox.addWidget(QLabel('ADXL Angle Y (°)'))
		vbox.addWidget(self.l_y_angle)
		vbox.addWidget(QLabel('ADXL Temperature (°C)'))
		vbox.addWidget(self.l_temp)
		vbox.addWidget(QLabel('Electrolytic Angle (°)'))
		vbox.addWidget(self.l_pres_angle)
		vbox.addWidget(QLabel('Angle to move (°)'))
		vbox.addWidget(self.t_move_angle)
		vbox.addWidget(self.b_move_abs)
		vbox.addWidget(self.b_move_rel)
		vbox.addWidget(self.b_move_hme)
		vbox.addWidget(self.b_zero)
		vbox.addWidget(self.l_move_feedback)

		self.setLayout(vbox)

		# Timer for data update
		self.timer = QTimer(self)
		self.timer.timeout.connect(self.timer_update_data)
		self.timer.start(100)

		# Show
		self.show()

	def b_move_abs_clicked(self):
		self.b_move_clicked("ABS")

	def b_move_rel_clicked(self):
		self.b_move_clicked("RELA")
		
	def b_move_hme_clicked(self):
		self.b_move_clicked("HOME")
		
	def b_move_clicked(self, button):

		if self.cont_moving:
			self.l_move_feedback.setText("Rig is already moving.")
			return

		angle = 0

		if button != "HOME":
			try:
				angle = float(self.t_move_angle.text())
			except:
				self.l_move_feedback.setText(f'Input angle is not valid.')
			else:
				self.l_move_feedback.setText(f'Going to move {angle}°.')

		data_set = {"cmd": "MOVE", "params": [button, angle]}
		self.client.publish("angle_rig/command", json.dumps(data_set))

		try:
			self.move_timer.stop()
		except:
			pass

		self.move_timer = QTimer(self)
		self.move_timer.timeout.connect(self.l_move_timer)
		self.move_timer.start(3000)

	def b_zero_clicked(self):

		data_set = {"cmd": "HOME", "params": []}
		self.client.publish("angle_rig/command", json.dumps(data_set))		

	def l_move_timer(self):
		self.l_move_feedback.setText('')
		self.move_timer.stop()

if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = App()
	sys.exit(app.exec_())