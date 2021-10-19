import serial.tools.list_ports
import time
import sys
from icecream import ic

def init():
    serial_handler = None
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        if "Arduino" in p.description:
            print("{}: ".format(p))
            serial_handler = serial.Serial(p.device,115200)
            time.sleep(2)
            break
    return serial_handler

def set(serial_l,red,green,blue):
    cmd = f"c {red}.{green}.{blue}"
    serial_l.write((cmd + "\n").encode())

def end(serial_handler):
    if(serial_handler):
        serial_handler.close()

def main():
    t_activity = sys.argv[1]
    t_status = sys.argv[2]
    ic(t_activity)
    ic(t_status)
    serial_l = init()
    if(serial_l is not None):
        # if t_activity == "Away":
        #     set(serial_l,255,200,0)
        # elif t_activity == "Available":
        #     set(serial_l,0,255,0)
        if t_activity == "In a meeting" or t_activity == "In a call":
            set(serial_l,255,0,0)
        else:
            set(serial_l,0,255,0)
    else:
        print("no arduino board found")
    end(serial_l)

# Using the special variable
# __name__
if __name__=="__main__":
    main()