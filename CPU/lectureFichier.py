#! usr/bin/python2.7
# -*- coding: utf-8 -*-

import time
import sys
from PyQt5.QtWidgets import (QWidget, QGridLayout, QPushButton, QApplication)
from PyQt5.QtGui import QPainter, QColor, QFont
from PyQt5.QtCore import QTimer

class Jauge(QWidget):
	

	def __init__(self):
		super(Jauge, self).__init__()
		self.width = 80
		self.height = 200
		self.initUI()
       
	def initUI(self):               
		self.setGeometry(300, 300, 100, 250)        
		self.setWindowTitle('Jauge en PyQt5')   

		self.show()

	def paintEvent(self, event):
		qp = QPainter()
		qp.begin(self)
		self.drawRects(event, qp)
		qp.end()

	def drawRects(self, event, qp):
      
		qp.setBrush(QColor("lime"))
		qp.drawRect(10,25,self.width,self.height)

		self.monitor(event,"/Users/kant1_sahal/Desktop/QuentinCocoon/interfacePython/file.txt", qp )

	def monitor(self, event, rfile, qp): 
		fd = open(rfile, "rw")
		content = fd.read()

		h = ord(content)

		qp.setBrush(QColor("black"))
		qp.drawRect(10,25,self.width,self.height - h*self.height/100) 

		fd.close()  

if __name__ == '__main__':
	app = QApplication(sys.argv)
	jauge = Jauge()


	timer = QTimer()
	timer.timeout.connect(jauge.update)
	timer.start(100)
		

	
	sys.exit(app.exec_())


	





