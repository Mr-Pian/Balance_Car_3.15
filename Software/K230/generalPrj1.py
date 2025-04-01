# TurtlePrj1 - By: 15171 - Sun Mar 16 2025
# 2025NUEDC  KZ19
import time, os, gc, sys, math
from machine import UART
from machine import FPIOA
from media.sensor import *
from media.display import *
from media.media import *


DISPLAY_WIDTH = 800#ALIGN_UP(800, 16)
DISPLAY_HEIGHT = 480

DETECT_WIDTH = 640
DETECT_HEIGHT = 480
LZ = 160
UZ = 100
LB = 80
RB = 400
UB = 300
DB = 400
#######################################################################
# Tracks a black line. Use [(128, 255)] for a tracking a white line.
GRAYSCALE_THRESHOLD = [(10, 90)]

# Each roi is (x, y, w, h). The line detection algorithm will try to find the
# centroid of the largest blob in each roi. The x position of the centroids
# will then be averaged with different weights where the most weight is assigned
# to the roi near the bottom of the image and less to the next roi and so on.
ROIS = [ # [ROI, weight]
       (LZ+LB,DB, RB-LB, DETECT_HEIGHT-DB, 0.7), #横向取样 下方1
       (LZ+LB, UB, RB-LB, DB-UB, 0.3), # 中间2
       (LZ+LB, UZ, RB-LB, UB-UZ, 0.1),   #上方3
       (LZ,UB,LB,DB-UB,0),#左
       (LZ+RB,UB,DETECT_WIDTH-RB,DB-UB,0)#右
       ]


# Compute the weight divisor (we're computing this so you don't have to make weights add to 1).
weight_sum = 0
for r  in  ROIS:
    weight_sum += r[-1]# r[-1] is the roi weight.
##################################################################################


fpioa = FPIOA()

# 将指定引脚配置为 UART 功能
fpioa.set_function(11, FPIOA.UART2_TXD)
fpioa.set_function(12, FPIOA.UART2_RXD)
uart = UART(UART.UART2, baudrate=115200, bits=UART.EIGHTBITS, parity=UART.PARITY_NONE, stop=UART.STOPBITS_ONE)

## 要发送的字符串
#message = "RAPUTA\n"

## 通过UART发送数据
#uart.write(message)

sensor = None

try:
    # construct a Sensor object with default configure
    sensor = Sensor(id=2, width = DETECT_WIDTH, height = DETECT_HEIGHT)
    # sensor reset
    sensor.reset()
    # set hmirror
    # sensor.set_hmirror(False)
    # sensor vflip
    # sensor.set_vflip(False)
    # set chn0 output size
    sensor.set_framesize(framesize=Sensor.VGA)
    # set chn0 output format
    sensor.set_pixformat(Sensor.GRAYSCALE)


    # use lcd as display output
    Display.init(Display.ST7701, width = DISPLAY_WIDTH,height = DISPLAY_HEIGHT,to_ide = True)

    # use IDE as output
    #Display.init(Display.VIRT, width = DETECT_WIDTH, height = DETECT_HEIGHT, fps = 100)

    # init media manager
    MediaManager.init()
    # sensor start run
    sensor.run()
    fps = time.clock()
    message = "K230-Initialized"
    uart.write(message)
    while True:
        fps.tick()
        # check if should exit.
        os.exitpoint()
        img = sensor.snapshot()
        ###########################################
        blob_check=[0,0,0,0,0]
        road_flag=0 #标志位  0-直线、曲线；1-左拐；2-右拐；3-十字路口；
        centroid_sum = 0
        for r in range(0,5):
            blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=ROIS[r][0:4], merge=True) # r[0:4] is roi tuple.
            if blobs:
                # Find the blob with the most pixels.
                largest_blob = max(blobs, key=lambda b: b.pixels())
                blob_check[r] = 1
                if(r<3) centroid_sum += largest_blob.cx() * r[-1]
                # Draw a rect around the blob.
                img.draw_rectangle([v for v in largest_blob.rect()])
                img.draw_cross(largest_blob.cx(), largest_blob.cy())
        print(blob_check)  #顺序：下中上左右
        if(blob_check[0]&&blob_check[1]):
            if(!blob_check[2]):     #前方断头
                if(blob_check[3]&&!blob_check[4]):      #左直角
                    print("left")
                    message = "left"
                elif(!blob_check[3]&&blob_check[4]):    #右直角
                    print("right")
                    message = "right"
                elif(blob_check[3]&&blob_check[4]):     #T形路口
                    print("T")
                    message = "T"
                else:
                    print("lost")
                    message =  "lost"
            else:                   #前方有色块
                if(blob_check[3]&&!blob_check[4]): #左锐角
                    print("left-T")
                    message = "left-T"
                elif(!blob_check[3]&&blob_check[4]): #右锐角
                    print("right-T")
                    message = "right-T"
                elif(blob_check[3]&&blob_check[4]):  #十字路口
                    print("CROSS")
                    message = "CROSS"
                else:                               #正常前进
                    center_pos = (centroid_sum / weight_sum) # Determine center of line.
                    print("Line:%f"%centor_pos)
                    message = "Line:%f"%centor_pos
        ############################################################
        #message = "%f"% deflection_angle
        uart.write(message)

#        img.draw_line(LZ,UB,640,UB,color=(0,0,255))
#        img.draw_line(LZ,DB,640,DB,color=(0,0,255))
#        img.draw_line(LZ+LB,0,LZ+LB,480,color=(0,0,255))
#        img.draw_line(LZ+RB,0,LZ+RB,480,color=(0,0,255))

        # draw result to screen
        img.draw_line(LZ,0,LZ,DETECT_HEIGHT,color=(0,0,0))
        Display.show_image(img)
        gc.collect()

        print(fps.fps())


except KeyboardInterrupt as e:
    print(f"user stop")
except BaseException as e:
    print(f"Exception '{e}'")
finally:
    # sensor stop run
    if isinstance(sensor, Sensor):
        sensor.stop()
    # deinit display
    Display.deinit()

    os.exitpoint(os.EXITPOINT_ENABLE_SLEEP)
    time.sleep_ms(100)

    # release media buffer
    MediaManager.deinit()
