import re
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
    serial_l.write(("m 0\n").encode())
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
        if t_status == "Away" or t_status == "Be right back" or t_status == "Offline":
            serial_l.write(("b 0\n").encode())
        elif t_activity == "In a meeting" or t_activity == "In a call":
            serial_l.write(("b 255\n").encode())
            time.sleep(1)
            set(serial_l,255,0,0)
        elif t_activity == "calling":
            serial_l.write(("b 255\n").encode())
            time.sleep(1)
            serial_l.write(("m 52\n").encode())
        else:
            serial_l.write(("b 255\n").encode())
            time.sleep(1)
            set(serial_l,0,255,0)
    else:
        print("no arduino board found")
    end(serial_l)

if __name__=="__main__":
    main()
