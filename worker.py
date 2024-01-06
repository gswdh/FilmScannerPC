from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from multiprocessing.connection import Client
from scanner import Scanner
import time
import numpy as np
import cv2


class WorkerSignals(QObject):
    finished = pyqtSignal()
    result = pyqtSignal(object)
    line = pyqtSignal(object)
    lines_done = pyqtSignal()


class Worker(QRunnable):
    run = True

    def __init__(self, device, gain, offset, brightness, nlines):
        super(Worker, self).__init__()
        self.signals = WorkerSignals()
        self.device = device
        self.gain = gain
        self.offset = offset
        self.brightness = brightness
        self.nlines = nlines

    @pyqtSlot()
    def run(self):
        self.scanner = Scanner()
        self.file_name = ""
        self.proceed = False

        if (
            self.scanner.start(self.device, self.gain, self.offset, self.brightness)
            != "OKAY"
        ):
            self.signals.finished.emit()
            return

        data = np.array([])
        output_data = []
        disp_lines = []

        line_ctr = 0

        while self.run:
            data = np.append(
                data, np.frombuffer(self.scanner.receive(), dtype=np.uint8)
            )
            if len(data):
                index = np.where(data == 255)
                if len(index):
                    try:
                        data = data[int(index[0][0]) :]
                        if len(data) > 2047:
                            line = data[:2048]
                            data = data[2048:]
                            line_ctr = line_ctr + 1
                            if not (line_ctr % 150):
                                self.signals.line.emit(
                                    np.array(disp_lines, dtype=np.uint8)
                                )
                                disp_lines = []
                            disp_lines.append(line[::4])
                            output_data.append(line[1:])
                    except:
                        pass
            if self.nlines:
                if line_ctr > self.nlines:
                    self.signals.lines_done.emit()
                    self.run = False

        # Wait for something
        while not self.proceed:
            pass

        # If the filename contains something, write out
        if self.file_name:
            self.file_name = str(self.file_name.split(".")[0])
            self.file_name = self.file_name + ".png"
            # Write the output data to file
            output_data = np.array(output_data, dtype=np.uint8)
            img = cv2.cvtColor(output_data, cv2.COLOR_GRAY2BGR)
            img = cv2.rotate(img, cv2.ROTATE_90_COUNTERCLOCKWISE)
            cv2.imwrite(self.file_name, img)

        # Finish up
        self.scanner.stop()
        self.signals.finished.emit()

    @pyqtSlot()
    def stop(self, file_name):
        self.file_name = file_name
        self.proceed = True
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
