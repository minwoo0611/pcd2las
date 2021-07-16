# pcd_to_las

**pcd2las is tools to change extension of all file from pcd to las or from las to pcd in a folder. You have to set folder paths which the original file is in(path) where the converted file will be saved(save_path). All files with pcd(or las) extension in the folder are replaced with las(or pcd) extension. las_to_pcd is commented out, you just need to uncommented it to use it. **

## Dependency

- [PCL](https://pointclouds.org/)
  ```
  sudo apt install libpcl-dev
  ```

## Install

Use the following commands to download and compile the package.

```
mkdir catkin_ws
cd ~/catkin_ws
git clone https://github.com/minwoo0611/pcd_to_las
cd pcd_to_las/
mkdir build
cd build
cmake ..
make
```

## Run the package

Run the file in build folder:
```
./las_to_pcd
```

