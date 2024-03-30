import sys
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from scanner import Scanner
from worker import Worker
import numpy as np
import datetime
import json

mtr_speed = 20000

class App(QWidget):
    thread_running = False

    def __init__(self):
        super().__init__()

        # Window init
        self.setWindowTitle("GSWDH Film Scanner Controller")
        self.setWindowIcon(QIcon("logo.png"))

        self.image_w = 900
        self.image_h = 512

        # GUI init

        # A place to put the scanned image
        self.l_image_display = QLabel(self)

        # Refresh button
        self.b_refresh = QPushButton("Refresh")
        self.b_refresh.clicked.connect(self.b_refresh_clicked)

        # Start stop button
        self.b_start_stop = QPushButton("Start")
        self.b_start_stop.clicked.connect(self.b_start_stop_clicked)
        self.scanning = False

        # Manual or auto motor mode
        self.cb_manual_auto = QCheckBox("Manual Motor")

        # Left and right motor control
        self.b_mtr_left = QPushButton("<")
        self.b_mtr_left.pressed.connect(self.b_mtr_left_click)
        self.b_mtr_left.released.connect(self.b_mtr_left_release)
        self.b_mtr_right = QPushButton(">")
        self.b_mtr_right.pressed.connect(self.b_mtr_right_click)
        self.b_mtr_right.released.connect(self.b_mtr_right_release)

        # Scan parameters
        self.c_devices = QComboBox(self)
        self.b_refresh_clicked()
        self.t_scan_length = QLineEdit(self)

        # Film presets
        self.c_presets = QComboBox(self)
        self.c_presets.activated.connect(self.c_preset_selected)

        # Setup the gain slider
        self.s_gain = QSlider(Qt.Horizontal)
        self.s_gain.setRange(100, 500)
        self.s_gain.setSingleStep(1)
        self.s_gain.valueChanged.connect(self.s_gain_changed)
        self.l_gain = QLabel("Gain")
        self.s_gain_changed()

        # Setup the offset slider
        self.s_offset = QSlider(Qt.Horizontal)
        self.s_offset.setRange(0, 100)
        self.s_offset.setSingleStep(1)
        self.s_offset.setValue(59)
        self.s_offset.valueChanged.connect(self.s_offset_changed)
        self.l_offset = QLabel("Offset (black level)")
        self.s_offset_changed()

        # Setup the brightness slider
        self.s_brightness = QSlider(Qt.Horizontal)
        self.s_brightness.setRange(0, 100)
        self.s_brightness.setSingleStep(1)
        self.s_brightness.setValue(100)
        self.s_brightness.valueChanged.connect(self.s_brightness_changed)
        self.l_brightness = QLabel("LED Brightness")
        self.s_brightness_changed()

        # Mean line value
        self.l_line_value = QLabel("Mean Line Value")

        # Line sharpness value
        self.l_line_sharpness = QLabel("Line Sharpness")

        # Min and max line value
        self.l_line_max = QLabel("Line Max Value")
        self.l_line_min = QLabel("Line Min Value")

        # Calibration button
        self.b_cal = QPushButton("Calibrate")
        self.b_cal.pressed.connect(self.b_cal_click)

        # Layout
        vbox = QVBoxLayout()
        vbox.setAlignment(Qt.AlignTop)
        vbox.addWidget(QLabel("Scanner"))
        vbox.addWidget(self.b_refresh)
        vbox.addWidget(self.c_devices)
        vbox.addWidget(QLabel("Scan Setting Presets"))
        vbox.addWidget(self.c_presets)
        vbox.addWidget(QLabel("Scan Length (frames)"))
        vbox.addWidget(self.t_scan_length)
        vbox.addWidget(self.b_start_stop)
        vbox.addWidget(self.cb_manual_auto)
        hbox = QHBoxLayout()
        hbox.addWidget(self.b_mtr_left)
        hbox.addWidget(self.b_mtr_right)
        vbox.addLayout(hbox)
        vbox.addWidget(self.l_gain)
        vbox.addWidget(self.s_gain)
        vbox.addWidget(self.l_offset)
        vbox.addWidget(self.s_offset)
        vbox.addWidget(self.l_brightness)
        vbox.addWidget(self.s_brightness)
        vbox.addWidget(self.l_line_value)
        vbox.addWidget(self.l_line_sharpness)
        vbox.addWidget(self.l_line_max)
        vbox.addWidget(self.l_line_min)
        vbox.addWidget(QLabel())
        vbox.addWidget(self.b_cal)

        hbox = QHBoxLayout()
        hbox.addWidget(self.l_image_display, 4)
        hbox.addLayout(vbox, 1)

        self.setLayout(hbox)

        # Some inits
        a = np.zeros(self.image_h * self.image_w)
        a = np.reshape(a, (self.image_w, self.image_h))
        self.image = np.require(a, np.uint8, "C")

        img = QImage(
            self.image.data, self.image_h, self.image_w, QImage.Format_Indexed8
        )
        img = img.transformed(QTransform().rotate(90))
        self.l_image_display.setPixmap(QPixmap.fromImage(img))

        self.c_add_presets()
        self.c_preset_selected()

        # Show
        self.show()
        self.setFixedSize(self.size())

    def b_refresh_clicked(self):
        scnr = Scanner()
        devices = scnr.list_devices()
        self.c_devices.clear()
        for device in devices:
            self.c_devices.addItem(device)

    def b_start_stop_clicked(self):
        if self.scanning:
            text, okPressed = QInputDialog.getText(
                self,
                "Image File Name",
                "File Name:",
                QLineEdit.Normal,
                f"image {str(datetime.datetime.now()).split('.')[0].replace(':', '-')}",
            )
            self.worker.stop(text)

            # Do some things here.
            self.scanning = False
            self.b_start_stop.setText("Start")

        else:
            if not str(self.c_devices.currentText()):
                return
            # Do some things here.
            self.scanning = True
            self.b_start_stop.setText("Stop")

            self.threadpool = QThreadPool()
            device = str(self.c_devices.currentText())
            gain = float(self.s_gain.value()) / 100.0
            offset = float(self.s_offset.value()) / 100.0
            brightness = float(self.s_brightness.value()) / 100.0
            try:
                nlines = int(int(self.t_scan_length.text()) * 2048 * 1.5 * 1.2)
            except:
                nlines = 0
            manual_mode = self.cb_manual_auto.isChecked()
            self.worker = Worker(device, gain, offset, brightness, nlines, manual_mode, mtr_speed)
            self.worker.signals.line.connect(self.t_handle_line)
            self.worker.signals.lines_done.connect(self.t_lines_done)
            self.worker.signals.finished.connect(self.t_quit)
            self.thread_running = True
            self.threadpool.start(self.worker)

    def s_gain_changed(self):
        self.l_gain.setText(f"Gain = {self.s_gain.value() / 100.0}")
        try:
            self.worker.set_gain(float(self.s_gain.value()) / 100.0)
        except:
            pass

    def s_offset_changed(self):
        self.l_offset.setText(f"Offset (black level) = {self.s_offset.value()} %")
        try:
            self.worker.set_offset(float(self.s_offset.value()) / 100.0)
        except:
            pass

    def s_brightness_changed(self):
        self.l_brightness.setText(f"LED Brightness = {self.s_brightness.value()} %")
        try:
            self.worker.set_brightness(float(self.s_brightness.value()) / 100.0)
        except:
            pass

    def t_quit(self):
        self.thread_running = False

    def t_lines_done(self):
        self.b_start_stop_clicked()

    def t_handle_line(self, line):
        if type(line) == np.ndarray:
            self.image = np.roll(self.image, shift=150, axis=0)
            self.image[0 : len(line)] = np.flip(line)
            img = QImage(
                self.image.data, self.image_h, self.image_w, QImage.Format_Indexed8
            )
            img = img.transformed(QTransform().rotate(90))
            self.l_image_display.setPixmap(QPixmap.fromImage(img))
            line = line[0]
            self.l_line_value.setText(f"Mean Line Value = {int(np.mean(line))}")
            gx = np.gradient(line) ** 2
            sharpness = np.average(gx)
            self.l_line_sharpness.setText(
                f"Line Sharpness = {round(float(sharpness), 3)}"
            )
            self.l_line_max.setText(f"Line Max Value = {int(np.max(line[50:-50]))}")
            self.l_line_min.setText(f"Line Min Value = {int(np.min(line[50:-50]))}")

    def c_preset_selected(self):
        if self.c_presets.currentText() == "Default":
            self.s_gain.setValue(100)
            self.s_offset.setValue(57)
            self.s_brightness.setValue(100)
        else:
            for film in self.film_presets["films"]:
                if film["name"] == self.c_presets.currentText():
                    gain = int(film["gain"] * 100)
                    offset = int(film["offset"])
                    brightness = int(film["brightness"])
                    if gain > 500:
                        gain = 500
                    if gain < 100:
                        gain = 100
                    if offset > 100:
                        offset = 100
                    if offset < 0:
                        offset = 0
                    if brightness > 100:
                        brightness = 100
                    if brightness < 0:
                        brightness = 0
                    self.s_gain.setValue(gain)
                    self.s_offset.setValue(offset)
                    self.s_brightness.setValue(brightness)

    def c_add_presets(self):
        self.c_presets.clear()
        self.c_presets.addItem("Default")

        with open("film_presets.json") as json_file:
            self.film_presets = json.load(json_file)

        for film in self.film_presets["films"]:
            self.c_presets.addItem(film["name"])

    def b_mtr_left_click(self):
        self.worker.set_motor_velocity(mtr_speed, 0)
        self.worker.set_motor_enable(True)

    def b_mtr_left_release(self):
        self.worker.set_motor_enable(False)

    def b_mtr_right_click(self):
        self.worker.set_motor_velocity(mtr_speed, 1)
        self.worker.set_motor_enable(True)

    def b_mtr_right_release(self):
        self.worker.set_motor_enable(False)

    def b_cal_click(self):
        self.worker.calibrate()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.setWindowIcon(QIcon("logo.png"))
    ex = App()
    exit_handler = app.exec_()
    try:
        ex.worker.stop("")
    except:
        pass
    sys.exit(exit_handler)
