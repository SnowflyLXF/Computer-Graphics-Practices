import subprocess
import os
import wx
wildcard = "Python source (*.py; *.pyc)|*.py;*.pyc|" \
         "All files (*.*)|*.*"

class Example(wx.Frame):

    def __init__(self, parent, title):
        super(Example, self).__init__(parent, title=title,
            size=(700, 600))

        self.InitUI()
        self.Centre()

    def InitUI(self):

        
        self.panel = wx.Panel(self)
        sizer = wx.GridBagSizer(4, 4)

        text = wx.StaticText(self.panel, label="Choose scn file")
        sizer.Add(text, pos=(0, 0), flag=wx.TOP|wx.LEFT|wx.BOTTOM, border=5)

        btn = wx.Button(self.panel, label="Open File Dialog")
        btn.Bind(wx.EVT_BUTTON, self.onOpenFile)
 
        sizer.Add(btn, pos=(1, 0), span=(1, 5),flag=wx.RIGHT, border=5)
        buttonClose = wx.Button(self.panel, wx.ID_EXIT, size=(90, 28))
        sizer.Add(buttonClose, pos=(3, 4), flag=wx.RIGHT|wx.BOTTOM, border=10)
        self.Bind(wx.EVT_BUTTON, self.OnQuitApp, id=wx.ID_EXIT)
         # tc.Bind(wx.EVT_TEXT,self.OnKeyTyped) 
        sizer.AddGrowableCol(1)
        sizer.AddGrowableRow(2)
        self.panel.SetSizer(sizer)
        self.LoadImages()

        self.pic.SetPosition((20, 50))

    def OnQuitApp(self, event):
        self.Close()

    def OnKeyTyped(self, event): 
        print(event.GetString()) 

    def onOpenFile(self, event):
        """
        Create and show the Open FileDialog
        """
        dlg = wx.FileDialog(
            self, message="Choose a file",
            defaultFile="",
            wildcard=wildcard,
            style=wx.FD_OPEN | wx.FD_CHANGE_DIR
            )
        if dlg.ShowModal() == wx.ID_OK:
            paths = dlg.GetPaths()
            for path in paths:
                print(path)
        subprocess.call("./build/raytracer " + path, shell=True)
        self.LoadImages()
        self.pic.SetPosition((20, 50))
        self.Refresh()
        dlg.Destroy()



    def LoadImages(self):

        self.pic = wx.StaticBitmap(self.panel, wx.ID_ANY,
            wx.Bitmap("output/test.bmp", wx.BITMAP_TYPE_ANY))


def main():

    app = wx.App()
    ex = Example(None, title='Ray Tracing rendering')
    ex.Show()
    
    app.MainLoop()


if __name__ == '__main__':
    main()

