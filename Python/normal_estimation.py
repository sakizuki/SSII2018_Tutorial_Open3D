# 法線推定 

import numpy as np
from open3d import *
import copy

if __name__ == "__main__":

    #データ読み込み
    pcd_ = read_point_cloud("../data/bun.pcd")
    print(":: 元の点群の可視化")
    draw_geometries([pcd_]) # まず，そのままの点群を表示する

    pcd = voxel_down_sample( pcd_, 0.002 )
    draw_geometries([pcd])

    radius = 0.02 #法線計算のための範囲指定
    viewpoint = np.array([0.,0.,100.], dtype='float64') # 法線向き修正のための視点位置


    #法線推定
    estimate_normals( pcd, KDTreeSearchParamHybrid(radius = radius, max_nn = 30) )

    pcd2 = copy.deepcopy(pcd) #法線向きの修正前後の点群の用意（比較用）
    np.asarray(pcd2.points)[:,0] += 0.2 # x軸方向に0.2移動する

    #向きの修正
    orient_normals_towards_camera_location( pcd2, camera_location = viewpoint )        

    print(":: 法線推定と向きの修正（nキーで法線を表示できます．）")
    draw_geometries([pcd, pcd2]) # 法線推定前後の点群を表示する
    
