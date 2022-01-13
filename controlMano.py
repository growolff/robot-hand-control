# -*- coding: utf-8 -*-

from ManoSerial import ManoSerial
import time as t

TO_SS1 = 11
TO_SS2 = 12
SEND_DATA_TRUE = 40
SEND_DATA_FALSE = 41
SET_POS_REF = 1

def readMotorData():

  t.sleep(1)
  mega.sendCmd(s1=SEND_DATA_TRUE)

  a = range(0,60)
  for i in a:
      mega.sendCmd(s1=TO_SS1,s2=0xEE)
      print(mega.getMsg())
      t.sleep(0.1)

  mega.sendCmd(s1=SEND_DATA_FALSE)
  t.sleep(0.5)
  mega.stopProcess()

def test():
  t.sleep(1)
  mega.sendCmd(s1=SEND_DATA_TRUE)
  t.sleep(0.1)
  mega.sendCmd(s1=TO_SS1,s2=SET_POS_REF,s3=200,s4=0)
  mega.sendCmd(s1=TO_SS1,s2=SET_POS_REF,s3=200,s4=0)
  t.sleep(1)
  mega.sendCmd(s1=TO_SS1,s2=SET_POS_REF,s3=50)
  mega.sendCmd(s1=TO_SS1,s2=SET_POS_REF,s3=50)
  t.sleep(1)
  mega.sendCmd(s1=TO_SS1,s2=SET_POS_REF,s3=10)
  mega.sendCmd(s1=TO_SS1,s2=SET_POS_REF,s3=10)
  t.sleep(0.5)
  mega.sendCmd(s1=SEND_DATA_FALSE)
  t.sleep(0.5)

  mega.stopProcess()

if __name__ == '__main__':
  mega = ManoSerial('COM5',500000)
  mega.startProcess()
  try:
      readMotorData()
      #test()
  except Exception as e:
      print(e)
      mega.stopProcess()
