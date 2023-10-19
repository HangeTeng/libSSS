import sys
sys.path.append(r'../build/')
import multiprocessing
from pySSSComm import Sender,Receiver


size = 60
p = [0,0,1]
num_threads = 1
Sip = "127.0.0.1:12222"
dest=range(size)
element_size = 2

def receiver_process(result_queue):
    receiver = Receiver(4)
    a = receiver.run(size=size, p=p, Sip=Sip, ot_type=1, sessionHint="1",num_threads=num_threads)
    print("receiver run!")
    result_queue.put(a)

def sender_process(result_queue):
    sender = Sender(4)
    a = sender.run(size=size, dest=dest, p=p, Sip=Sip, ot_type=1, sessionHint="1",num_threads=num_threads)
    print("sender run!")
    result_queue.put(a)


if __name__ == "__main__":
    result_queue_recv = multiprocessing.Queue()
    result_queue_send = multiprocessing.Queue()
    
    receiver_proc = multiprocessing.Process(target=receiver_process, args=(result_queue_recv,))
    sender_proc = multiprocessing.Process(target=sender_process, args=(result_queue_send,))
    
    receiver_proc.start()
    sender_proc.start()
    
    receiver_proc.join()
    sender_proc.join()

    print("send get")
    send = result_queue_send.get()
    print("recv get")
    recv = result_queue_recv.get()
    
    print("checking")
    for i in range(size):
        # print(send[i]+recv[1][i])
        if((send[i]+recv[1][i])% (((p[2] if len(p)==3 else 0)<< 128)+(p[1]<<64)+p[0]) != recv[0][i]):
            print("error!")
            break
    if i == size-1:
        print("correct!")
