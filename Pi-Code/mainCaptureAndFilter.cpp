// RealsenseResources.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define VTK_LEGACY_SILENT
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <atomic>
#include <fstream>
#include <algorithm>
#include <librealsense2/rs.hpp>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/common/common_headers.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/segmentation/region_growing.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/keypoints/uniform_sampling.h>
#include <pcl/registration/transforms.h>
#include <pcl/registration/icp.h>
#include <pcl/registration/icp_nl.h>
#include <pcl/visualization/range_image_visualizer.h>
#include <pcl/keypoints/uniform_sampling.h>
#include <pcl/features/normal_3d.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/features/moment_of_inertia_estimation.h>
#include <pcl/filters/project_inliers.h>
#include <pcl/surface/concave_hull.h>
#include <pcl/common/intersections.h>

#ifdef _DEBUG

#pragma comment(lib,"pcl_common_debug.lib")
#pragma comment(lib,"pcl_features_debug.lib")
#pragma comment(lib,"pcl_filters_debug.lib")
#pragma comment(lib,"pcl_io_debug.lib")
#pragma comment(lib,"pcl_io_ply_debug.lib")
#pragma comment(lib,"pcl_kdtree_debug.lib")
#pragma comment(lib,"pcl_keypoints_debug.lib")
#pragma comment(lib,"pcl_octree_debug.lib")
#pragma comment(lib,"pcl_outofcore_debug.lib")
#pragma comment(lib,"pcl_people_debug.lib")
#pragma comment(lib,"pcl_recognition_debug.lib")
#pragma comment(lib,"pcl_registration_debug.lib")
#pragma comment(lib,"pcl_sample_consensus_debug.lib")
#pragma comment(lib,"pcl_search_debug.lib")
#pragma comment(lib,"pcl_segmentation_debug.lib")
#pragma comment(lib,"pcl_surface_debug.lib")
#pragma comment(lib,"pcl_tracking_debug.lib")
#pragma comment(lib,"pcl_visualization_debug.lib")
#pragma comment(lib,"vtkRenderingCore-8.1-gd.lib")
#pragma comment(lib,"vtkCommonCore-8.1-gd.lib")
#pragma comment(lib,"vtkCommonColor-8.1-gd.lib")
#pragma comment(lib,"vtkCommonComputationalGeometry-8.1-gd.lib")
#pragma comment(lib,"vtkCommonDataModel-8.1-gd.lib")
#pragma comment(lib,"vtkCommonExecutionModel-8.1-gd.lib")
#pragma comment(lib,"vtkCommonMath-8.1-gd.lib")
#pragma comment(lib,"vtkCommonMisc-8.1-gd.lib")
#pragma comment(lib,"vtkCommonSystem-8.1-gd.lib")
#pragma comment(lib,"vtkCommonTransforms-8.1-gd.lib")
#pragma comment(lib,"vtkIOCore-8.1-gd.lib")
#pragma comment(lib,"vtkFiltersCore-8.1-gd.lib")
#pragma comment(lib,"vtksys-8.1-gd.lib")
#pragma comment(lib,"flann-gd.lib")
#pragma comment(lib,"flann_s-gd.lib")
#pragma comment(lib,"flann_cpp-gd.lib")
#pragma comment(lib,"flann_cpp_s-gd.lib")
#pragma comment(lib,"openNI2.lib")


#else

#pragma comment(lib,"pcl_common_release.lib")
#pragma comment(lib,"pcl_features_release.lib")
#pragma comment(lib,"pcl_filters_release.lib")
#pragma comment(lib,"pcl_io_release.lib")
#pragma comment(lib,"pcl_io_ply_release.lib")
#pragma comment(lib,"pcl_kdtree_release.lib")
#pragma comment(lib,"pcl_keypoints_release.lib")
#pragma comment(lib,"pcl_octree_release.lib")
#pragma comment(lib,"pcl_outofcore_release.lib")
#pragma comment(lib,"pcl_people_release.lib")
#pragma comment(lib,"pcl_recognition_release.lib")
#pragma comment(lib,"pcl_registration_release.lib")
#pragma comment(lib,"pcl_sample_consensus_release.lib")
#pragma comment(lib,"pcl_search_release.lib")
#pragma comment(lib,"pcl_segmentation_release.lib")
#pragma comment(lib,"pcl_surface_release.lib")
#pragma comment(lib,"pcl_tracking_release.lib")
#pragma comment(lib,"pcl_visualization_release.lib")
#pragma comment(lib,"vtkRenderingCore-8.1.lib")
#pragma comment(lib,"vtkCommonCore-8.1.lib")
#pragma comment(lib,"vtkCommonColor-8.1.lib")
#pragma comment(lib,"vtkCommonComputationalGeometry-8.1.lib")
#pragma comment(lib,"vtkCommonDataModel-8.1.lib")
#pragma comment(lib,"vtkCommonExecutionModel-8.1.lib")
#pragma comment(lib,"vtkCommonMath-8.1.lib")
#pragma comment(lib,"vtkCommonMisc-8.1.lib")
#pragma comment(lib,"vtkCommonSystem-8.1.lib")
#pragma comment(lib,"vtkCommonTransforms-8.1.lib")
#pragma comment(lib,"vtkIOCore-8.1.lib")
#pragma comment(lib,"vtkFiltersCore-8.1.lib")
#pragma comment(lib,"vtksys-8.1.lib")
#pragma comment(lib,"flann.lib")
#pragma comment(lib,"flann_s.lib")
#pragma comment(lib,"flann_cpp.lib")
#pragma comment(lib,"flann_cpp_s.lib")
#pragma comment(lib,"openNI2.lib")

#endif

//#define XRESOLUTION 1280
//#define YRESOLUTION 720
#define XRESOLUTION 640
#define YRESOLUTION 360

#define MIN_Y 0.01
#define MAX_Y 1.35
#define MIN_Z -0.02
#define MAX_Z 0.2

void pointPickingEventOccurred(const pcl::visualization::PointPickingEvent& event, void* viewer_void)
{
	std::cout << "[INOF] Point picking event occurred." << std::endl;

	float x, y, z;
	if (event.getPointIndex() == -1)
	{
		return;
	}
	event.getPoint(x, y, z);
	std::cout << "[INOF] Point coordinate ( " << x << ", " << y << ", " << z << ")" << std::endl;
}

//Initialize the camera
int initCamera(rs2::pipeline &pipe) {

	rs2::context ctx;
	rs2::config cfg;

	//Check for camera
	auto list = ctx.query_devices();
	if (list.size() == 0) {
		cout << "No device detected. Is it plugged in?\n";
		cin.get();
		return 1;
	}

	//cfg.enable_stream(RS2_STREAM_COLOR, XRESOLUTION, YRESOLUTION, RS2_FORMAT_BGR8);
	cfg.enable_stream(RS2_STREAM_DEPTH, XRESOLUTION, YRESOLUTION, RS2_FORMAT_Z16);
	
	auto profile = pipe.start(cfg);
	auto depth_sensor = profile.get_device().first<rs2::depth_sensor>();

	//Set options
	depth_sensor.set_option(RS2_OPTION_VISUAL_PRESET, RS2_RS400_VISUAL_PRESET_HIGH_ACCURACY);

	return 0;
}

//Save cloud
int logCloudToPCD(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, std::string fileName) {

	time_t currentTime = time(NULL);
	struct tm * now = localtime(&currentTime);

	//Name image
	char buffer[100];
	strftime(buffer, 100, "%F-%H-%M-%S", now);

	std::string fileTime(buffer);
	std::string fileAppend = fileTime + ".pcd";

	fileName = fileName + fileAppend;
	pcl::io::savePCDFileASCII(fileName, *cloud);

	return 0;
}

//Convert to PCL cloud
int pointsToPCL(const rs2::points& points, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{

	rs2::video_stream_profile sp = points.get_profile().as<rs2::video_stream_profile>();
	cloud->width = sp.width();
	cloud->height = sp.height();
	cloud->is_dense = false;
	cloud->points.resize(points.size());
	auto ptr = points.get_vertices();
	for (auto& p : cloud->points)
	{
		p.x = ptr->x;
		p.y = ptr->y;
		p.z = ptr->z;
		ptr++;
	}

	return 0;
}

//Capture and calculate point cloud
int captureCloud(rs2::pipeline &pipe, pcl::PointCloud<pcl::PointXYZ>::Ptr outCloud) {

	//Capture depth image
	rs2::frameset frames = pipe.wait_for_frames();
	rs2::frame depth_frame = frames.get_depth_frame();

	//Calculate point cloud
	rs2::pointcloud pc;
	rs2::points points;
	points = pc.calculate(depth_frame);

	//Convert to pcl point cloud
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pointsToPCL(points, cloud);

	//Eliminate invalid points
	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud(cloud);
	pass.setFilterFieldName("z");
	pass.setFilterLimits(0.0, 0.0);
	pass.setFilterLimitsNegative(true);
	pass.filter(*outCloud);

	return 0;
}

int transformPointCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr inCloud, pcl::PointCloud<pcl::PointXYZ>::Ptr outCloud) {



	return 0;
}

int main()
{
	//Initialize variables
	Eigen::Matrix4f TsensorToRover;
	TsensorToRover << 0.999954, 0.0068174, -0.00677314, 0, 0.0068174, -0.00648923, 0.999956, 0, 0.00677314, -0.999956, -0.00653541, 0, 0, 0, 0, 1;

	//Initialize camera
	rs2::pipeline pipe;

	initCamera(pipe);

	//Let camera stabilize
	rs2::frameset frames;
	for (int i = 0; i < 10; i++)
	{
		frames = pipe.wait_for_frames();
	}

	//Communicate status
	//

	//Enter main loop
	while (1) {

		//Take image and calculate point cloud
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
		captureCloud(pipe, cloud);

		//Transform point cloud
		pcl::transformPointCloud(*cloud, *cloud, TsensorToRover);

		//Passthrough filter on Y and Z
		pcl::PassThrough<pcl::PointXYZ> pass;
		pass.setInputCloud(cloud);
		pass.setFilterFieldName("y");
		pass.setFilterLimits(MIN_Y, MAX_Y);
		pass.filter(*cloud);

		pass.setFilterFieldName("z");
		pass.setFilterLimits(MIN_Z, MAX_Z);
		pass.filter(*cloud);
		
		////Statistical Outlier Removal
		pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
		sor.setInputCloud(cloud);
		sor.setMeanK(50);
		sor.setStddevMulThresh(1.0);
		sor.filter(*cloud);

		//Downsample (Voxel grid)
		  // Create the filtering object
		pcl::VoxelGrid<pcl::PointXYZ> vox;
		vox.setInputCloud(cloud);
		vox.setLeafSize(0.01f, 0.01f, 10.0f);
		vox.filter(*cloud);

		//Radius Outlier
		pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;
		outrem.setInputCloud(cloud);
		outrem.setRadiusSearch(0.05);
		outrem.setMinNeighborsInRadius(2);
		outrem.filter(*cloud);

		//Wait for take image command
		//



		logCloudToPCD(cloud, "cloud.pcd");



	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->setBackgroundColor(0, 0, 0);
	viewer->addPointCloud<pcl::PointXYZ>(cloud, "sample cloud");
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");
	viewer->addCoordinateSystem(0.12);
	viewer->initCameraParameters();
	viewer->registerPointPickingCallback(pointPickingEventOccurred, (void*)&viewer);

	while (!viewer->wasStopped())
	{

		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	if (pcl::io::loadPCDFile<pcl::PointXYZ>("cloud.pcd2020-01-28-02-30-05.pcd", *cloud) == -1) //* load the file
	{
		PCL_ERROR("Couldn't read file test_pcd.pcd \n");
		return (-1);
	}

	Eigen::MatrixXf eigenCloud = cloud->getMatrixXfMap(2, 4, 0);
	
	Eigen::IOFormat CSVFormat(Eigen::FullPrecision, Eigen::DontAlignCols, ", ", "\n", "", "", "", "");


	std::ofstream file("eigenCloudCSV.txt");
	if (file.is_open())
	{
		file << eigenCloud.transpose().format(CSVFormat) << endl;
		file.close();
	}

	return 0;
}

//Start thread:
			//Initialize camera and settings
			//Communicate status
			//Start main loop

//Main loop:
			//Wait for take image command
			//Take Image + Temporal/Spatial Filter?
			//Transformation - how to calibrate? Could fit a plane onto totally flat ground while mounted on the rover. Could do this on command/automatically without user input if needed.
			//Passthrough filter on Z and Y (ground level and far away cropped out)
			//Downsampling
			//Statistical Outlier Removal
			//Clustering - Only useful if we want to distinguish obstacles from each other - how expensive?
			//
			//
			//
			//

//MAPPING:
	//Transform point cloud to world coordinates

//3 COORDINATE SYSTEMS: CAMERA, ROVER, AND WORLD
//Could implement own depth image -> cloud function, do pre-filtration
//Potentional settings to change: disparity shift, depth units, exposure, 
//DOWNSAMPLING: INCREASE LEAF SIZE ALONG Z