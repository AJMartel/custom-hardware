# -*- coding: utf-8-sig -*-
import wx
import matplotlib
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg
import matplotlib.pyplot as plt
from matplotlib import rc
from class_sensor import *
import numpy as np
import datetime

class dataLogWindow(wx.Frame):
    '''Data logging panel for controlling sensors and data viewing.'''
    # create main application window #
    def __init__(self, parent, title):        
        wx.Frame.__init__(self, parent, title=title, size=(400,400))
        
        self.CreateStatusBar()     # add status bar
        # create menus #
        infoMenu = wx.Menu()       # add info menu
        infoAbout = infoMenu.Append(wx.ID_ABOUT, "&About", "Information about the program")
        self.Bind(wx.EVT_MENU, self.OnAbout, infoAbout)
        infoExit = infoMenu.Append(wx.ID_EXIT,"E&xit","Terminate the program")
        self.Bind(wx.EVT_MENU, self.OnExit, infoExit)
        # add menus to menu bar #
        menuBar = wx.MenuBar()
        menuBar.Append(infoMenu, "&Info")
        self.SetMenuBar(menuBar)
        
        self.logger = wx.TextCtrl(self, size=(200,300), style=wx.TE_MULTILINE | wx.TE_READONLY)
        self.logger.AppendText('logging initialised\n')
        
        # add interactive graph window of data #
        self.dataView = dataGraph(self)
        self.controller = controlPanel(self)
        # add panel of controls for each sensor #
        self.sensors = []
        
        # set sizing #
        self.top_sizer = wx.BoxSizer(wx.HORIZONTAL)
        self.top_sizer.Add(self.dataView, 0, wx.EXPAND)
        self.top_sizer.Add(self.logger, 0, wx.EXPAND)
        self.top_sizer.Add(self.controller, 0, wx.EXPAND)
        self.bottom_sizer = wx.BoxSizer(wx.HORIZONTAL)
        #self.bottom_sizer.AddMany(self.sensors)
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.top_sizer, 0, wx.EXPAND)
        self.sizer.Add(self.bottom_sizer, 0, wx.EXPAND)
        self.SetSizer(self.sizer)
        self.SetAutoLayout(1)
        self.sizer.Fit(self)
        
        # create timer #
        self.isLogging = False
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.update, self.timer)
        
        self.Show()
        
    def OnAbout(self, event):
        # A message dialog box with an OK button. wx.OK is a standard ID in wxWidgets.
        desc = "Data Logger v0.0\nA.Sanders\nalansanders89@gmail.com"
        dlg = wx.MessageDialog(self, desc, "Data Logger", wx.OK)
        dlg.ShowModal() # show dialog
        dlg.Destroy() # destroy it when finished.
        
    def OnExit(self, event):
        self.Close(True)  # Close the frame.
    
    def update(self, event):
        pass

class controlPanel(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        
        # create time view controls #
        parent.dataView.deltaDays = 1
        self.setDays = wx.TextCtrl(self, value="1", pos=(5,5), size=(100,20))
        self.Bind(wx.EVT_TEXT, self.getParams, self.setDays)
        self.setButton = wx.Button(self, label="Manual Time", pos=(5,30), size=(120,25))
        self.setButton.SetFont(wx.Font(10, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False))
        self.setButton.Bind(wx.EVT_BUTTON, self.manualParams)
        self.autoButton = wx.Button(self, label="Auto Time", pos=(5,60), size=(120,25))
        self.autoButton.SetFont(wx.Font(10, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False))
        self.autoButton.Bind(wx.EVT_BUTTON, self.autoParams)
    
        # create check controls #
        self.checkPeriod = 60
        self.setCheckPeriod = wx.TextCtrl(self, value="60", pos=(5,90), size=(100,20))
        self.Bind(wx.EVT_TEXT, self.getCheckPeriod, self.setCheckPeriod)
        self.setCheckPeriodButton = wx.Button(self, label="Set Check Period", pos=(5,115), size=(120,25))
        self.setCheckPeriodButton.SetFont(wx.Font(10, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False))
        self.setCheckPeriodButton.Bind(wx.EVT_BUTTON, self.updateCheckPeriod)
    
        # sensor creation controls #
        self.sensorID = "arduino"
        self.addSensorID = wx.TextCtrl(self, value="arduino", pos=(5,145), size=(100,20))
        self.Bind(wx.EVT_TEXT, self.getSensorID, self.addSensorID)
        self.addSensorButton = wx.Button(self, label="Add Sensor", pos=(5,170), size=(120,25))
        self.addSensorButton.SetFont(wx.Font(10, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False))
        self.addSensorButton.Bind(wx.EVT_BUTTON, self.addSensor)
        self.remSensorButton = wx.Button(self, label="Remove Sensor", pos=(5,195), size=(120,25))
        self.remSensorButton.SetFont(wx.Font(10, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False))
        self.remSensorButton.Bind(wx.EVT_BUTTON, self.removeSensor)
    
    def getParams(self, event):
        self.GetParent().dataView.deltaDays = float(event.GetString())
        logger = self.GetParent().logger
        logger.AppendText("Time range set to {} days\n".format(self.GetParent().dataView.deltaDays))
        fig = self.GetParent().dataView
        sensorPan = self.GetParent().sensors[0]
        if not fig.useAuto:
            past = datetime.datetime.now() - datetime.timedelta(days = fig.deltaDays)
            fig.x_min = past
            fig.x_max = datetime.datetime.now()
            if fig.x_min > sensorPan.sensor.timestamp.min():
                xi_min = np.where(sensorPan.sensor.timestamp >= fig.x_min)[0][0]
                xi_max = np.where(sensorPan.sensor.timestamp <= fig.x_max)[0][-1]
                tem_temp = sensorPan.sensor.temperature[xi_min:xi_max]
                tem_hum = sensorPan.sensor.humidity[xi_min:xi_max]
            else:
                tem_temp = sensorPan.sensor.temperature
                tem_hum = sensorPan.sensor.humidity
            fig.y_min = tem_temp.min()
            fig.y_max = tem_temp.max()
            fig.y2_min = tem_hum.min()
            fig.y2_max = tem_hum.max()
            fig.setup_axes()
            fig.canvas.draw()
        
    def manualParams(self, event):
        fig = self.GetParent().dataView
        fig.useAuto = False
        fig.ax.autoscale(False)
        logger = self.GetParent().logger
        logger.AppendText("Manual time range enabled\n")
        
    def autoParams(self, event):
        fig = self.GetParent().dataView
        fig.useAuto = True
        fig.ax.autoscale(True)
        logger = self.GetParent().logger
        logger.AppendText("Automatic time range enabled\n")
    
    def getCheckPeriod(self, event):
        self.checkPeriod = float(event.GetString())
    
    def updateCheckPeriod(self, event):
        timer = self.GetParent().timer
        timer.Stop()
        timer.Start(self.checkPeriod)
    
    def getSensorID(self, event):
        self.sensorID = event.GetString()
    
    def addSensor(self, event):
        parent = self.GetParent()
        newSensorPanel = sensorPanel(parent, self.sensorID)
        parent.sensors.append(newSensorPanel)
        parent.bottom_sizer.Add(newSensorPanel, 0, wx.EXPAND)
        parent.SetSizer(parent.sizer)
        parent.SetAutoLayout(1)
        parent.sizer.Fit(parent)
        parent.Layout()
    
    def removeSensor(self, event):
        parent = self.GetParent()
        sensors = parent.sensors
        for sensor in sensors:
            if sensor.sensor.id == self.sensorID:
                remSensorPanel = sensor
                parent.bottom_sizer.Remove(remSensorPanel)
                parent.sizer.Remove(remSensorPanel)
                parent.SetSizer(parent.sizer)
                parent.SetAutoLayout(1)
                parent.sizer.Fit(parent)
                remSensorPanel.Destroy()
                parent.Layout()
                sensors.remove(sensor)
                parent.logger.AppendText("Panel destroyed\n")
        
        

class dataGraph(wx.Panel):
    # Create plot area and axes
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        
        fig_width_cm = 25.0
        inches_per_cm = 1.0/2.54
        fig_width = fig_width_cm * inches_per_cm
        golden_mean = (np.sqrt(5)-1.0)/2.0
        fig_height = golden_mean * fig_width
        fig_size = (fig_width, fig_height)
        params = {'backend': 'ps',
                'axes.labelsize': 10,
                'text.fontsize': 10,
                'label.fontsize': 10,
                'label.rotation': 45,
                'legend.fontsize': 10,
                'xtick.labelsize': 10,
                'ytick.labelsize': 10,
                'text.usetex': False,
                'figure.subplot.left' : 0.01,
                'figure.subplot.bottom' : 0.01,
                'figure.subplot.right' : 0.99,
                'figure.subplot.top' : 0.99,
                'figure.subplot.wspace' : 0.00,
                'figure.subplot.hspace' : 0.00,
                'figure.figsize': fig_size}
        plt.rcParams.update(params)
        
        self.fig = plt.figure()
        self.canvas = FigureCanvasWxAgg(self, -1, self.fig)
        self.canvas.SetPosition((0,0))
        self.canvas.SetSize((640,320))
        self.ax = self.fig.add_axes([0.08,0.17,0.83,0.8]) # left, bottom, right, top
        self.rax = self.ax.twinx()
        
        # default values #
        self.useAuto = True
        self.x_min = 0
        self.x_max = 1
        self.y_min = 0
        self.y_max = 1
        self.y2_min = 0
        self.y2_max = 1
        
        self.setup_axes()
    
    def setup_axes(self):
        self.formatter = plt.ScalarFormatter(useOffset=False, useMathText=True)
        self.ax.yaxis.set_major_formatter(self.formatter)
        self.rax.yaxis.set_major_formatter(self.formatter)
        self.ax.minorticks_on()
        self.rax.minorticks_on()
        if not self.useAuto:
            self.ax.set_xlim([self.x_min, self.x_max])
            self.ax.set_ylim([self.y_min, self.y_max])
            self.rax.set_ylim([self.y2_min, self.y2_max])
        self.ax.set_xlabel("Time")
        self.ax.set_ylabel("Temperature ($^\circ$C)")
        self.rax.set_ylabel("Humidity (% RH)")
        labels = self.ax.get_xticklabels()
        for label in labels:
            label.set_rotation(30)
          

class sensorPanel(wx.Panel):
    '''Sensor panels for control of data logger sensors.'''
    # create panel of controls for sensor #
    def __init__(self, parent, sensor_id):
        wx.Panel.__init__(self, parent)
        self.sensor = sensor(sensor_id)
        
        self.isLogging = False
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.update, self.timer)
        
        self.line, = parent.dataView.ax.plot(self.sensor.timestamp, self.sensor.temperature, 'k-')
        
        self.quote = wx.StaticText(self, label=self.sensor.id, pos=(5,5))
        
        # timer period (ms) #
        self.timerPeriod = 5000
        
        # create set period control #
        self.period = 5
        self.periodStr = "5"
        self.setPeriod = wx.TextCtrl(self, value=self.periodStr, pos=(5,25), size=(100,20))
        self.Bind(wx.EVT_TEXT, self.updatePeriod, self.setPeriod)
        
        # Create start/stop button
        self.start_stop_button = wx.Button(self, label="Start Monitoring", pos=(5,50), size=(100,20))
        self.start_stop_button.SetFont(wx.Font(10, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False))
        self.start_stop_button.Bind(wx.EVT_BUTTON, self.onStartStopButton)
        
    def onStartStopButton(self, event):
        if not self.isLogging:
            self.start_stop_button.SetLabel("Running Up")
            self.sensor.start_monitoring(self.period)
            print "monitoring sensor 1"
            self.timer.Start(self.timerPeriod)
            self.isLogging = True
            self.start_stop_button.SetLabel("Stop Monitoring")
        else:
            self.timer.Stop()
            self.isLogging = False
            self.start_stop_button.SetLabel("Start Monitoring")
            self.sensor.stop_monitoring()
    
    def updatePeriod(self, event):
        logger = self.GetParent().logger
        self.periodStr = event.GetString()
        self.period = float(self.periodStr)
        self.sensor.stop_monitoring()
        self.sensor.start_monitoring(self.period)
        logger.AppendText("Sampling period of {0} set to {1:3.1f} s\n".format(self.sensor.id, self.period))        
                            
    def update(self, event):
        fig = self.GetParent().dataView
        logger = self.GetParent().logger
        if self.sensor.log_data():
            self.line.set_data(self.sensor.timestamp, self.sensor.temperature)
            self.line.axes.figure.canvas.draw()
            # Update plot
            fig.ax.clear()
            fig.ax.plot(self.sensor.timestamp, self.sensor.temperature, 'r-')
            #fig.rax.clear()
            fig.rax.plot(self.sensor.timestamp, self.sensor.humidity, 'b-')
            if not fig.useAuto:
                past = datetime.datetime.now() - datetime.timedelta(days = fig.deltaDays)
                fig.x_min = past
                fig.x_max = datetime.datetime.now()
                if fig.x_min > self.sensor.timestamp.min():
                    xi_min = np.where(self.sensor.timestamp >= fig.x_min)[0][0]
                    xi_max = np.where(self.sensor.timestamp <= fig.x_max)[0][-1]
                    tem_temp = self.sensor.temperature[xi_min:xi_max]
                    tem_hum = self.sensor.humidity[xi_min:xi_max]
                else:
                    tem_temp = self.sensor.temperature
                    tem_hum = self.sensor.humidity
                fig.y_min = tem_temp.min()
                fig.y_max = tem_temp.max()
                fig.y2_min = tem_hum.min()
                fig.y2_max = tem_hum.max()
            fig.setup_axes()
            fig.canvas.draw()
            logger.AppendText('{0}: {1}, {2:4.2f} C, {3:4.2f} % RH\n'.format(self.sensor.id,
                            self.sensor.timestamp[-1], self.sensor.temperature[-1],
                            self.sensor.humidity[-1]))                       
                                                                                                
app = wx.App(False)
frame = dataLogWindow(None, 'Data Logger')
app.MainLoop()