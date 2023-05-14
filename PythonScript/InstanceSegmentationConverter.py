# by navid

import cv2
import numpy as np

# read instance segmentation image saved by unreal engine
img = cv2.imread("23.png")

#print(img[:10,:10,:])

# convert to RGB
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB )



# set some predefined colors (add more if needed)
predefined_colors = [[255,255,0], [0,255,255], [255,0,255], [255,255,255],
                     [255,0,0], [0,255,0], [0,0,255], [0,0,0], [160,0,160],
                     [0,160,160],  [160,160,0],  [160,160,160], [160,0,0],
                     [0,160,0], [0,0,160], [80,80,80], [0,0,80], [0,80,0],
                     [80,0,0], [80,80,0]]

# get R channel (only R channel contains information)
rchannel = img[:,:,0]

# get G channel (only G channel contains information)
gchannel = img[:,:,1]

# get B channel (only B channel contains information)
bchannel = img[:,:,2]

print(np.unique(img[:,:,0].flatten()))
print(np.unique(img[:,:,1].flatten()))
print(np.unique(img[:,:,2].flatten()))

# flatten 2d-array and get unique values
rchannel_flatten = rchannel.flatten()
gchannel_flatten = gchannel.flatten()
bchannel_flatten = bchannel.flatten()

channels_stacked = np.vstack((gchannel_flatten, bchannel_flatten))

unique_ids = np.unique(channels_stacked, axis=1)

for i in range(0, unique_ids.shape[1]):

    print(i)
    
    where = (np.where((img[:,:,1] == unique_ids[0,i]) & (img[:,:,2] == unique_ids[1,i]) ))

    img[where[0], where[1], :] = predefined_colors[i]


channels_stacked = np.vstack((rchannel_flatten, gchannel_flatten, bchannel_flatten))
print(np.unique(channels_stacked, axis=1)/255)


cv2.imshow('img', img)

'''
# for every unique value set one of predefined colors
for i in range(0, len(rchannel_vals)):
    x_where = np.where(img[:,:,0] == rchannel_vals[i])[0]
    y_where = np.where(img[:,:,0] == rchannel_vals[i])[1]

    img[x_where, y_where, :] = predefined_colors[i]

# show image
cv2.imwrite('image.png', img)
'''
    
