from pySSSComm import Sender
import threading

if __name__ == "__main__":
    size = 6
    p = [0,1]
    num_threads = 1
    Sip = "127.0.0.1:12222"
    dest=range(size)

    sender = Sender()
    sender.init(size=size, dest=dest, p=p, Sip=Sip, ot_type=1, num_threads=num_threads)
    a = sender.run()

    print_lock = threading.Lock()
    with print_lock:
        print("server:")
        print(a)
