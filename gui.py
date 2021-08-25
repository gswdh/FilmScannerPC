import sys
from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication, QLineEdit, QSlider, QComboBox, QWidget, QLabel, QVBoxLayout, QHBoxLayout, QPushButton
from PyQt5.QtGui import QIcon, QPixmap

class App(QWidget):

	def __init__(self):
		super().__init__()
		
		# Window init
		self.setWindowTitle('GSWDH Film Scanner Controller')
		self.setGeometry(10, 10, 1200, 800)

		# GUI init

		# A place to put the scanned image 
		self.l_image_display = QLabel(self)

		self.l_image_display.setStyleSheet("background-color: lightgreen")


		# Controls panel
		b_refresh = QPushButton('Refresh')
		c_devices = QComboBox(self)
		t_scan_length = QLineEdit(self)
		b_start_stop = QPushButton('Start')
		s_gain = QSlider(QtCore.Qt.Horizontal)
		s_offset = QSlider(QtCore.Qt.Horizontal)
		s_led_brightness = QSlider(QtCore.Qt.Horizontal)



		vbox = QVBoxLayout()
		vbox.setAlignment(QtCore.Qt.AlignTop)
		vbox.addWidget(b_refresh)
		vbox.addWidget(c_devices)
		vbox.addWidget(QLabel('Scan Length (frames)'))
		vbox.addWidget(t_scan_length)
		vbox.addWidget(b_start_stop)
		vbox.addWidget(QLabel('Gain'))
		vbox.addWidget(s_gain)
		vbox.addWidget(QLabel('Offset (black level)'))
		vbox.addWidget(s_offset)
		vbox.addWidget(QLabel('LED Brightness'))
		vbox.addWidget(s_led_brightness)




		

		hbox = QHBoxLayout()
		hbox.addWidget(self.l_image_display, 5)
		hbox.addLayout(vbox, 1)



		self.setLayout(hbox)

	
		# Show
		self.show()
	
		

if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = App()
	sys.exit(app.exec_())