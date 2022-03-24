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

		self.COLORTABLE=[]
		for i in range(256): 
			self.COLORTABLE.append(qRgb(int(i/4),int(i),int(i/2)))

		a = np.random.random(512*512)
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
		self.s_gain.setRange(1, 10)
		self.s_gain.setSingleStep(1)
		self.s_gain.valueChanged.connect(self.s_gain_changed)
		self.l_gain = QLabel('Gain')
		self.s_gain_changed()

		# Setup the offset slider
		self.s_offset = QSlider(Qt.Horizontal)
		self.s_offset.setRange(0, 100)
		self.s_offset.setSingleStep(1)
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
			gain = 1
			offset = 1
			brightness = 100
			self.worker = Worker(device, gain, offset, brightness)
			self.worker.signals.result.connect(self.print_output)
			self.worker.signals.finished.connect(self.thread_complete)
			self.worker.signals.line.connect(self.handle_line)
			self.threadpool.start(self.worker)

	def s_gain_changed(self):
		self.l_gain.setText(f'Gain = {self.s_gain.value()}')
	
	def s_offset_changed(self):
		self.l_offset.setText(f'Offset (black level) = {self.s_offset.value()} %')
	
	def s_brightness_changed(self):
		self.l_brightness.setText(f'LED Brightness = {self.s_brightness.value()} %')
	
	def handle_line(self, line):
		if type(line) == np.ndarray:
			if len(line) == 2048:
				self.image = np.roll(self.image, 1)
				img = QImage(self.image.data, 512, 512, QImage.Format_Indexed8)
				img.setColorTable(self.COLORTABLE)
				self.l_image_display.setPixmap(QPixmap.fromImage(img))

		

if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = App()
	sys.exit(app.exec_())