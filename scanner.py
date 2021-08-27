import ftd2xx
import time


class Scanner:

	def __init__(self):
		self.dev = False

	def list_devices(self):
		devs = []
		try:
			for dev in ftd2xx.listDevices():
				dev = dev.decode()
				if 'GS' in dev:
					devs.append(dev)
		except:
			pass
		return devs

	def connect(self, device):
		self.dev = ftd2xx.openEx(device.encode(), 1)
		time.sleep(0.1)
		self.dev.setTimeouts(5000, 5000)
		time.sleep(0.1)
		self.dev.setBitMode(0xFF, 0x00)
		time.sleep(0.1)
		self.dev.setBitMode(0xFF, 0x40)
		time.sleep(0.1)
		self.dev.setUSBParameters(0x10000, 0x10000)
		time.sleep(0.1)
		self.dev.setLatencyTimer(2)
		time.sleep(0.1)
		self.dev.setFlowControl(ftd2xx.defines.FLOW_RTS_CTS, 0, 0)
		time.sleep(0.1)
		self.dev.purge(ftd2xx.defines.PURGE_RX)
		time.sleep(0.1)
		self.dev.purge(ftd2xx.defines.PURGE_TX)
		time.sleep(0.1)

	def disconnect(self):
		self.dev.close()
		self.dev = False

	def start(self, device, gain, offset, brightness, ip, control_port, data_port):
		self.connect(device)
		try:
			if self.dev.getDeviceInfo()['serial'].decode() != device:
				# Failed to connect
				return 'FAIL'
		except:
			# Failed to init
			return 'FAIL'



		return 'OKAY'

	def stop(self):
		self.disconnect()






if __name__ == "__main__":

	scanner = Scanner()

	print(scanner.list_devices())