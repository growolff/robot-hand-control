import serial
import time
import csv
import sys

ser = serial.Serial('COM7',115200)
ser.flushInput()

def main(name):

    filename = name

    while True:
        try:
            ser_bytes = ser.readline()
            decoded_bytes = (ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
            string = decoded_bytes.rstrip()
            indice,pulgar,medio,celda = string.split(',')
            print(indice,pulgar,medio,celda)
            with open("log_datos/" + filename + ".csv","a") as f:
                writer = csv.writer(f,delimiter=",")
                writer.writerow([time.time(),indice,pulgar,medio,celda])
        except Exception as e:
            print(e)
            #print("Keyboard Interrupt")
            break

if __name__ == '__main__':
    main(sys.argv[1])
