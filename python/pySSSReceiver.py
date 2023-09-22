import sys
sys.path.append(r'../build/')
import SSS

size = 2 
p = [1, 1]
num_threads=1
Sip="127.0.0.1:12222"

receiver = SSS.OSNReceiver()
receiver.init(size=size, p=p, Sip=Sip, ot_type =1, num_threads=num_threads)
a = receiver.run_osn()

print(a)