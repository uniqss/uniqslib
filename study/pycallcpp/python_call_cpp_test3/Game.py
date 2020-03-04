import KBEngine
import random
import math
from KBEDebug import *
from interfaces.GameTable import GameTable
from GlobalConst import *
from GlobalDefine import *
from game.Happy import *
from PlayerData import *
# from game.GameLogic import *
import operator
from random import choice
from functools import cmp_to_key
import ctypes

class tagOutCardResult_py(ctypes.Structure):
    _fields_ = [("cbCardCount", ctypes.c_ubyte), \
("cbResultCard1", ctypes.c_ubyte), \
("cbResultCard2", ctypes.c_ubyte), \
("cbResultCard3", ctypes.c_ubyte), \
("cbResultCard4", ctypes.c_ubyte), \
("cbResultCard5", ctypes.c_ubyte), \
("cbResultCard6", ctypes.c_ubyte), \
("cbResultCard7", ctypes.c_ubyte), \
("cbResultCard8", ctypes.c_ubyte), \
("cbResultCard9", ctypes.c_ubyte), \
("cbResultCard10", ctypes.c_ubyte), \
("cbResultCard11", ctypes.c_ubyte), \
("cbResultCard12", ctypes.c_ubyte), \
("cbResultCard13", ctypes.c_ubyte), \
("cbResultCard14", ctypes.c_ubyte), \
("cbResultCard15", ctypes.c_ubyte), \
("cbResultCard16", ctypes.c_ubyte), \
("cbResultCard17", ctypes.c_ubyte), \
("cbResultCard18", ctypes.c_ubyte), \
("cbResultCard19", ctypes.c_ubyte), \
("cbResultCard20", ctypes.c_ubyte)]

class Game(KBEngine.Base, GameTable):
    # 普通斗地主
    Happy = 1

    def __init__(self):
        KBEngine.Base.__init__(self)
        GameTable.__init__(self)

    def init_data(self, lobby_id, field_id, room_id, table_id, max_chair, field_config, room_config):
        GameTable.init_data(self, lobby_id, field_id, room_id, table_id, max_chair, field_config, room_config)

        # 添加定时器
        self.addGameTimer(0, 1, 1)
        # 游戏牌型逻辑
        # self.gameLogic = GameLogic()

        self.game_instance = None
        # if self.context['room_type'] == Game.Happy:
        #     self.game_instance = Happy()
        import ctypes
        so = ctypes.cdll.LoadLibrary
        lib = so("./libpycallclass.so")
        ERROR_MSG('display(\)')
        ret = tagOutCardResult_py(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        ERROR_MSG("before lib.display(ctypes.byref(ret))")
        lib.display(ctypes.byref(ret))
        ERROR_MSG("after lib.display(ctypes.byref(ret))")
        ERROR_MSG('#######################################################################################')
        ERROR_MSG(ret)
        ERROR_MSG(ret.cbCardCount)
        ERROR_MSG(ret.cbResultCard1)
        ERROR_MSG(ret.cbResultCard2)
        ERROR_MSG(ret.cbResultCard3)
        ERROR_MSG(type(ret))
        self.game_instance = Happy()
        self.game_instance.recvGameInstance(self)

    def resetData(self):
        """"""

    # 定时器，超时处理
    def onTimer(self, id, flag):
        """
        当时间到达则该接口被调用
        @param id		:  返回值定时器ID 用于删除定时器用
        @param flag	:  最后一个参数所给入的数据 必须为int
        """
        GameTable.onTimer(self, id, flag)
        self.game_instance.onTimer(id, flag)

    def handleReqYves(self, context):
        interface_name = context['interface_name']
        DEBUG_MSG('###### GAMELAND: 处理请求:  %s  !' % interface_name)
        if interface_name == 'reqKent_ready':
            self.game_instance.reqKent_ready(context)
        if interface_name == 'reqKent_callbanker':
            self.game_instance.reqKent_callbanker(context)
        if interface_name == 'reqKent_outCard':
            self.game_instance.reqKent_outCard(context)
        if interface_name == 'reqKent_SoundInfo':
            self.game_instance.reqKent_SoundInfo(context)
        if interface_name == 'reqKent_Trusteeship':
            self.game_instance.reqKent_Trusteeship(context)
    # 玩家进入桌子时调用
    def onPlayerEnterGameTable(self, player):
        DEBUG_MSG('@@@ # table functions # @@@ : onPlayerEnterGameTable ====================')
        self.game_instance.onPlayerEnterGameTable(player)

    # 玩家离开桌子时调用
    def onLeaveTable(self, player, leave_type):
        # ERROR_MSG('[%s] leave table' % (player.getNickName()))
        self.game_instance.onLeaveTable(player, leave_type)

    # 游戏开始时调用
    def onEventGameStart(self):
        DEBUG_MSG('@@@ # table functions # @@@ : onEventGameStart ====================')
        self.game_instance.game_start()

    # 玩家掉线时调用
    def onEventPlayerOffline(self, chair_id):
        ERROR_MSG('玩家掉线 ------------------>>>>  ')
        DEBUG_MSG('@@@ GAMELAND @@@ : onEventPlayerOffline ====================')
        self.game_instance.onEventPlayerOffline(chair_id)

    # 掉线玩家进入桌子
    def onEnterTable(self, context):
        # DEBUG_MSG('@@@ GameGf @@@ : onEnterTable ====================')
        player = context['player']
        self.game_instance.onEnterTable(context)

    def on_table_status_change(self, context):
        # 2 = 等待
        pass

    def onDestroy(self):
        ERROR_MSG('Game 销毁------释放引用------------>>>> ')
        GameTable.onDestroy(self)
        self.game_instance = None

    def on_task_get(self, user_id, task):
        ERROR_MSG('Game on_task_get------------user_id = %s task_list = %s' % (user_id, task))

    def on_task_update(self, user_id, task_id, progress):
        ERROR_MSG('Game on_task_update ------------user_id = %s task_list = %s' % (user_id, task_id))

    def on_task_finish(self, user_id, task_id):
        ERROR_MSG('Game on_task_finish ------------user_id = %s task_list = %s' % (user_id, task_id))
