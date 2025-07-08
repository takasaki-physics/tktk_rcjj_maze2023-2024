import sensor, image, time, os, ml, uos, gc, ustruct, pyb
from pyb import UART
from machine import Pin
led = pyb.LED(1)
uart = UART(3, 19200, timeout_char=200)
# LED(1)==red, 2==blue, 3==green
p0 = Pin('P0', Pin.OUT)
p0.value(0)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((120,0,140,140))
sensor.skip_frames(time=2000)

iro = 0
okuru = 0
net = None
labels = None
yellow_threshold   =  (41, 61, -3, 29, 42, 100)#　しきい値を追加してみました。
green_threshold   = (17, 31, -123, -18, -39, 36)
red_threshold   = (26, 54, 31, 44, 13, 41)
try:
    # Load the MLFlow model
    net = ml.Model("trained.tflite", load_to_fb=uos.stat('trained.tflite')[6] > (gc.mem_free() - (64*1024)))
except Exception as e:
    print(e)
    raise Exception('Failed to load "trained.tflite", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

try:
    labels = [line.rstrip('\n') for line in open("labels.txt")]
except Exception as e:
    raise Exception('Failed to load "labels.txt", did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

clock = time.clock()

while True:
    count = 0
    send = 0
    victim = 0
    clock.tick()
    img = sensor.snapshot()
    blobs = img.find_blobs([red_threshold])
    blobs2 = img.find_blobs([yellow_threshold]) #　追加してみました。
    blobs3 = img.find_blobs([green_threshold])
    if blobs and len(blobs) >= 5:
        send = 4
        if okuru == 0:
            x = int(send)
            try:
                uart.write(ustruct.pack('B', x))
                print(x)
            except OSError as err:
                pass
        for b in blobs:
            img.draw_rectangle(b[0:4], color=(255, 0, 0))
            img.draw_cross(b[5], b[6], color=(255, 0, 0))
            print("red")
            if okuru == 0:
                iro = 1
                for num in range(6):
                    led.on()
                    p0.value(1)
                    time.sleep(0.5)
                    led.off()
                    p0.value(0)
                    time.sleep(0.5)
            break
    #　以下、コピペした黄色の検出です。(文字色は黄色にしています。)
    if blobs2 and len(blobs2) >= 2:
        send = 5
        if okuru == 0:
            x = int(send)
            try:
                uart.write(ustruct.pack('B', x))
                print(x)
            except OSError as err:
                pass
        for b in blobs2:
            img.draw_rectangle(b[0:4], color=(255, 255, 0))
            img.draw_cross(b[5], b[6], color=(255, 255, 0))
            print("yellow")
            if okuru == 0:
                iro = 1
                for num in range(6):
                    led.on()
                    p0.value(1)
                    time.sleep(0.5)
                    led.off()
                    p0.value(0)
                    time.sleep(0.5)
            break
    if blobs3 and len(blobs3) >= 5:
        send = 6
        if okuru == 0:
            x = int(send)
            try:
                uart.write(ustruct.pack('B', x))
                print(x)
            except OSError as err:
                pass
        for b in blobs3:
            img.draw_rectangle(b[0:4], color=(0, 255, 0))
            img.draw_cross(b[5], b[6], color=(0, 255, 0))
            print("green")
            if okuru == 0:
                iro = 1
                for num in range(6):
                    led.on()
                    p0.value(1)
                    time.sleep(0.5)
                    led.off()
                    p0.value(0)
                    time.sleep(0.5)
            break

    predictions_list = list(zip(labels, net.predict([img])[0].flatten().tolist()))

    for label, confidence in predictions_list:
        print(f"{label} = {confidence}")
        if confidence >= 0.9:
            victim = label

    if victim == "H":
        send = 1
        if okuru == 0:
            x = int(send)
            try:
                uart.write(ustruct.pack('B', x))
                print(x)
            except OSError as err:
                pass
            for num in range(5):
                led.on()
                p0.value(1)
                time.sleep(0.5)
                led.off()
                p0.value(0)
                time.sleep(0.5)
    elif victim == "S":
        send = 2
        if okuru == 0:
            x = int(send)
            try:
                uart.write(ustruct.pack('B', x))
                print(x)
            except OSError as err:
                pass
            for num in range(6):
                led.on()
                p0.value(1)
                time.sleep(0.5)
                led.off()
                p0.value(0)
                time.sleep(0.5)
    elif victim == "U":
        send = 3
        if okuru == 0:
            x = int(send)
            try:
                uart.write(ustruct.pack('B', x))
                print(x)
            except OSError as err:
                pass
            for num in range(7):
                led.on()
                p0.value(1)
                time.sleep(0.5)
                led.off()
                p0.value(0)
                time.sleep(0.5)
    elif victim == 0:
        if iro == 0:
            send = 0

    print(send)
    if send != 0:
        okuru = 100

    if okuru > 0:
        okuru -= 1
        print(okuru)

    print(clock.fps(), "fps")
    uart = UART(3, 19200, timeout_char=200)
    time.sleep(0.05)
