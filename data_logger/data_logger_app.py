import wx
import matplotlib
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg
import matplotlib.pyplot as plt
from matplotlib import rc
from class_sensor import *
from math import sqrt

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
        
        # add interactive graph window of data #
        dataView = dataGraph(self)
        # add panel of controls for each sensor #
        sensorPanel1 = sensorPanel(self, dataView)
        
        # set sizing #
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(dataView, 1, wx.EXPAND)
        self.sizer.Add(sensorPanel1, 0, wx.EXPAND)
        self.SetSizer(self.sizer)
        self.SetAutoLayout(1)
        self.sizer.Fit(self)
        self.Show()
        
    def OnAbout(self, event):
        # A message dialog box with an OK button. wx.OK is a standard ID in wxWidgets.
        desc = "Data Logger v0.0\nA.Sanders\nalansanders89@gmail.com"
        dlg = wx.MessageDialog(self, desc, "Data Logger", wx.OK)
        dlg.ShowModal() # show dialog
        dlg.Destroy() # destroy it when finished.
        
    def OnExit(self, event):
        self.Close(True)  # Close the frame.

class dataGraph(wx.Panel):
    # Create plot area and axes
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        
        fig_width_cm = 25.0
        inches_per_cm = 1.0/2.54
        fig_width = fig_width_cm * inches_per_cm
        golden_mean = (sqrt(5)-1.0)/2.0
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
        self.ax = self.fig.add_axes([0.07,0.14,0.9,0.8]) # left, bottom, right, top
        self.setup_axes()
    
    def setup_axes(self):
        self.ax.autoscale(True)
        self.formatter = plt.ScalarFormatter(useOffset=False, useMathText=True)
        self.ax.yaxis.set_major_formatter(self.formatter)
        self.ax.minorticks_on()
        self.ax.set_xlabel("Time")
        self.ax.set_ylabel("Temperature")
        labels = self.ax.get_xticklabels()
        for label in labels:
            label.set_rotation(30)
          

class sensorPanel(wx.Panel):
    '''Sensor panels for control of data logger sensors.'''
    # create panel of controls for sensor #
    def __init__(self, parent, fig):
        self.sensor = sensor('arduino')
        wx.Panel.__init__(self, parent)
        
        self.isLogging = False
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.update, self.timer)
        self.fig = fig
        #self.line, = self.fig.ax.plot(self.sensor.timestamp, self.sensor.temperature, 'k-')
        self.quote = wx.StaticText(self, label="Sensor 1", pos=(5,5))
        
        # Create start/stop button
        self.start_stop_button = wx.Button(self, label="Start Monitoring", pos=(5,20), size=(100,20))
        self.start_stop_button.SetFont(wx.Font(10, wx.DEFAULT, wx.NORMAL, wx.NORMAL, False))
        self.start_stop_button.Bind(wx.EVT_BUTTON, self.onStartStopButton)
        
    def onStartStopButton(self, event):
        if not self.isLogging:
            period = 5
            self.start_stop_button.SetLabel("Running Up")
            self.sensor.start_monitoring(period)
            print "monitoring sensor 1"
            
            secs = 1000
            self.timer.Start(secs)
            self.isLogging = True
            self.start_stop_button.SetLabel("Stop Monitoring")
        else:
            self.timer.Stop()
            self.isLogging = False
            self.start_stop_button.SetLabel("Start Monitoring")
            self.sensor.stop_monitoring()
            
    def update(self, event):
        self.sensor.log_data()
        #self.line.set_data(self.sensor.timestamp, self.sensor.temperature)
        #self.line.axes.figure.canvas.draw()
        # Update plot
        self.fig.ax.clear()
        self.fig.ax.plot(self.sensor.timestamp, self.sensor.temperature, 'k-')
        self.fig.setup_axes()
        self.fig.canvas.draw()                        
                                                                                                
app = wx.App(False)
frame = dataLogWindow(None, 'Data Logger')
app.MainLoop()