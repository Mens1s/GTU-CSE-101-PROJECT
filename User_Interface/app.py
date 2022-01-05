#self.setWindowIcon(./gtuLogo.jpg)
#clicked=self.function
from PyQt5 import QtWidgets 
from PyQt5 import QtGui 
from PyQt5 import QtCore 
import sys
import numpy as np
import cv2
from deeplearning import face_mask_prediction

class VideoCapture(QtCore.QThread):
    change_pixmap_signal = QtCore.pyqtSignal(np.ndarray)

    def __init__(self):
        super().__init__()
        self.run_stament = True
    def run(self):
        capture = cv2.VideoCapture(0)
        
        while self.run_stament:
            ret, frame = capture.read()
            prediction_image = face_mask_prediction(frame)
                      
            if ret == True:
                self.change_pixmap_signal.emit(prediction_image)
                
        capture.release()    

    def stop(self):
        self.run_stament = False
        self.wait()

class mainWindow(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowIcon(QtGui.QIcon("./images/icon.png")) #windows icon
        self.setWindowTitle("FACE MASK RECOGNITION by GTU")
        self.setFixedSize(600,600)
        
        #widgets 
        label = QtWidgets.QLabel("<h2>Health Automation System by <b>GTU Students</b></h2>")
        self.cameraButton = QtWidgets.QPushButton("Open Camera",clicked=self.cameraButtonClick, checkable=True)
        
        
        #screen
        self.screen = QtWidgets.QLabel()
        self.image = QtGui.QPixmap(600,400)
        self.image.fill(QtGui.QColor('darkGrey'))
        self.screen.setPixmap(self.image)
        
        #layout
        layout = QtWidgets.QVBoxLayout()
        layout.addWidget(label)
        layout.addWidget(self.cameraButton)
        layout.addWidget(self.screen)
        
        
        self.setLayout(layout)
        
        self.show()
        
    def cameraButtonClick(self):
        
        status = self.cameraButton.isChecked()
        if status == True:
            self.cameraButton.setText("Close Camera")
            
            #open
            self.capture = VideoCapture()
            self.capture.change_pixmap_signal.connect(self.updateImage)
            self.capture.start()
            
        
        elif status == False:
            self.cameraButton.setText("Open Camera")
            self.capture.stop()
            
    @QtCore.pyqtSlot(np.ndarray)
    def updateImage(self,image_array):
        rgb_image = cv2.cvtColor(image_array,cv2.COLOR_BGR2RGB)
        h,w,ch = rgb_image.shape
        bytes_per_line = ch*w     
              
        convertedImg = QtGui.QImage(rgb_image.data,w,h,bytes_per_line,QtGui.QImage.Format_RGB888)
        scaleImg = convertedImg.scaled(600,400,QtCore.Qt.KeepAspectRatio)      
        qt_image = QtGui.QPixmap.fromImage(scaleImg)
        
        self.screen.setPixmap(qt_image)
              
if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    mw = mainWindow()
    sys.exit(app.exec())