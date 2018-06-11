# RGBD画像の表示

from open3d import *
import matplotlib.pyplot as plt
import numpy as np

if __name__ == "__main__":
    color_raw = read_image("../data/image00100.png")
    depth_raw = read_image("../data/depth00100.png")
    camera_intrinsic = read_pinhole_camera_intrinsic("../data/d435.json")
    print( camera_intrinsic )
    rgbd_image = create_rgbd_image_from_color_and_depth( color_raw, depth_raw )
    print(rgbd_image)
    plt.subplot(1, 2, 1)
    plt.title('Grayscale image')
    plt.imshow(rgbd_image.color)
    plt.subplot(1, 2, 2)
    plt.title('Depth image')
    plt.imshow(rgbd_image.depth)
    plt.show()
    pcd = create_point_cloud_from_rgbd_image(rgbd_image, camera_intrinsic)
    print(np.asarray(pcd.points))
    print("\n")
    draw_geometries([pcd])
    write_point_cloud( "out.ply", pcd )
