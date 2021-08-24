from multiprocessing.connection import Client
import numpy as np
import time

address = ('localhost', 6000)

data = b'\0' * int(5e3)
sends = 100

data_len = len(data) * sends

conn = Client(address, authkey=b'secret password')

start = time.time()

for i in range(sends):
	conn.send(data)

stop = time.time()

t_period = stop - start

bytes_s = data_len / t_period

print(f'Speed was {round(bytes_s / 1e6, 3)}MB/s for {data_len / 1e6}MB')

conn.close()