#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/registration/transforms.h>

#include<fstream>
#include <iostream>
#include <sstream>
#include <istream>
#include<string>
#include<algorithm>
#include <sys/io.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdint.h>
#include <liblas/liblas.hpp>
#include <vector>

using namespace std;

int str_cnt = 0;
string str_arr[300];


static void list_dir(const char *path, const char * extension)
{
    str_cnt = 0 ; 
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
      char *str_buff = new char[2];
      strcpy(str_buff, entry->d_name);
      if (string(str_buff) == "updown") {
        continue;
      }
      char* comma = strchr(str_buff, '.');
      if(extension == "pcd"){
        if(string(comma) != ".pcd")
          {
                continue;
          }
      }
      else if(extension == "las"){
        if(string(comma) != ".las")
          {
                continue;
          }
      }
      str_arr[str_cnt] = str_buff;
      str_cnt ++;
    }

    closedir(dir);
}

int
main (int argc, char** argv)
{
  //change pcd to las
  
  pcl::PointCloud<pcl::PointXYZI>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZI>) ;
  pcl::PCDReader reader;
  char* path = "/home/jmw0611/pcd_point/";
  char* save_path = "/home/jmw0611/las_point/";
  
  // create directory and remove old files;
  int unused = system((std::string("exec rm -r ") + save_path).c_str());
  unused = system((std::string("mkdir -p ") + save_path).c_str());
  
  list_dir(path, "pcd");

  for(int i=0; i<str_cnt; i++) {
    cloud.reset(new pcl::PointCloud<pcl::PointXYZI>);
    pcl::io::loadPCDFile<pcl::PointXYZI> (path + str_arr[i], *cloud);
    int point_size = cloud -> points.size() ;

    int file_length = str_arr[i].size();
    char pcd_file[file_length];
    liblas::Header header;

    strcpy(pcd_file, str_arr[i].c_str());
    char* las_file = strtok(pcd_file, ".");
    strcat(las_file, ".las");
    string trajectory = string(save_path) + string(las_file);
    strcpy(las_file, trajectory.c_str());

    std::ofstream ofs;
    ofs.open(las_file, ios::out | ios::binary);

    header.SetDataFormatId(liblas::ePointFormat1);
    header.SetScale(0.01,0.01,0.01);
    liblas::SpatialReference srs;
    srs.SetFromUserInput("EPSG:5180");
    header.SetSRS(srs);
    liblas::Writer writer(ofs, header);
    liblas::Point point(&header);

    for(int j=0; j<point_size; j++) {
      point.SetCoordinates(cloud->points[j].x, cloud->points[j].y, cloud->points[j].z);
      point.SetIntensity(cloud->points[j].intensity);
      writer.WritePoint(point);
    }
  }
  
 /*
  // change las to pcd
  
  std::ifstream ifs;
  pcl::PointCloud<pcl::PointXYZI>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZI>);
  char* path = "/home/jmw0611/las_point/";
  char* save_path = "/home/jmw0611/pcd_point/";
  
  int unused = system((std::string("exec rm -r ") + save_path).c_str());
  unused = system((std::string("mkdir -p ") + save_path).c_str());
  list_dir(path, "las");
  
  for(int i=0; i<str_cnt; i++){
    cloud.reset(new pcl::PointCloud<pcl::PointXYZI>());
    ifs.open(path + str_arr[i], std::ios::in | std::ios::binary);
    
    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);
    liblas::Header const& header = reader.GetHeader();
    cloud -> is_dense = false ;

    while (reader.ReadNextPoint()){
      liblas::Point const& p = reader.GetPoint();
      pcl::PointXYZI point;
      point.x = p.GetX();
      point.y = p.GetY();
      point.z = p.GetZ();
      point.intensity = p.GetIntensity();
     //if las file has rgb channel
      //uint16_t r1 = p.GetColor().GetRed();
      //uint16_t g1 = p.GetColor().GetGreen();
      //uint16_t b1 = p.GetColor().GetBlue();
      //int r2 = ceil(((float)r1/65535)*(float)255);
      //int g2 = ceil(((float)g1/65535)*(float)255);
      //int b2 = ceil(((float)b1/65535)*(float)255);
      //uint32_t rgb = ((int)r2) << 16 | ((int)g2) << 8 | ((int)b2);
      //point.rgb = *reinterpret_cast<float*>(&rgb);
      
      cloud -> push_back(point);
    }
    
    int file_length = str_arr[i].size();
    char las_file[file_length];
    strcpy(las_file, str_arr[i].c_str());
    char* pcd_file = strtok(las_file, ".");
    strcat(pcd_file, ".pcd");
    string trajectory = string(save_path) + string(pcd_file);
    strcpy(pcd_file, trajectory.c_str());

    pcl::io::savePCDFileBinary (pcd_file, *cloud);
    ifs.close();
  }
  
*/

  
  return (0);
}
