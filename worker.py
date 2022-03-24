from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from multiprocessing.connection import Client
from scanner import Scanner

class WorkerSignals(QObject):
	finished = pyqtSignal()
	result = pyqtSignal(object)



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
		# Do some tasks...

		self.signals.result.emit("Thread running")

		self.scanner = Scanner()

		if self.scanner.start(self.device, self.gain, self.offset, self.brightness) != 'OKAY':
			# Could not connect
			self.signals.finished.emit()
			return

		while self.run:
			pass
			
		# Finish up
		self.scanner.stop()
		self.signals.finished.emit()

	@pyqtSlot()
	def stop(self):
		self.run = False