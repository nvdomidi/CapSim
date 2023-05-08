# by navid

import cv2
import numpy as np

# read semantic segmentation image saved by unreal engine
img = cv2.imread("321.png")

# convert to RGB
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB )

# set some predefined colors (add more if needed)
predefined_colors = [[255,255,0], [0,255,255], [255,0,255], [255,255,255],
                     [255,0,0], [0,255,0], [0,0,255]]

# get R channel (only R channel contains information)
rchannel = img[:,:,0]

# flatten 2d-array and get unique values
rchannel_flatten = rchannel.flatten()
rchannel_vals = np.unique(rchannel_flatten)

# for every unique value set one of predefined colors
for i in range(0, len(rchannel_vals)):
    x_where = np.where(img[:,:,0] == rchannel_vals[i])[0]
    y_where = np.where(img[:,:,0] == rchannel_vals[i])[1]

    img[x_where, y_where, :] = predefined_colors[i]

# show image
cv2.imwrite('image.png', img)

    
