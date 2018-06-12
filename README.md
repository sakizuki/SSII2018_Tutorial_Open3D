# Open3Dを利用した3次元点群処理
## 概要
本レポジトリは，第24回画像センシングシンポジウム(SSII2018)の
チュートリアル講演「3D物体検出とロボットビジョンへの応用
-3D点群処理の基礎と位置姿勢推定のしくみ-」のために用意した資料です．
講演中で解説した点群処理アルゴリズムのコードを紹介します．

本レポジトリのコードを実行することによって，以下を試すことができます．
- Open3Dを用いた基本的な点群処理
- Realsenseセンサを使ったRGBD連続画像の取得
- 上記を用いた3Dシーンの再構成

講演資料は[こちら](https://www.slideshare.net/SSII_Slides/3d-101077557)で公開していますので，併せてご覧いただければ幸いです．

※本レポジトリはUbuntu16.04での動作確認をおこなっています．

## ファイル構成

- [src] ・・・RealsenseセンサによってRGBD画像を取得するプログラムが格納されています．
- [data] ・・・サンプルデータが格納されています．
- [Python]・・・上記の講演で使用したOpen3Dのコードが格納されています．
- CMakeLists.txt

## 準備
### 依存環境の構築
このレポジトリで紹介するすべてのプログラムの実行のためには，下記のライブラリのインストールが必要です．
1. [Open3D](https://github.com/IntelVCL/Open3D)  点群処理用
2. [librealsense](https://github.com/IntelRealSense/librealsense)  RealSenseセンサからのRGBD画像取得用

インストール方法はそれぞれのレポジトリで確認してください．

### プログラムのビルド
ディレクトリ「src」内のデータ取得のためのプログラム(rs-capture)のみビルドが必要です．

```
mkdir build
cd build
cmake ../
make
```

## 各プログラムの説明
### rs-capture
RealSense SR300, D415, D435でRGBD画像群を取得するコードサンプルです． 
実行時は「build」ディレクトリにて下記のコマンドを実行してください．
```
cd <path_to_SSII2018_Tutorial_Open3D>/build/
./rs-capture
```
「image」と「depth」というディレクトリが作成され，その中にセンサで撮影されたRGB画像と距離画像を保存します．
ここで撮影したデータは，Open3Dが提供する[Reconstruction System](http://www.open3d.org/docs/tutorial/ReconstructionSystem/index.html)によって3次元復元することができます．


### kdtree.py
kd treeによる近傍点探索のコードサンプルです．
三種類の探索方法（RNN, KNN, RKNN）を試すことができます．

### keypoint_matching.py
キーポイントマッチングによる位置姿勢推定のためのコードサンプルです．

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
このスクリプトでは距離画像がRGBカメラのカメラの座標系でレンダリングされていることを想定しています．（RGB画像を基準として距離画像とピクセル単位で位置合わせ）
したがって，それぞれのセンサのRGBカメラの内部パラメータを用いて点群に変換しています．
点群データは「out.ply」として保存します．


## RealSenseセンサで取得したRGBD画像を使ったシーンの再構成

rs-captureで取得したRGBD画像群（「image」と「depth」に格納されています）を用いて，Open3DのReconstruction systemによる3次元シーンの再構成を実行します．
下記のコマンドで実行できます．
詳しい方法は[Open3Dのドキュメント](http://www.open3d.org/docs/tutorial/ReconstructionSystem/index.html)を参照してください．

```
cd <your_path_to_open3d_lib>/Tutorial/ReconstructionSystem/
python make_fragments.py [imageとdepthのまでのパス] [-path_intrinsic (optional)]
python register_fragments.py [imageとdepthのまでのパス]
python integrate_scene.py [imageとdepthのまでのパス] [-path_intrinsic (optional)]
```

下記の例のような結果が得られます．

![Reconstruction](https://github.com/sakizuki/SSII2018_Tutorial_Open3D/blob/master/doc/reconstraction.png)