
'''
update_reg[128] = MSB{ UPD[1], RESERVED[14], GP_REG[16], ADDR[64], DATA[32], CLR[1] }
command_byte[8] = MSB{ ADDR[7], DATA[1] }
'''

def gs_make_packet(addr, data):
	
	packet = bytearray(128)

	# Init the packet
	for i in range(128):
		packet[i] = (i << 1)

	# Put the address and data into the packet
	for i in range(32):
		if (data & (1 << i)):
			packet[i+1] = packet[i+1] + 1
		if (addr & (1 << i)):
			packet[i+33] = packet[i+33] + 1

	# Set the UPD bit
	packet[127] = packet[127] + 1

	return packet


if __name__ == '__main__':

	addr = 100
	data = 455

	p = gs_make_packet(addr, data)

	for byte in p:
		print(hex(byte))