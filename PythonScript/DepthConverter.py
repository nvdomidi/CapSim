# by navid

from PIL import Image
import numpy as np
import matplotlib.pyplot as plt

def img_to_depth(img):
    # get R channel (only R channel contains information)
    R = np.array(img[:,:,0]).astype(np.float32)

    # get G channel (only G channel contains information)
    G = np.array(img[:,:,1]).astype(np.float32)

    # get B channel (only B channel contains information)
    B = np.array(img[:,:,2]).astype(np.float32)

    normalized = (R + G * 256 + B * 256 * 256) / (256 * 256 * 256 - 1)

    return normalized

def depth_to_logarithmic_depth(depth):
    value = 1.0 + np.log(depth) / 5.70378
    clamped = np.clip(value, 0.005, 1.0)
    return clamped

def onclick(event):
    if event.xdata is not None and event.ydata is not None:
        x = int(event.xdata)
        y = int(event.ydata)
        rgb = logarithmic_depth[y, x]  # Assuming 'image' is a NumPy array representing the image
        print(f"Clicked coordinates: ({x}, {y})")
        print(f"RGB value at position ({x}, {y}): {rgb}")
    

# read depth image saved by unreal engine
img_name = 'capture1_depth.png'
img = Image.open(img_name)
img = np.asarray(img)

# 1. use normal depth
normalized_depth = img_to_depth(img)
grayscale = (normalized_depth*255).astype(np.uint8)
depth_image = Image.fromarray(grayscale)
depth_image.save('image_depth.png')

# 2. use logarithmic depth
logarithmic_depth = depth_to_logarithmic_depth(normalized_depth)
grayscale = (logarithmic_depth*255).astype(np.uint8)
depth_image = Image.fromarray(grayscale)
depth_image.save('image_depth_logarithmic.png')

# 3. display logarithmic depth (click to show depth value)
fig, ax = plt.subplots()
ax.imshow(logarithmic_depth)
cid = fig.canvas.mpl_connect('button_press_event', onclick)
plt.show()




