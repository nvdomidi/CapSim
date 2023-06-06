# By Navid

import open3d as o3d
import numpy as np


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
        x = np.float32(lines[8+i].split()[0])
        y = - np.float32(lines[8+i].split()[1])
        z = np.float32(lines[8+i].split()[2])

        points[i,:] = np.array([x,y,z])

    return points, point_number


file_path = "27.ply"

points, point_number = read_ply_file(file_path)

point_list = o3d.geometry.PointCloud()
point_list.points = o3d.utility.Vector3dVector(points)
#point_list.colors = o3d.utility.Vector3dVector(int_color)

o3d.visualization.draw_geometries([point_list])
