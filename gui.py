import sys
from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication, QLineEdit, QSlider, QComboBox, QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton
from PyQt5.QtGui import QIcon, QPixmap
import scanner

class App(QWidget):

	def __init__(self):
		super().__init__()

		# Create ourselves a scanner object
		self.scnr = scanner.Scanner()
		
		# Window init
		self.setWindowTitle('GSWDH Film Scanner Controller')
		self.setGeometry(10, 10, 1200, 600)

		# GUI init

		# A place to put the scanned image 
		self.l_image_display = QLabel(self)
		self.l_image_display.setStyleSheet("background-color: lightgreen")

		# Refresh button
		self.b_refresh = QPushButton('Refresh')
		self.b_refresh.clicked.connect(self.b_refresh_clicked)

		# Start stop button
		self.b_start_stop = QPushButton('Start')
		self.b_start_stop.clicked.connect(self.b_start_stop_clicked)
		self.scanning = False

		# Scan parameters
		self.c_devices = QComboBox(self)
		self.b_refresh_clicked()
		self.t_scan_length = QLineEdit(self)
		
		# Setup the gain slider
		self.s_gain = QSlider(QtCore.Qt.Horizontal)
		self.s_gain.setRange(1, 10)
		self.s_gain.setSingleStep(1)
		self.s_gain.valueChanged.connect(self.s_gain_changed)
		self.l_gain = QLabel('Gain')
		self.s_gain_changed()

		# Setup the offset slider
		self.s_offset = QSlider(QtCore.Qt.Horizontal)
		self.s_offset.setRange(0, 100)
		self.s_offset.setSingleStep(1)
		self.s_offset.valueChanged.connect(self.s_offset_changed)
		self.l_offset = QLabel('Offset (black level)')
		self.s_offset_changed()

		# Setup the brightness slider
		self.s_brightness = QSlider(QtCore.Qt.Horizontal)
		self.s_brightness.setRange(0, 100)
		self.s_brightness.setSingleStep(1)
		self.s_brightness.setValue(100)
		self.s_brightness.valueChanged.connect(self.s_brightness_changed)		
		self.l_brightness = QLabel('LED Brightness')
		self.s_brightness_changed()

		# Layout
		vbox = QVBoxLayout()
		vbox.setAlignment(QtCore.Qt.AlignTop)
		vbox.addWidget(QLabel('Scanner'))
		vbox.addWidget(self.b_refresh)
		vbox.addWidget(self.c_devices)
		vbox.addWidget(QLabel('Scan Length (frames)'))
		vbox.addWidget(self.t_scan_length)
		vbox.addWidget(self.b_start_stop)
		vbox.addWidget(self.l_gain)
		vbox.addWidget(self.s_gain)
		vbox.addWidget(self.l_offset)
		vbox.addWidget(self.s_offset)
		vbox.addWidget(self.l_brightness)
		vbox.addWidget(self.s_brightness)

		hbox = QHBoxLayout()
		hbox.addWidget(self.l_image_display, 4)
		hbox.addLayout(vbox, 1)

		self.setLayout(hbox)

	
		# Show
		self.show()

	def b_refresh_clicked(self):
		devices = self.scnr.list_devices()
		self.c_devices.clear()
		for device in devices:
			self.c_devices.addItem(device)

	def b_start_stop_clicked(self):
		if self.scanning:
			print('Stopping scanning...')

			# Do some things here.
			self.scanning = False
			self.b_start_stop.setText('Start')
			self.scnr.stop()

		else:
			print('Starting scanning...')
			
			# Do some things here.
			self.scanning = True
			self.b_start_stop.setText('Stop')
			self.scnr.start(str(self.c_devices.currentText()))

	def s_gain_changed(self):
		self.l_gain.setText(f'Gain = {self.s_gain.value()}')
	
	def s_offset_changed(self):
		self.l_offset.setText(f'Offset (black level) = {self.s_offset.value()} %')
	
	def s_brightness_changed(self):
		self.l_brightness.setText(f'LED Brightness = {self.s_brightness.value()} %')
	


		

if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = App()
	sys.exit(app.exec_())