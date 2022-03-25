import sys
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from scanner import Scanner
from worker import Worker
import numpy as np

class App(QWidget):

	def __init__(self):
		super().__init__()

		a = np.zeros(512*512)
		a = np.reshape(a,(512,512))
		self.image = np.require(a, np.uint8, 'C')

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
		self.s_gain = QSlider(Qt.Horizontal)
		self.s_gain.setRange(100, 500)
		self.s_gain.setSingleStep(1)
		self.s_gain.valueChanged.connect(self.s_gain_changed)
		self.l_gain = QLabel('Gain')
		self.s_gain_changed()

		# Setup the offset slider
		self.s_offset = QSlider(Qt.Horizontal)
		self.s_offset.setRange(0, 100)
		self.s_offset.setSingleStep(1)
		self.s_offset.setValue(59)
		self.s_offset.valueChanged.connect(self.s_offset_changed)
		self.l_offset = QLabel('Offset (black level)')
		self.s_offset_changed()

		# Setup the brightness slider
		self.s_brightness = QSlider(Qt.Horizontal)
		self.s_brightness.setRange(0, 100)
		self.s_brightness.setSingleStep(1)
		self.s_brightness.setValue(100)
		self.s_brightness.valueChanged.connect(self.s_brightness_changed)		
		self.l_brightness = QLabel('LED Brightness')
		self.s_brightness_changed()

		# Mean line value
		self.l_line_value = QLabel('Mean Line Value')

		# Layout
		vbox = QVBoxLayout()
		vbox.setAlignment(Qt.AlignTop)
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
		vbox.addWidget(self.l_line_value)

		hbox = QHBoxLayout()
		hbox.addWidget(self.l_image_display, 4)
		hbox.addLayout(vbox, 1)

		self.setLayout(hbox)

	
		# Show
		self.show()

	def b_refresh_clicked(self):
		scnr = Scanner()
		devices = scnr.list_devices()
		self.c_devices.clear()
		for device in devices:
			self.c_devices.addItem(device)

	def print_output(self, s):
		print(s)
	
	def thread_complete(self):
		print("Thread quit")

	def b_start_stop_clicked(self):
		if self.scanning:
			print('Stopping scanning...')

			self.worker.stop()

			# Do some things here.
			self.scanning = False
			self.b_start_stop.setText('Start')

		else:
			print('Starting scanning...')
			
			# Do some things here.
			self.scanning = True
			self.b_start_stop.setText('Stop')

			self.threadpool = QThreadPool()
			device = str(self.c_devices.currentText())
			gain = float(self.s_gain.value()) / 100.0
			offset = float(self.s_offset.value()) / 100.0
			brightness = float(self.s_brightness.value()) / 100.0
			self.worker = Worker(device, gain, offset, brightness)
			self.worker.signals.result.connect(self.print_output)
			self.worker.signals.finished.connect(self.thread_complete)
			self.worker.signals.line.connect(self.handle_line)
			self.threadpool.start(self.worker)

	def s_gain_changed(self):
		self.l_gain.setText(f'Gain = {self.s_gain.value() / 100.0}')
		try:
			self.worker.set_gain(float(self.s_gain.value()) / 100.0)
		except:
			pass

	def s_offset_changed(self):
		self.l_offset.setText(f'Offset (black level) = {self.s_offset.value()} %')
		try:
			self.worker.set_offset(float(self.s_offset.value()) / 100.0)
		except:
			pass

	def s_brightness_changed(self):
		self.l_brightness.setText(f'LED Brightness = {self.s_brightness.value()} %')
		try:
			self.worker.set_brightness(float(self.s_brightness.value()) / 100.0)
		except:
			pass

	def handle_line(self, line):
		if type(line) == np.ndarray:
			if len(line) == 512:
				self.image = np.roll(self.image, shift=1, axis=0)
				self.image[0] = line
				img = QImage(self.image.data, 512, 512, QImage.Format_Indexed8)
				self.l_image_display.setPixmap(QPixmap.fromImage(img))
				self.l_line_value.setText(f'Mean Line Value = {np.mean(line)}')

		

if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = App()
	sys.exit(app.exec_())