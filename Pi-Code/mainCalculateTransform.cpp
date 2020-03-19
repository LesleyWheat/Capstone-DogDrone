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
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/radius_outlier_removal.h>
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

#define XRESOLUTION 1280
#define YRESOLUTION 720

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
	depth_sensor.set_option(RS2_OPTION_VISUAL_PRESET, RS2_RS400_VISUAL_PRESET_DEFAULT);

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

	rs2::spatial_filter spatial_filter;
	rs2::temporal_filter temporal_filter;


	//Filter
	rs2::frame filtered = depth_frame;
	filtered = spatial_filter.process(filtered);
	filtered = temporal_filter.process(filtered);

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

Eigen::IOFormat CommaInitFmt(Eigen::FullPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", " << ", ";");

int main() {

	//Initialize camera
	rs2::pipeline pipe;

	initCamera(pipe);

	//Let camera stabilize
	rs2::frameset frames;
	for (int i = 0; i < 10; i++)
	{
		frames = pipe.wait_for_frames();
	}

	//Capture
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	captureCloud(pipe, cloud);

	//Filter points further than 1m away
	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud(cloud);
	pass.setFilterFieldName("z");
	pass.setFilterLimits(0.0, 0.9);
	pass.filter(*cloud);

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

	//Radial Outlier Removal
	pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;
	outrem.setInputCloud(cloud);
	outrem.setRadiusSearch(0.02);
	outrem.setMinNeighborsInRadius(100);
	outrem.filter(*cloud);

	//Fit largest plane (ground)
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
	pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
	// Create the segmentation object
	pcl::SACSegmentation<pcl::PointXYZ> seg;
	// Optional
	seg.setOptimizeCoefficients(true);
	// Mandatory
	seg.setModelType(pcl::SACMODEL_PLANE);
	seg.setMethodType(pcl::SAC_RANSAC);
	seg.setDistanceThreshold(0.01);

	seg.setInputCloud(cloud);
	seg.segment(*inliers, *coefficients);

	if (inliers->indices.size() == 0)
	{
		PCL_ERROR("Could not estimate a planar model for the given dataset.");
		return (-1);
	}

	std::cerr << "Model coefficients: " << coefficients->values[0] << " "
		<< coefficients->values[1] << " "
		<< coefficients->values[2] << " "
		<< coefficients->values[3] << std::endl;

	//Find rotation of floor to XY plane
	Eigen::Vector3f groundPlaneNormal;
	groundPlaneNormal << coefficients->values[0], coefficients->values[1], coefficients->values[2];

	Eigen::Vector3f xyPlaneNormal;
	xyPlaneNormal << 0.0, 0.0, 1.0;

	Eigen::Vector3f crossProduct = xyPlaneNormal.cross(groundPlaneNormal);

	float theta = -atan2(crossProduct.norm(), xyPlaneNormal.dot(groundPlaneNormal));

	//Build transformation matrix
	Eigen::Affine3f transformationMatrix = Eigen::Affine3f::Identity();
	//transformationMatrix.translation() << 0, 0, -coefficients->values[3] / coefficients->values[2];
	transformationMatrix.rotate(Eigen::AngleAxisf(theta, crossProduct.normalized()));

	cout << "Transformation Matrix: " << endl << transformationMatrix.matrix().format(CommaInitFmt) << endl;

	pcl::PointCloud<pcl::PointXYZ>::Ptr transformedCloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::transformPointCloud(*cloud, *transformedCloud, transformationMatrix);


	//
	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->setBackgroundColor(0, 0, 0);
	viewer->addPointCloud<pcl::PointXYZ>(cloud, "cloud");
	viewer->addPointCloud<pcl::PointXYZ>(transformedCloud, "transformedCloud");
	viewer->addCoordinateSystem(1.0);
	viewer->initCameraParameters();
	viewer->addPlane(*coefficients);

	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}

}