import os


def scanner_encode(data):

	# Find any instances of 3 0xFF bytes in series, add a 0x00 after each series
	data = data.replace(b'\xFF\xFF\xFF', b'\xFF\xFF\xFF\x00')

	# Add the preample in front
	return bytearray((0xFF, 0xFF, 0xFF, 0xFF)) + data

def scanner_decode(data):

	

data = bytearray()

# Some data
data = data + os.urandom(2048*2)
