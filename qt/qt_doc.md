
### 编译QT6.7


```
sudo apt install libgl1-mesa-dev

sudo apt-get install libxcb-cursor0

sudo apt-get install mesa-utils

sudo apt install ninja-build

cmake .. -DCMAKE_INSTALL_PREFIX=/home/hejunlin/qt6


裁剪

cmake .. -DQT_BUILD_EXAMPLES=OFF \
    -DQT_FEATURE_gui=ON -DQT_FEATURE_widgets=ON -DQT_FEATURE_sql=OFF \
    -DCMAKE_INSTALL_PREFIX=/home/hejunlin/qt6

    


```