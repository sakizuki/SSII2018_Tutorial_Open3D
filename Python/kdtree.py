# kd-treeによる近傍点探索

import numpy as np
from open3d import *

if __name__ == "__main__":

    # 点群の読み込み
    pcd = read_point_cloud("../data/bun.pcd")

    pcd.paint_uniform_color([0.5, 0.5, 0.5]) #元の点群をグレーにする
    pcd_tree = KDTreeFlann(pcd)

    # クエリの点を赤にする
    query_id = 20773
    pcd.colors[query_id] = [1, 0, 0]

    # 以下は用意された3種類のサーチ方法の例
    # 1.RNNの方法
    [k, idx, dist] = pcd_tree.search_radius_vector_3d(pcd.points[query_id], 0.01)
    # 検出された点を緑にする．
    np.asarray(pcd.colors)[idx[1:], :] = [0, 1, 0]

    # 2.KNNの方法
    [k, idx, dist] = pcd_tree.search_knn_vector_3d(pcd.points[query_id], 100)
    np.asarray(pcd.colors)[idx[1:], :] = [0, 0, 1]

    # 3.RKNNの方法
    [k, idx, dist] = pcd_tree.search_hybrid_vector_3d(pcd.points[query_id], radius=0.01, max_nn=100 )
    np.asarray(pcd.colors)[idx[1:], :] = [0, 1, 1]

    
    # サーチ結果の解説
    # k   : 近傍点数
    # idx : 見つかった点群のid
    # dist: 見つかった点までの二乗距離
    print('近傍点数 k=', k)
    id = 50
    d = np.asarray(pcd.points)[query_id] - np.asarray(pcd.points)[idx[50]]
    norm = np.linalg.norm(d)
    print('クエリと',id, '番目に近い点の二乗距離',norm*norm)
    print('dist[',id, '] =', dist[id])

    print('点群の可視化')
    draw_geometries([pcd])
