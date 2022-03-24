import sys
import signal
import threading
import datetime

from PyQt5.QtCore import QObject, pyqtSignal, QTimer, QCoreApplication, QThread, pyqtSlot


class Worker(QObject):
    timeChanged = pyqtSignal(object)

    def __init__(self, parent=None):
        QObject.__init__(self, parent)

        self.timer = QTimer(self)
        self.timer.setInterval(1000)

        self.timer.timeout.connect(self.main_process)

    @pyqtSlot()
    def start(self):
        self.timer.start()
        print("Worker thread {}: Start timer".format(threading.get_ident()))

    @pyqtSlot()
    def main_process(self):
        timestamp = datetime.datetime.now()
        print(
            "Worker thread {}: {}".format(
                threading.get_ident(), timestamp.strftime("%d-%m-%Y %H-%M-%S")
            )
        )
        self.timeChanged.emit(timestamp)


class WorkerThread(QObject):
    def __init__(self, parent=None):
        QObject.__init__(self, parent)

        self.emitter = Worker()
        self.thread = QThread(self)
        self.emitter.moveToThread(self.thread)

        self.thread.started.connect(self.emitter.start)
        self.thread.finished.connect(self.emitter.deleteLater)
        self.emitter.timeChanged.connect(self.show_time)

    @pyqtSlot()
    def start(self):
        self.thread.start()

    def stop(self):
        if self.thread.isRunning():
            self.thread.quit()
            self.thread.wait()
            print("Exit thread")

    @pyqtSlot(object)
    def show_time(self, timestamp):
        print(
            "Main   thread {}: {}".format(
                threading.get_ident(), timestamp.strftime("%d-%m-%Y %H-%M-%S")
            )
        )


def signal_handler(sig, frame):
    print("Quit")
    app.quit()


if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    app = QCoreApplication(sys.argv)

    timer = QTimer()
    timer.timeout.connect(lambda: None)
    timer.start(500)

    print("Main    thread {}".format(threading.get_ident()))
    emitter = WorkerThread()
    emitter.start()
    sys.exit(app.exec_())