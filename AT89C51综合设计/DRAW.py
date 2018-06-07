import binascii
import serial
from time import sleep
import matplotlib.pyplot as plt
import matplotlib.animation as animation


#初始数据绘图
def update(frame):
    #读入模拟
    read_string = binascii.hexlify(ser.read()).decode('ascii')
    a = (int(read_string[0], 16) * 16 + int(read_string[1], 16))
    print(a)
    sleep(0.1)
    #绘图数据生成  
    del(data_read[0])
    data_read.append(a)
    #绘图 
    line.set_ydata(data_read)    
    #颜色设置
    if abs(a) >= 40:
        plt.setp(line, 'color', 'r', 'linewidth', 2.0)
    else:
        plt.setp(line, 'color', 'b', 'linewidth', 2.0)
    return line

if __name__ == '__main__':
    data_read = list(range(100))
    fig, ax = plt.subplots()
    line, = ax.plot(data_read)
    ax.set_ylim(0, 100)
    plt.grid(True)
    ax.set_ylabel("Temperature: ℃")
    ax.set_xlabel("Relative Time: s")

    ser = serial.Serial(port='COM3', baudrate=1200)
    ani = animation.FuncAnimation(fig, update, frames=None, interval=100)
    plt.show()
    ser.close()
