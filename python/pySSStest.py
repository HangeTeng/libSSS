import sys
sys.path.append(r'../build/')
import SSS
import multiprocessing


size = 2 
p = [1, 1]
num_threads = 1
Sip = "127.0.0.1:12222"
dest=range(size)

def receiver_process(result_queue):
    receiver = SSS.OSNReceiver()
    receiver.init(size=size, p=p, Sip=Sip, ot_type=1, num_threads=num_threads)
    a = receiver.run_osn()
    result_queue.put(a)

def sender_process(result_queue):
    sender = SSS.OSNSender()
    sender.init(size=size, dest=dest, p=p, Sip=Sip, ot_type=1, num_threads=num_threads)
    a = sender.run_osn()
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

    send = result_queue_send.get()
    recv = result_queue_recv.get()

    print("send result:", send)
    print("Receiver result:", recv)
    
    for i in range(size):
        print((send[i][0]+recv[1][i][0])% ((1<<64)+1))
        print(send[i][0])
        print(recv[0][i][0])
        print((1<<64)+1)
        print(recv[1][i][0])
        if((send[i][0]+recv[1][i][0])% ((1<<64)+1) != recv[0][i][0]):
            print("error")
            break
        print("correct")


# 线程资源会阻塞
# import threading

# if __name__ == "__main__":
#     receiver_thread = threading.Thread(target=receiver_process)
#     sender_thread = threading.Thread(target=sender_process)
    
#     print(1)
#     receiver_thread.start()
#     print(2)
#     sender_thread.start()
    
#     receiver_thread.join()
#     sender_thread.join()
