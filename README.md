# Open3Dを利用した3次元点群処理
## 概要
本レポジトリは，第24回画像センシングシンポジウム(SSII)2018の
チュートリアル講演「3D物体検出とロボットビジョンへの応用
-3D点群処理の基礎と位置姿勢推定のしくみ-」のために用意した資料です．
講演中で解説した点群処理アルゴリズムのコードを紹介します．

本レポジトリのコードを実行することによって，以下を試すことができます．
- Open3Dを用いた基本的な点群処理
- Realsenseセンサを使ったRGBD連続画像の取得
- 上記を用いた3Dシーンの再構成

講演で公開した資料は[こちら](https://www.slideshare.net/SSII_Slides/3d-101077557)で公開していますので，併せてご覧いただければ幸いです．

## ファイル構成

- [src ] ・・・RealsenseセンサによってRGBD画像を取得するプログラムが格納されています．
- [data] ・・・サンプルデータが格納されています．
- [Python]・・・上記の講演で使用したOpen3Dのコードが格納されています．
- CMakeLists.txt

## 準備
### 依存環境の構築
このレポジトリで紹介するすべてのプログラムの実行のためには，下記のライブラリのインストールが必要です．
1. [Open3D](https://github.com/IntelVCL/Open3D)  点群処理用
2. [librealsense](https://github.com/IntelRealSense/librealsense)  RealsenseセンサからのRGBD画像取得用

インストール方法はそれぞれのレポジトリで確認してください．

### プログラムのビルド
[src]内のデータ取得のためのプログラム(rs-capture)のみビルドが必要です．

```
mkdir build
cd build
cmake ../
make
```

## 各プログラムの説明
### rs-capture
buildディレクトリで，下記のコマンドを実行してください．
```
./rs-capture
```
[image]と[depth]というディレクトリが作成され，（すでにある場合は削除してから再度作成します．）その中にセンサで撮影されたRGB画像と距離画像を保存します．
ここで撮影したデータは，Open3Dが提供する[Reconstruction System](http://www.open3d.org/docs/tutorial/ReconstructionSystem/index.html)によって3次元復元することができます．

### kdtree.py
kd treeによる近傍点探索のコードサンプルです．
三種類の探索方法（RNN, KNN, RKNN）を試すことができます．

### keypoint_matching.py
キーポイントマッチングのコードサンプルです．

### normal_estimation.py
法線推定のコードサンプルです．法線向き修正の効果を確認するために修正前後のデータを同時に可視化します．

### NumPy_and_Open3D.ipynb
NumPyとOpen3Dのデータの相互変換の方法の説明です．

### pv.py
点群データ(.pcd, .ply)を可視化するためのビューワです．
```
python pv.pcd [可視化したいファイル名]
```
で実行できます．

### rgbd_and_pcd.py
RGB画像と距離画像を読み込んで，点群に変換するためのサンプルコードです．
このスクリプトでは距離画像がRGBカメラのカメラの座標系でレンダリングされていることを想定しています．（RGB画像を基準として距離画像とピクセル単位で位置合わせされている．）
したがって，それぞれのセンサのRGBカメラの内部パラメータを用いて点群に変換しています．
点群データは「out.ply」として保存します．