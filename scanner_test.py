import ftd2xx
import time

dev = ftd2xx.openEx(b'GS1FPQH5', 1)
time.sleep(0.1)
dev.setTimeouts(5000, 5000)
time.sleep(0.1)
dev.setBitMode(0xff, 0x00)
time.sleep(0.1)
dev.setBitMode(0xff, 0x40)
time.sleep(0.1)
dev.setUSBParameters(0x10000, 0x10000)
time.sleep(0.1)
dev.setLatencyTimer(2)
time.sleep(0.1)
dev.setFlowControl(ftd2xx.defines.FLOW_RTS_CTS, 0, 0)
time.sleep(0.1)
dev.purge(ftd2xx.defines.PURGE_RX)
time.sleep(0.1)
dev.purge(ftd2xx.defines.PURGE_TX)
time.sleep(0.1)

print(f"Device Details")
print(f"Serial = {dev.getDeviceInfo()['serial'].decode()}")
print(f"Type   = {dev.getDeviceInfo()['type']}")
print(f"ID     = {dev.getDeviceInfo()['id']}")
print(f"Desc   = {dev.getDeviceInfo()['description'].decode()}")

time.sleep(10)

dev.close()