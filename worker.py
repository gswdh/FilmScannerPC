from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from multiprocessing.connection import Client
from scanner import Scanner
import time
import numpy as np

class WorkerSignals(QObject):
	finished = pyqtSignal()
	result = pyqtSignal(object)
	line = pyqtSignal(object)

class Worker(QRunnable):

	run = True

	def __init__(self, device, gain, offset, brightness):
		super(Worker, self).__init__()
		self.signals = WorkerSignals()
		self.device = device
		self.gain = gain
		self.offset = offset
		self.brightness = brightness

	@pyqtSlot()
	def run(self):
		self.scanner = Scanner()

		if self.scanner.start(self.device, self.gain, self.offset, self.brightness) != 'OKAY':
			# Could not connect
			self.signals.finished.emit()
			return

		data = np.array([])

		line_ctr = 0

		while self.run:
			data = np.append(data, np.frombuffer(self.scanner.receive(), dtype=np.uint8))
			if len(data):
				index = np.where(data == 255)
				if len(index):
					try:
						data = data[int(index[0][0]):]
						if len(data) > 2047:
							line = data[:2048]
							data = data[2048:]
							line_ctr = line_ctr + 1
							if line_ctr == 150:
								self.signals.line.emit(line[::4])
								line_ctr = 0
					except:
						pass

		# Finish up
		self.scanner.stop()
		self.signals.finished.emit()

	@pyqtSlot()
	def stop(self):
		self.run = False

	@pyqtSlot()
	def set_gain(self, gain):
		try:
			self.scanner.setGain(gain)
		except:
			pass

	@pyqtSlot()
	def set_offset(self, offset):
		try:
			self.scanner.setBlackLevel(offset)
		except:
			pass

	@pyqtSlot()
	def set_brightness(self, brightness):
		try:
			self.scanner.setLEDBrightness(brightness)
		except:
			pass

	