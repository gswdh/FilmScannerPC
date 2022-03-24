import ftd2xx
import time
import gsbus

MTR_EN_ADDR = 0
MTR_DIR_ADDR = 1
MTR_SPEED_ADDR = 2
LED_PWM_ADDR = 3
SCAN_EN_ADDR = 4
SCAN_SUB_SAMPLE_ADDR = 5
SCAN_FRAME_RATE_ADDR = 6
DAC_GAIN_ADDR = 7
DAC_OFFSET_ADDR = 8

class Scanner:

	def __init__(self):
		self.dev = False

	def transmit(self, data):
		self.dev.write(data)

	def receive(self):
		pass

	def setReg(self, addr, data):

		# Assemble a new packet
		packet = gsbus.gs_make_packet(addr, data)

		# Send the new packet
		self.transmit(packet.decode("utf-8", errors='ignore'))

	def setBlackLevel(self, level):
		self.setReg(DAC_OFFSET_ADDR, int(level))

	def setGain(self, gain):
		gain_level = 65535 / gain
		if gain_level > 65535:
			gain_level = 65535
		self.setReg(DAC_GAIN_ADDR, int(gain_level))		

	def setMotorSpeed(self, speed, direc):
		self.setReg(MTR_SPEED_ADDR, int(speed))
		self.setReg(MTR_DIR_ADDR, int(direc))

	def setLEDBrightness(self, level):
		pwm_level = 255 * level;
		if pwm_level > 255:
			pwm_level = 255
		self.setReg(LED_PWM_ADDR, int(pwm_level))

	def setScanEnable(self, state):
		self.setReg(SCAN_EN_ADDR, int(state))

	def setScanSampleMode(self, mode):
		self.setReg(SCAN_SUB_SAMPLE_ADDR, int(mode))

	def setScanFrameRate(self, rate):
		self.setReg(SCAN_FRAME_RATE_ADDR, int(rate))

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
		
		# Connect to the scanner
		self.connect(device)

		try:
			if self.dev.getDeviceInfo()['serial'].decode() != device:
				# Failed to connect
				return 'FAIL'
		except:
			# Failed to init
			return 'FAIL'

		# Setup the black level
		self.setBlackLevel(38275)

		# Set the gain to the lowest
		self.setGain(1)

		# Set the motor speed for full speed scan
		self.setMotorSpeed(37306, 1)

		# Set the LED value to off
		self.setLEDBrightness(0)

		# Full resolution scan
		self.setScanSampleMode(0)

		# Full frame rate
		self.setScanFrameRate(0)

		# Stop any scanning
		self.setScanEnable(1)

		return 'OKAY'

	def stop(self):
		# Stop any scanning
		self.setScanEnable(0)

		# Disconnect from the FT232H
		self.disconnect()









if __name__ == "__main__":

	scanner = Scanner()

	print(scanner.list_devices())