import sys
sys.path.append(r'../build/')
import SSS
import numpy as np

class Sender(SSS.OSNSender):
    def run(self,size,dest, p = [0,0,1], Sip="127.0.0.1:12222", ot_type=1, sessionHint="",num_threads=1):
        result = []
        a_list = super().run(size=size,dest=dest, p=p, Sip=Sip, ot_type=ot_type, sessionHint=sessionHint,num_threads=num_threads)
        for a in a_list:
            result.append(a[0] + (a[1]<<64))
        return np.array(result)
    
class Receiver(SSS.OSNReceiver):
    def run(self,size, p = [0,0,1], Sip="127.0.0.1:12222", ot_type=1, sessionHint="",num_threads=1):
        result_1 = []
        result_2 = []
        a_list = super().run(size=size,p=p, Sip=Sip, ot_type=ot_type, sessionHint=sessionHint,num_threads=num_threads)
        for a in a_list[0]:
            result_1.append(a[0] + (a[1]<<64))
        for a in a_list[1]:
            result_2.append(a[0] + (a[1]<<64))
        return np.array([result_1,result_2])
