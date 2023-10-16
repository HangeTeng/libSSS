from pySSSComm import Receiver
import threading

if __name__ == "__main__":
    size = 6
    p = [0,1]
    num_threads = 1
    Sip = "127.0.0.1:12222"
    dest=range(size)

    receiver = Receiver()
    receiver.init(size=size, p=p, Sip=Sip, ot_type=1, num_threads=num_threads)
    a = receiver.run()

    print_lock = threading.Lock()
    with print_lock:
        print("receiver:")
        print(a)