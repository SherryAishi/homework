# !/usr/bin/Anaconda3/python
# -*- coding: utf-8 -*-

from func_timeout import func_timeout, FunctionTimedOut
import datetime
from copy import deepcopy


#!/usr/bin/Anaconda3/python
# -*- coding: utf-8 -*-

class Board(object):
    """
    Board 黑白棋棋盘,规格是8*8,黑棋用 X 表示,白棋用 O 表示,未落子时用 . 表示。
    """

    def __init__(self):
        """
        初始化棋盘状态
        """
        self.empty = '.'  # 未落子状态
        self._board = [[self.empty for _ in range(8)] for _ in range(8)]  # 规格：8*8
        self._board[3][4] = 'X'  # 黑棋棋子
        self._board[4][3] = 'X'  # 黑棋棋子
        self._board[3][3], self._board[4][4] = 'O', 'O'  # 白棋棋子

    def __getitem__(self, index):
        """
        添加Board[][] 索引语法
        :param index: 下标索引
        :return:
        """
        return self._board[index]

    def display(self, step_time=None, total_time=None):
        """
        打印棋盘
        :param step_time: 每一步的耗时, 比如:{"X":1,"O":0},默认值是None
        :param total_time: 总耗时, 比如:{"X":1,"O":0},默认值是None
        :return:
        """
        board = self._board
        # print(step_time,total_time)
        # 打印列名
        print(' ', ' '.join(list('ABCDEFGH')))
        # 打印行名和棋盘
        for i in range(8):
            # print(board)
            print(str(i + 1), ' '.join(board[i]))
        if (not step_time) or (not total_time):
            # 棋盘初始化时展示的时间
            step_time = {"X": 0, "O": 0}
            total_time = {"X": 0, "O": 0}
            print("统计棋局: 棋子总数 / 每一步耗时 / 总时间 ")
            print("黑   棋: " + str(self.count('X')) + ' / ' + str(step_time['X']) + ' / ' + str(
                total_time['X']))
            print("白   棋: " + str(self.count('O')) + ' / ' + str(step_time['O']) + ' / ' + str(
                total_time['O']) + '\n')
        else:
            # 比赛时展示时间
            print("统计棋局: 棋子总数 / 每一步耗时 / 总时间 ")
            print("黑   棋: " + str(self.count('X')) + ' / ' + str(step_time['X']) + ' / ' + str(
                total_time['X']))
            print("白   棋: " + str(self.count('O')) + ' / ' + str(step_time['O']) + ' / ' + str(
                total_time['O']) + '\n')

    def count(self, color):
        """
        统计 color 一方棋子的数量。(O:白棋, X:黑棋, .:未落子状态)
        :param color: [O,X,.] 表示棋盘上不同的棋子
        :return: 返回 color 棋子在棋盘上的总数
        """
        count = 0
        for y in range(8):
            for x in range(8):
                if self._board[x][y] == color:
                    count += 1
        return count

    def get_winner(self):
        """
        判断黑棋和白旗的输赢,通过棋子的个数进行判断
        :return: 0-黑棋赢,1-白旗赢,2-表示平局,黑棋个数和白旗个数相等
        """
        # 定义黑白棋子初始的个数
        black_count, white_count = 0, 0
        for i in range(8):
            for j in range(8):
                # 统计黑棋棋子的个数
                if self._board[i][j] == 'X':
                    black_count += 1
                # 统计白旗棋子的个数
                if self._board[i][j] == 'O':
                    white_count += 1
        if black_count > white_count:
            # 黑棋胜
            return 0, black_count - white_count
        elif black_count < white_count:
            # 白棋胜
            return 1, white_count - black_count
        elif black_count == white_count:
            # 表示平局,黑棋个数和白旗个数相等
            return 2, 0

    def _move(self, action, color):
        """
        落子并获取反转棋子的坐标
        :param action: 落子的坐标 可以是 D3 也可以是(2,3)
        :param color: [O,X,.] 表示棋盘上不同的棋子
        :return: 返回反转棋子的坐标列表,落子失败则返回False
        """
        # 判断action 是不是字符串,如果是则转化为数字坐标
        if isinstance(action, str):
            action = self.board_num(action)

        fliped = self._can_fliped(action, color)

        if fliped:
            # 有就反转对方棋子坐标
            for flip in fliped:
                x, y = self.board_num(flip)
                self._board[x][y] = color

            # 落子坐标
            x, y = action
            # 更改棋盘上 action 坐标处的状态,修改之后该位置属于 color[X,O,.]等三状态
            self._board[x][y] = color
            return fliped
        else:
            # 没有反转子则落子失败
            return False

    def backpropagation(self, action, flipped_pos, color):
        """
        回溯
        :param action: 落子点的坐标
        :param flipped_pos: 反转棋子坐标列表
        :param color: 棋子的属性,[X,0,.]三种情况
        :return:
        """
        # 判断action 是不是字符串,如果是则转化为数字坐标
        if isinstance(action, str):
            action = self.board_num(action)

        self._board[action[0]][action[1]] = self.empty
        # 如果 color == 'X',则 op_color = 'O';否则 op_color = 'X'
        op_color = "O" if color == "X" else "X"

        for p in flipped_pos:
            # 判断action 是不是字符串,如果是则转化为数字坐标
            if isinstance(p, str):
                p = self.board_num(p)
            self._board[p[0]][p[1]] = op_color

    def is_on_board(self, x, y):
        """
        判断坐标是否出界
        :param x: row 行坐标
        :param y: col 列坐标
        :return: True or False
        """
        return x >= 0 and x <= 7 and y >= 0 and y <= 7

    def _can_fliped(self, action, color):
        """
        检测落子是否合法,如果不合法,返回 False,否则返回反转子的坐标列表
        :param action: 下子位置
        :param color: [X,0,.] 棋子状态
        :return: False or 反转对方棋子的坐标列表
        """
        # 判断action 是不是字符串,如果是则转化为数字坐标
        if isinstance(action, str):
            action = self.board_num(action)
        xstart, ystart = action

        # 如果该位置已经有棋子或者出界,返回 False
        if not self.is_on_board(xstart, ystart) or self._board[xstart][ystart] != self.empty:
            return False

        # 临时将color放到指定位置
        self._board[xstart][ystart] = color
        # 棋手
        op_color = "O" if color == "X" else "X"

        # 要被翻转的棋子
        flipped_pos = []
        flipped_pos_board = []

        for xdirection, ydirection in [[0, 1], [1, 1], [1, 0], [1, -1], [0, -1], [-1, -1], [-1, 0],
                                       [-1, 1]]:
            x, y = xstart, ystart
            x += xdirection
            y += ydirection
            # 如果(x,y)在棋盘上,而且为对方棋子,则在这个方向上继续前进,否则循环下一个角度。
            if self.is_on_board(x, y) and self._board[x][y] == op_color:
                x += xdirection
                y += ydirection
                # 进一步判断点(x,y)是否在棋盘上,如果不在棋盘上,继续循环下一个角度,如果在棋盘上,则进行while循环。
                if not self.is_on_board(x, y):
                    continue
                # 一直走到出界或不是对方棋子的位置
                while self._board[x][y] == op_color:
                    # 如果一直是对方的棋子,则点（x,y）一直循环,直至点（x,y)出界或者不是对方的棋子。
                    x += xdirection
                    y += ydirection
                    # 点(x,y)出界了和不是对方棋子
                    if not self.is_on_board(x, y):
                        break
                # 出界了,则没有棋子要翻转OXXXXX
                if not self.is_on_board(x, y):
                    continue

                # 是自己的棋子OXXXXXXO
                if self._board[x][y] == color:
                    while True:
                        x -= xdirection
                        y -= ydirection
                        # 回到了起点则结束
                        if x == xstart and y == ystart:
                            break
                        # 需要翻转的棋子
                        flipped_pos.append([x, y])

        # 将前面临时放上的棋子去掉,即还原棋盘
        self._board[xstart][ystart] = self.empty  # restore the empty space

        # 没有要被翻转的棋子,则走法非法。返回 False
        if len(flipped_pos) == 0:
            return False

        for fp in flipped_pos:
            flipped_pos_board.append(self.num_board(fp))
        # 走法正常,返回翻转棋子的棋盘坐标
        return flipped_pos_board

    def get_legal_actions(self, color):
        """
        按照黑白棋的规则获取棋子的合法走法
        :param color: 不同颜色的棋子,X-黑棋,O-白棋
        :return: 生成合法的落子坐标,用list()方法可以获取所有的合法坐标
        """
        # 表示棋盘坐标点的8个不同方向坐标,比如方向坐标[0][1]则表示坐标点的正上方。
        direction = [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]

        op_color = "O" if color == "X" else "X"
        # 统计 op_color 一方邻近的未落子状态的位置
        op_color_near_points = []

        board = self._board
        for i in range(8):
            # i 是行数,从0开始,j是列数,也是从0开始
            for j in range(8):
                # 判断棋盘[i][j]位子棋子的属性,如果是op_color,则继续进行下一步操作,
                # 否则继续循环获取下一个坐标棋子的属性
                if board[i][j] == op_color:
                    # dx,dy 分别表示[i][j]坐标在行、列方向上的步长,direction 表示方向坐标
                    for dx, dy in direction:
                        x, y = i + dx, j + dy
                        # 表示x、y坐标值在合理范围,棋盘坐标点board[x][y]为未落子状态,
                        # 而且（x,y）不在op_color_near_points 中,统计对方未落子状态位置的列表才可以添加该坐标点
                        if 0 <= x <= 7 and 0 <= y <= 7 and board[x][y] == self.empty and (
                                x, y) not in op_color_near_points:
                            op_color_near_points.append((x, y))
        l = [0, 1, 2, 3, 4, 5, 6, 7]
        for p in op_color_near_points:
            if self._can_fliped(p, color):
                # 判断p是不是数字坐标,如果是则返回棋盘坐标
                # p = self.board_num(p)
                if p[0] in l and p[1] in l:
                    p = self.num_board(p)
                yield p

    def board_num(self, action):
        """
        棋盘坐标转化为数字坐标
        :param action:棋盘坐标,比如A1
        :return:数字坐标,比如 A1 --->(0,0)
        """
        row, col = str(action[1]).upper(), str(action[0]).upper()
        if row in '12345678' and col in 'ABCDEFGH':
            # 坐标正确
            x, y = '12345678'.index(row), 'ABCDEFGH'.index(col)
            return x, y

    def num_board(self, action):
        """
        数字坐标转化为棋盘坐标
        :param action:数字坐标 ,比如(0,0)
        :return:棋盘坐标,比如 （0,0）---> A1
        """
        row, col = action
        l = [0, 1, 2, 3, 4, 5, 6, 7]
        if col in l and row in l:
            return chr(ord('A') + col) + str(row + 1)

class Game(object):
    def __init__(self, black_player, white_player):
        self.board = Board()  # 棋盘
        # 定义棋盘上当前下棋棋手,先默认是 None
        self.current_player = None
        self.black_player = black_player  # 黑棋一方
        self.white_player = white_player  # 白棋一方
        self.black_player.color = "X"
        self.white_player.color = "O"

    def switch_player(self, black_player, white_player):
        """
        游戏过程中切换玩家
        :param black_player: 黑棋
        :param white_player: 白棋
        :return: 当前玩家
        """
        # 如果当前玩家是 None 或者 白棋一方 white_player,则返回 黑棋一方 black_player;
        if self.current_player is None:
            return black_player
        else:
            # 如果当前玩家是黑棋一方 black_player 则返回 白棋一方 white_player
            if self.current_player == self.black_player:
                return white_player
            else:
                return black_player

    def print_winner(self, winner):
        """
        打印赢家
        :param winner: [0,1,2] 分别代表黑棋获胜、白棋获胜、平局3种可能。
        :return:
        """
        print(['黑棋获胜!', '白棋获胜!', '平局'][winner])

    def force_loss(self, is_timeout=False, is_board=False, is_legal=False):
        """
         落子3个不合符规则和超时则结束游戏,修改棋盘也是输
        :param is_timeout: 时间是否超时,默认不超时
        :param is_board: 是否修改棋盘
        :param is_legal: 落子是否合法
        :return: 赢家（0,1）,棋子差 0
        """

        if self.current_player == self.black_player:
            win_color = '白棋 - O'
            loss_color = '黑棋 - X'
            winner = 1
        else:
            win_color = '黑棋 - X'
            loss_color = '白棋 - O'
            winner = 0

        if is_timeout:
            print('\n{} 思考超过 60s, {} 胜'.format(loss_color, win_color))
        if is_legal:
            print('\n{} 落子 3 次不符合规则,故 {} 胜'.format(loss_color, win_color))
        if is_board:
            print('\n{} 擅自改动棋盘判输,故 {} 胜'.format(loss_color, win_color))

        diff = 0

        return winner, diff

    def run(self):
        """
        运行游戏
        :return:
        """
        # 定义统计双方下棋时间
        total_time = {"X": 0, "O": 0}
        # 定义双方每一步下棋时间
        step_time = {"X": 0, "O": 0}
        # 初始化胜负结果和棋子差
        winner = None
        diff = -1

        # 游戏开始
        print('\n=====开始游戏!=====\n')
        # 棋盘初始化
        self.board.display(step_time, total_time)
        while True:
            # 切换当前玩家,如果当前玩家是 None 或者白棋 white_player,则返回黑棋 black_player;
            #  否则返回 white_player。
            self.current_player = self.switch_player(self.black_player, self.white_player)
            start_time = datetime.datetime.now()
            # 当前玩家对棋盘进行思考后,得到落子位置
            # 判断当前下棋方
            color = "X" if self.current_player == self.black_player else "O"
            # 获取当前下棋方合法落子位置
            legal_actions = list(self.board.get_legal_actions(color))
            # print("%s合法落子坐标列表："%color,legal_actions)
            if len(legal_actions) == 0:
                # 判断游戏是否结束
                if self.game_over():
                    # 游戏结束,双方都没有合法位置
                    winner, diff = self.board.get_winner()  # 得到赢家 0,1,2
                    break
                else:
                    # 另一方有合法位置,切换下棋方
                    continue

            board = deepcopy(self.board._board)

            # legal_actions 不等于 0 则表示当前下棋方有合法落子位置
            try:
                for i in range(0, 3):
                    # 获取落子位置
                    action = func_timeout(60, self.current_player.get_move,
                                          kwargs={'board': self.board})

                    # 如果 action 是 Q 则说明人类想结束比赛
                    if action == "Q":
                        # 说明人类想结束游戏,即根据棋子个数定输赢。
                        break
                    if action not in legal_actions:
                        # 判断当前下棋方落子是否符合合法落子,如果不合法,则需要对方重新输入
                        print("你落子不符合规则,请重新落子！")
                        continue
                    else:
                        # 落子合法则直接 break
                        break
                else:
                    # 落子3次不合法,结束游戏！
                    winner, diff = self.force_loss(is_legal=True)
                    break
            except FunctionTimedOut:
                # 落子超时,结束游戏
                winner, diff = self.force_loss(is_timeout=True)
                break

            # 结束时间
            end_time = datetime.datetime.now()
            if board != self.board._board:
                # 修改棋盘,结束游戏！
                winner, diff = self.force_loss(is_board=True)
                break
            if action == "Q":
                # 说明人类想结束游戏,即根据棋子个数定输赢。
                winner, diff = self.board.get_winner()  # 得到赢家 0,1,2
                break

            if action is None:
                continue
            else:
                # 统计一步所用的时间
                es_time = (end_time - start_time).seconds
                if es_time > 60:
                    # 该步超过60秒则结束比赛。
                    print('\n{} 思考超过 60s'.format(self.current_player))
                    winner, diff = self.force_loss(is_timeout=True)
                    break

                # 当前玩家颜色,更新棋局
                self.board._move(action, color)
                # 统计每种棋子下棋所用总时间
                if self.current_player == self.black_player:
                    # 当前选手是黑棋一方
                    step_time["X"] = es_time
                    total_time["X"] += es_time
                else:
                    step_time["O"] = es_time
                    total_time["O"] += es_time
                # 显示当前棋盘
                self.board.display(step_time, total_time)

                # 判断游戏是否结束
                if self.game_over():
                    # 游戏结束
                    winner, diff = self.board.get_winner()  # 得到赢家 0,1,2
                    break

        print('\n=====游戏结束!=====\n')
        self.board.display(step_time, total_time)
        self.print_winner(winner)

        # 返回'black_win','white_win','draw',棋子数差
        if winner is not None and diff > -1:
            result = {0: 'black_win', 1: 'white_win', 2: 'draw'}[winner]

            # return result,diff

    def game_over(self):
        """
        判断游戏是否结束
        :return: True/False 游戏结束/游戏没有结束
        """

        # 根据当前棋盘,判断棋局是否终止
        # 如果当前选手没有合法下棋的位子,则切换选手；如果另外一个选手也没有合法的下棋位置,则比赛停止。
        b_list = list(self.board.get_legal_actions('X'))
        w_list = list(self.board.get_legal_actions('O'))

        is_over = len(b_list) == 0 and len(w_list) == 0  # 返回值 True/False

        return is_over
