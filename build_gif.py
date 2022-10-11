import cv2
import glob
import re

img_array = []
numbers = re.compile(r'(\d+)')
def numericalSort(value):
  parts = numbers.split(value)
  parts[1::2] = map(int, parts[1::2])
  return parts

size = (0, 0)
for filename in sorted(glob.glob('./images/*.png'), key=numericalSort):
    img = cv2.imread(filename)
    height, width, layers = img.shape
    size = (width, height)
    img_array.append(img)

out = cv2.VideoWriter('./render.mov', cv2.VideoWriter_fourcc(*'DIVX'), 40, size)
 
for i in range(len(img_array)):
    out.write(img_array[i])
out.release()