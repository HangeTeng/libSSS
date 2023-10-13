import sys
sys.path.append(r'../build/')
import SSS

size = 2 
p = [1, 0]
num_threads=1
Sip="127.0.0.1:12222"


sender = SSS.OSNSender()
sender.init(size=size, dest=[0, 1], p=p, Sip=Sip, ot_type =1, num_threads=num_threads)
a = sender.run_osn()

print(a)