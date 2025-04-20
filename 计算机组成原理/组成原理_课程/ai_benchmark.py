import numpy as np
import matplotlib.pyplot as plt

def create_multi_radar_chart(objects_list, attributes, values_list):
    """
    生成多个物体属性对比的雷达图(仅边框)
    
    参数:
    objects_list (list): 物体名称列表(5个)
    attributes (list): 五个属性名称的列表
    values_list (list): 包含5个子列表的列表，每个子列表是5个属性值(0-100)
    """
    
    # 检查输入数据
    if len(objects_list) != 5 or len(attributes) != 5 or len(values_list) != 5:
        raise ValueError("必须提供5个物体、5个属性和5组值(每组5个值)")
    
    # 准备角度数据
    angles = np.linspace(0, 2*np.pi, 5, endpoint=False).tolist()
    angles += angles[:1]  # 闭合图形
    
    # 创建图形
    fig, ax = plt.subplots(figsize=(10, 10), subplot_kw=dict(polar=True))
    
    # 颜色和线型列表
    colors = ['b', 'g', 'r', 'c', 'm']
    line_styles = ['-', '--', ':', '-.', (0, (3, 1, 1, 1))]
    
    # 绘制每个物体的雷达图(仅边框)
    for i in range(5):
        # 准备数据(闭合)
        values = values_list[i] + values_list[i][:1]
        
        # 仅绘制线条，不填充
        ax.plot(angles, values, color=colors[i], linewidth=2,
                linestyle=line_styles[i], label=objects_list[i])
    
    # 设置角度刻度
    ax.set_theta_offset(np.pi/2)
    ax.set_theta_direction(-1)
    ax.set_thetagrids(np.degrees(angles[:-1]), attributes)
    
    # 设置径向刻度
    ax.set_rlabel_position(0)
    plt.yticks([20, 40, 60, 80, 100], ["20", "40", "60", "80", "100"], 
               color="grey", size=8)
    plt.ylim(0, 100)
    
    # 添加标题和图例
    plt.title("five benchmark to five AI", size=16, y=1.1)
    plt.legend(loc='upper right', bbox_to_anchor=(1.3, 1.1))
    
    # 显示图形
    plt.show()

# 使用示例
if __name__ == "__main__":
    # 示例数据
    objects =  ["deepseek", "chatGPT", "kimi", "grok3", "doubao"]
    attributes = ["Verbal-reasoning", "Semantic-comprehension", "Anti-interference", "associative-ability", "Task-completion"] # 价格值越高表示性价比越好
    
    # 五个产品的五个属性值(0-100)
    values = [
        [69, 75, 88, 76, 68.67],  # 产品A
        [62, 85, 54, 48, 73],   # 产品B
        [72, 85, 86, 40, 40.33],   # 产品C
        [69, 70, 86, 44, 75],   # 产品D
        [93, 85, 94, 84, 54.33]    # 产品E
    ]
    
    create_multi_radar_chart(objects, attributes, values)