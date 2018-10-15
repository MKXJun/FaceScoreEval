//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 头文件名: StarFaceMatch.h
// 
// 作者: X_Jun
//
// 使用库: Opencv3.2, Dlib, STASM
//
// 描述: 构建明星脸集合，并用于检测当前人脸与哪个明星比较相像
//
// 备注：需要支持C++11
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _STARFACEMATCH_H_
#define _STARFACEMATCH_H_

#include "FaceScore.h"

namespace Face
{

struct MatchInfo
{
	std::string starName;	// 明星名
	float similarRate;		// 相似度
};


// **********************************************
// 功能: 构建明星脸的脸部比例数据库
// 参数:
//       [In]sourceFolderPath 明星脸图片所在的源文件夹(已经变换好的)
//       [In]destFilePath 目标输出路径，需指定文件名类型为.dat
// 返回值: 数据库是否构建成功
// 备注: 明星脸图片文件夹内不能有图片外的文件
//       并且输出路径不能在图片文件夹内
bool BuildStarFaceDatabase(const char* sourceDirectory, const char* destDataFilePath);

// **********************************************
// 功能: 从明星脸的脸部比例数据库中搜寻相似的脸型
// 参数:
//       [In]sourceDataFilePath 明星脸的脸部比例数据库
//       [In]facePro 脸型各部分比例数据
//       [Out]matchResult 匹配结果
// 返回值: 若成功输出匹配结果则返回true
bool SearchSimilarStarFace(const char* sourceDataFilePath, const Face::FaceProportion& facePro, MatchInfo* matchResult);

// **********************************************
// 功能: 算出脸型各部分比例的数据
// 参数:
//       [In]pointSet 特征
// 返回值: 返回脸型各部分比例数据
FaceProportion CountProportionData(const FeaturePointSet& pointSet);


// **********************************************
// 功能: 获取文件夹下面的文件名
// 作者: Chitose Ruri
// 参数:
//       [In]path 目标文件夹
//       [Out]fileNames 返回文件夹内文件名，0下标的是上层目录(**/)，不要使用
// 返回值: 操作完成
bool GetFilesNames(std::string directory, std::vector<std::string> & fileNames);



/**
	*	@ 描述 - 提取 Output\Source 文件夹中图片信息，分析获取特征数据并存放在 Output\Transform 中。
	*	@ 作者 - HansonLost
	*/
void OutputTransform();

// **********************************************
// 功能: 获取文件夹下面的文件名
// 作者: Chitose Ruri
// 参数:
//       [In]path stars文件夹的相对路径,如（\star）
// 返回值: 操作完成
bool CheckSourceAndTransform(std::string path);


}	// namespace Face





#endif