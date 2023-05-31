import open3d as o3d
import numpy as np

LABEL_COLORS = np.array([
    (255, 255, 255), # None
    (70, 70, 70),    # Roads
    (100, 40, 40),   # Car
    (55, 90, 80),    # Sky
    (220, 20, 60),   # Buildings
    (153, 153, 153), # Ground
    (157, 234, 50),  # Other1
    (128, 64, 128),  # Other2
    (244, 35, 232),  # Other3
    (107, 142, 35),  # Other4
    (0, 0, 142),     # Other5
    (102, 102, 156), # Other6
    (220, 220, 0),   # Other7
    (70, 130, 180),  # Other8
    (81, 0, 81),     # Other9
    (150, 100, 100), # Other10
    (230, 150, 140), # Other11
    (180, 165, 180), # Other12
    (250, 170, 30),  # Other13
    (110, 190, 160), # Other14
    (170, 120, 50),  # Other15
    (45, 60, 150),   # Other16
    (145, 170, 100), # Other17
]) / 255.0 # normalize each channel [0-1] since is what Open3D uses

def read_ply_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    lines = [line.strip() for line in lines]

    # get the number of points from the third line
    point_number = int(lines[2].split()[-1])

    # put the points into a [3,n] array, put labels into a list
    points = np.zeros((point_number, 3), dtype=np.float32)
    labels = []



    # get the point data and put them into points array
    # get the labels and append them to labels list
    # y should become -y because the coordinate system is different
    for i in range(0, point_number):
        x = np.float32(lines[10+i].split()[0])
        y = - np.float32(lines[10+i].split()[1])
        z = np.float32(lines[10+i].split()[2])

        points[i,:] = np.array([x,y,z])

        labels.append(int(lines[10+i].split()[-1]))

    return points, labels, point_number


file_path = "23.ply"

points, labels, point_number = read_ply_file(file_path)

# get color for each label
int_color = LABEL_COLORS[labels]

point_list = o3d.geometry.PointCloud()
point_list.points = o3d.utility.Vector3dVector(points)
point_list.colors = o3d.utility.Vector3dVector(int_color)

o3d.visualization.draw_geometries([point_list])
