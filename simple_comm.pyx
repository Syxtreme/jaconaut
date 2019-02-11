# import numpy as np
# cimport numpy as np
from libcpp cimport bool

cdef extern from "kinova_api.h":
    struct CartesianDictionary:
        float X
        bool xset
        float Y
        bool yset
        float Z
        bool zset
        float ThetaX
        bool thetaxset
        float ThetaY
        bool thetayset
        float ThetaZ
        bool thetazset

    cdef cppclass Jaco2:
        Jaco2(int display_error_level)
        # main functions
        void Connect()
        void Disconnect()
        void ResetInstr()
        void SetFrameType(int type)

        void InitFingers()

        void Home()
        CartesianInfo GetPosition()
        AngularInfo GetAngularPosition()

        int Move(CartesianDictionary position, int maxWaitTime)
        void MoveToPos(CartesianInfo position)

        void PrintQuickStatus()
        void PrintGeneralInfo()

cdef extern from "KinovaTypes.h":
    struct CartesianInfo:
        float X
        float Y
        float Z
        float ThetaX
        float ThetaY
        float ThetaZ

    struct AngularInfo:
        float Actuator1
        float Actuator2
        float Actuator3
        float Actuator4
        float Actuator5
        float Actuator6
        float Actuator7

cdef class pyCartesianDictionary():
    cdef CartesianDictionary cartDict

    def __cinit__(self, *args):
        # self.cartDict = new CartesianDictionary()
        pass

    def reset(self):
        del self.X
        del self.Y
        del self.Z
        del self.ThetaX
        del self.ThetaY
        del self.ThetaZ

    @property
    def cartesianDictionary(self):
        return self.cartDict
        
    @property
    def X(self):
        return self.cartDict.X

    @X.setter
    def X(self, value):
        self.cartDict.X = value
        self.cartDict.xset = True

    @X.deleter
    def X(self):
        self.cartDict.xset = False

    @property
    def Y(self):
        return self.cartDict.Y

    @Y.setter
    def Y(self, value):
        self.cartDict.Y = value
        self.cartDict.yset = True

    @Y.deleter
    def Y(self):
        self.cartDict.yset = False

    @property
    def Z(self):
        return self.cartDict.Z

    @Z.setter
    def Z(self, value):
        self.cartDict.Z = value
        self.cartDict.zset = True

    @Z.deleter
    def Z(self):
        self.cartDict.zset = False

    @property
    def ThetaX(self):
        return self.cartDict.ThetaX

    @ThetaX.setter
    def ThetaX(self, value):
        self.cartDict.ThetaX = value
        self.cartDict.thetaxset = True

    @ThetaX.deleter
    def ThetaX(self):
        self.cartDict.thetaxset = False

    @property
    def ThetaY(self):
        return self.cartDict.ThetaY

    @ThetaY.setter
    def ThetaY(self, value):
        self.cartDict.ThetaY = value
        self.cartDict.thetayset = True

    @ThetaY.deleter
    def ThetaY(self):
        self.cartDict.thetayset = False

    @property
    def ThetaZ(self):
        return self.cartDict.ThetaZ

    @ThetaZ.setter
    def ThetaZ(self, value):
        self.cartDict.ThetaZ = value
        self.cartDict.thetazset = True

    @ThetaZ.deleter
    def ThetaZ(self):
        self.cartDict.thetazset = False

    def __setitem__(self, item, value):
        setattr(self, item, value)

    def __getitem__(self, item):
        getattr(self, item)


cdef class pyJaco2:
    cdef Jaco2* thisptr # hold a C++ instance

    def __cinit__(self, display_error_level = 0):
        self.thisptr = new Jaco2(display_error_level)

    def __dealloc__(self):
        del self.thisptr

    def Connect(self):
        self.thisptr.Connect()

    def ResetInstr(self):
        self.thisptr.ResetInstr()
    
    def SetFrameType(self, ftype):
        self.thisptr.SetFrameType(ftype)

    def GetAngularPosition(self):
        return self.thisptr.GetAngularPosition()

    def GetPosition(self):
        return self.thisptr.GetPosition()

    def Move(self, *args, maxWaitTime = 10, **kwargs):
        cpdef pyCartesianDictionary pos
        pos = pyCartesianDictionary()
        pos.reset()
        tempdict = {}
        for i, coord in enumerate(args):
            if i == 0:
                tempdict["X"] = coord
            elif i == 1:
                tempdict["Y"] = coord
            elif i == 2:
                tempdict["Z"] = coord
            elif i == 3:
                tempdict["ThetaX"] = coord
            elif i == 4:
                tempdict["ThetaY"] = coord
            elif i == 5:
                tempdict["ThetaZ"] = coord

        kwargs.update(tempdict)

        for k, v in kwargs.items():
            pos[k] = v
        
        # print(pos.cartesianDictionary)
        return self.thisptr.Move(pos.cartesianDictionary, maxWaitTime)

    def InitFingers(self):
        self.thisptr.InitFingers()

    def Home(self):
        self.thisptr.Home()

    def MoveToPos(self, position):
        cdef CartesianInfo pos
        pos.X = position[0]
        pos.Y = position[1]
        pos.Z = position[2]
        pos.ThetaX = position[3]
        pos.ThetaY = position[4]
        pos.ThetaZ = position[5]
        self.thisptr.MoveToPos(pos)

    def Disconnect(self):
        self.thisptr.Disconnect()

    def PrintQuickStatus(self):
        self.thisptr.PrintQuickStatus()

    def PrintGeneralInfo(self):
        self.thisptr.PrintGeneralInfo()