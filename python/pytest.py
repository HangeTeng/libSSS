import sys
sys.path.append(r'../build/')
import SSS
import threading
from threading import Thread
import time, signal


class Thread_ret(Thread):
    def __init__(self, func, kwargs):
        Thread.__init__(self)
        self.func = func
        self.kwargs = kwargs
        self.result = None
        self._stop_event = threading.Event()

    def run(self):
        try:
            self.result = self.func(**self.kwargs)
        except KeyboardInterrupt:
            self.stop()

    def getResult(self):
        return self.result
    
    def stop(self):
        self._stop_event.set()
    
    def stopped(self):
        return self._stop_event.is_set()


if __name__ == '__main__':
    size = 2 
    p = [1, 1]
    num_threads=1
    Sip="127.0.0.1:12222"


    sender = SSS.OSNSender()
    sender.init(size=size, dest=[1, 0], p=p, Sip=Sip, ot_type =1, num_threads=num_threads)

    receiver = SSS.OSNReceiver()
    receiver.init(size=size, p=p,  Sip=Sip,  ot_type =1, num_threads=num_threads)
   
    t1 = Thread_ret(sender.run_osn, kwargs={})
    t2 = Thread_ret(receiver.run_osn, kwargs={})

    # def signal_handler(sig, frame):
    #     print("Ctrl-C received, stopping threads...")
    #     t1.stop()
    #     t2.stop()

    # # 设置Ctrl-C信号处理器
    # signal.signal(signal.SIGINT, signal_handler)

    # 启动线程运行
    t2.start()
    t1.start()
    

    # # 等待线程完成
    # t1.join()
    # t2.join()

    print(t1.getResult())
    print(t2.getResult())

