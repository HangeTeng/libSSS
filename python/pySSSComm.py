import sys
sys.path.append(r'../build/')
import SSS
import numpy as np

class Sender(SSS.OSNSender):
    def __init__(self):
        super().__init__()

    def run(self):
        result = []
        a_list = super().run_osn()
        for a in a_list:
            result.append(a[0] + (a[1]<<64))
        return np.array(result)
    
class Receiver(SSS.OSNReceiver):
    def __init__(self):
        super().__init__()

    def run(self):
        result_1 = []
        result_2 = []
        a_list = super().run_osn()
        for a in a_list[0]:
            result_1.append(a[0] + (a[1]<<64))
        for a in a_list[1]:
            result_2.append(a[0] + (a[1]<<64))
        return np.array([result_1,result_2])
