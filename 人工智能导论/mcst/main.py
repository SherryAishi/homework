# 导入随机包
import random
import numpy as np
from time import time 
from game import Game,Board
from copy import deepcopy
import datetime
class RandomPlayer:
    """
    随机玩家, 随机返回一个合法落子位置
    """

    def __init__(self, color):
        """
        玩家初始化
        :param color: 下棋方，'X' - 黑棋，'O' - 白棋
        """
        self.color = color


    def random_choice(self, board):
        """
        从合法落子位置中随机选一个落子位置
        :param board: 棋盘
        :return: 随机合法落子位置, e.g. 'A1'
        """
        # 用 list() 方法获取所有合法落子位置坐标列表
        action_list = list(board.get_legal_actions(self.color))

        # 如果 action_list 为空，则返回 None,否则从中选取一个随机元素，即合法的落子坐标
        if len(action_list) == 0:
            return None
        else:
            return random.choice(action_list)

    def get_move(self, board):
        """
        根据当前棋盘状态获取最佳落子位置
        :param board: 棋盘
        :return: action 最佳落子位置, e.g. 'A1'
        """
        if self.color == 'X':
            player_name = '黑棋'
        else:
            player_name = '白棋'
        #print("请等一会，对方 {}-{} 正在思考中...".format(player_name, self.color))
        action = self.random_choice(board)
        return action

class simulation_game:
    def __init__(self,black_player,white_player,board,current_player = None):
        self.board = board
        self.current_player = current_player
        self.black_player = black_player
        self.white_player = white_player
        self.black_player.color = "X"
        self.white_player.color = "O"
    def switch_player(self,black_player,white_player):
        if self.current_player == None:
            return black_player
        else:
            if self.current_player == black_player:
                return white_player
            else:
                return black_player
    
    def force_loss(self,is_time_out = False,is_board = False,is_legal = False):
        if self.current_player == self.black_player:
            winner = 1
        else:
            winner = 0
        
        diff = 0
        return winner,diff
    def run(self):
        total_time = {"X":0,"O":0}
        step_time = {"X":0,"O":0}
        winner = None
        diff = -1
        while 1:
            self.current_player = self.switch_player(self.black_player,self.white_player)
            start_time = datetime.datetime.now()
            color = "X" if self.current_player == self.black_player else "O"
            legal_actions = list(self.board.get_legal_actions(color))
            if len(legal_actions) == 0:
                if self.game_over():
                    winner,diff = self.board.get_winner()
                    break
                else:
                    continue
            action = self.current_player.get_move(self.board)
            if action is None:
                continue
            else:
                self.board._move(action,color)
                if self.game_over():
                    winner,diff = self.board.get_winner()
                    break
        return winner,diff
    def game_over(self):
        b_list = list(self.board.get_legal_actions('X'))
        w_list = list(self.board.get_legal_actions('O'))
        is_over = len(b_list) == 0 and len(w_list) == 0
        return is_over            

class Treenode():
    def __init__(self,parent,color):
        self.parent = parent
        self.color = color
        self.A=0
        #A的数值未获胜次数
        #B的数值为该节点被选择的此时
        self.B=0
        self.child=dict()
        
def oppo(color):
    if color=='X':
        return 'O'
    return 'X'       
    
def select(node,board):
    
    """
    若选择的节点为叶子节点,即没有子节点时,返回其自身作为选择情况
    若选择的节点不为叶子节点,则有：
    node.child为字典,keys值指的是可以选择的行动
    node.child[key]则为具体的子节点
    则字典中的键值加数据为：键值是具体的行动,如"D4",而数据为对应的节点
    若其子节点中存在B为零的节点,也就是说存在尚未扩展的子节点,这个时候选择将终止    
    """

    if len(node.child)==0:
        return node
    else:
        best_score=-1
        best_move=None
        for k in node.child.keys():
            if node.child[k].B==0:
                best_move=k
                break
            else:
                B=node.B
                a=node.child[k].A
                b=node.child[k].B
                
                score=a/b+np.sqrt(2*np.log(B)/b)
                if(score>best_score):
                    best_score=score
                    best_move=k
            #通过遍历操作后,获得了最初节点下一层的选择节点
            #经过最初节点的选择后,修改棋局情况,便于后续递归进行选择
        board._move(best_move, node.color)
        return select(node.child[best_move],board)
        
def expand(node,board):
    
    """扩展了选定节点的全部子节点"""
    
    for move in board.get_legal_actions(node.color):
        node.child[move]=Treenode(node,oppo(node.color))

def simulate(node,board):
    """模拟是在经过了选择和扩展后进行的,这里可以使用之前的随机player进行"""
    
    black_player=RandomPlayer("X")
    white_player=RandomPlayer("O")
    if node.color=="O":
        current_player=black_player
    else:
        current_player=white_player
    
    simu_game=simulation_game(black_player,white_player,board,current_player)
    return simu_game.run()

def back_ward(node,score):
    node.A+=score
    node.B+=1
    if node.parent!=None:
        back_ward(node.parent,1-score)



class AIPlayer:
    """
    AI 玩家
    """

    def __init__ (self,color):
        """
        玩家初始化
        :param color: 下棋方,'X' - 黑棋,'O' - 白棋
        """
        self.time=time()
        self.color = color

    def mcst(self,board):
        """   
        在mcst中,一方面要实现在60秒内进行尽可能多的选择扩展模拟反向传播,使得树
        的准确度增高,另一方面要进行最后一次选择,找到最优action
        """
        root=Treenode(None,self.color) 
        while time()-self.time<2:
            new_board=deepcopy(board)
            choice=select(root,new_board) #选择结束后会选择到目前最优节点
            expand(choice,new_board)
            winner,diff=simulate(choice,new_board)
            back=[1,0,0.5][winner]
            if choice.color=="X":
                back=1-back
            back_ward(choice,back)
        best_score=-1
        best_choice=None
        for k in root.child.keys():
            B=root.B
            a=root.child[k].A
            b=root.child[k].B
            score=a/b+np.sqrt(2*np.log(B)/b)
            if score > best_score:
                best_score = score
                best_choice = k
        return best_choice 
        
        
    def get_move(self, board):
        self.time=time()
        action=self.mcst(deepcopy(board))
        return action


if __name__ == "__main__":
    

    black_player =  RandomPlayer("X")
    white_player = AIPlayer("O")
    game = Game(black_player, white_player)
    game.run()
