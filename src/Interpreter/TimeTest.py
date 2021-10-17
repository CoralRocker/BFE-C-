import time
from subprocess import call, DEVNULL
import sys
import struct
human = open(sys.argv[1]+".txt", "w");
bot = open(sys.argv[1]+".dat","wb+");



numRuns = 25
sumTime = 0

human.write("Running {numRuns} times\n")
for i in range(0, numRuns):
    human.write(f"Run {i+1}/{numRuns}\n")
    s_t = time.time()
    call(["./bfi SpeedTest/speed.b"], shell=True)
    e_t = time.time()
    bot.write(struct.pack('d', e_t-s_t))
    sumTime += (e_t - s_t)
human.write(f"Took an average of {sumTime/numRuns} seconds\n")
