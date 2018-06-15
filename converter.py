#!/usr/bin/env python3

import numpy as np
import cv2

#  with open('str2/test.dat') as fin:
#      for idx, line in enumerate(fin):
#          line = line.strip()
#          if not line:
#              continue
#          values = line.split(',')
#          values = [int(i) for i in values]
#          class_value = np.argmax(values[-9:])
#
#          values = np.array(values[:-9])
#          values = values.reshape((48, 48))
#
#          idx = str(idx).zfill(3)
#          cv2.imwrite(f'str2/images/test_{idx}_{class_value}.png', values)

lines = []
for fname in ['str2/train.dat', 'str2/test.dat']:
    with open(fname) as train_file:
        for line in train_file:
            line = line.strip()
            if not line:
                continue
            lines.append(line)

print(len(lines))
print(len(set(lines)))
print(len(lines) - len(set(lines)), 'elementos repeditos')
