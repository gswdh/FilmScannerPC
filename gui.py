import sys
from PyQt5 import QtCore
from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import QApplication, QLineEdit, QSlider, QComboBox, QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton
from PyQt5.QtGui import QIcon, QPixmap
import paho.mqtt.client as mqtt
import json

class App(QWidget):

	json_packet = ""

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
		except:
			pass

	def __init__(self):
		super().__init__()

		# MQTT
		self.client = mqtt.Client()
		self.client.on_connect = self.mqtt_on_connect
		self.client.on_message = self.mqtt_on_message
		self.client.username_pw_set("jtm", password="")
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
		
		# Start stop button
		self.b_move = QPushButton('Move')
		self.b_move.clicked.connect(self.b_start_stop_clicked)
		self.moving = False

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


		self.setLayout(vbox)


		# Timer for data update
		self.timer = QTimer(self)
		self.timer.timeout.connect(self.timer_update_data)
		self.timer.start(100)


		# Show
		self.show()

	def b_refresh_clicked(self):
		return

	def b_start_stop_clicked(self):
		return

	def s_gain_changed(self):
		#self.l_gain.setText(f'Gain = {self.s_gain.value()}')
		return
	
	def s_offset_changed(self):
		#self.l_offset.setText(f'Offset (black level) = {self.s_offset.value()} %')
		return
	
	def s_brightness_changed(self):
		#self.l_brightness.setText(f'LED Brightness = {self.s_brightness.value()} %')
		return
	


		

if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = App()
	sys.exit(app.exec_())