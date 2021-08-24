from multiprocessing.connection import Listener

address = ('localhost', 6000)

listener = Listener(address, authkey=b'secret password')

while True:

	conn = listener.accept()

	# While the connection is good
	while True:

		try:
			data = conn.recv()
			print('Got data')

		except:
			break