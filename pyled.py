import serial.tools.list_ports

global s
ports = list(serial.tools.list_ports.comports())
for p in ports:
    if "Arduino" in p.description:
        print("{}: ".format(p))
        s = serial.Serial(p.device,115200)
        break

serialString = ""
line = ""
cmd ="c 0.100.0"
s.write((cmd + "\n").encode())
# s.write("m 0".encode())
while True:

    try:
            # serialPort.reset_input_buffer()
            # serialPort.reset_output_buffer()
            # serialString = serialPort.read(10).hex()
        for b in s.read():
            if b == b"":
                continue

            c = chr(b)

            if c == "\n":
                print(line)
                line = ""
                # cmd ="c 10.100.0" denn funkar
                # s.write((cmd + "\n").encode())
            elif c != "\r":
                line += c
    except KeyboardInterrupt:
        break

s.close()
