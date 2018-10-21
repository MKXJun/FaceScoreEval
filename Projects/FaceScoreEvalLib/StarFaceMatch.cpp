#include "FaceP.h"
#include "StarFaceMatch.h"
#include "FaceImage.h"
#include "File.h"
#include "Utility.h"
	
#include <fstream>


namespace Face
{

	namespace fs = std::experimental::filesystem;

	// **********************************************
	// 功能: 构建明星脸的脸部比例数据库和图片库(减少人脸识别库调用)
	// 参数:
	//       [In]sourceDir 明星脸图片所在的源文件夹(未变换好的)
	//       [In]destDir   输出
	//       [In]destFilePath 目标输出路径，需指定文件名类型为.dat
	// 返回值: 数据库是否构建成功
	// 备注: 明星脸图片文件夹内不能有图片外的文件
	//       并且输出路径不能在图片文件夹内
	bool BuildStarFaceDatabase(const char * sourceDirectory, const char * destDataFile)
	{
		// 数据布局
		// [明星数据
		//   [明星名]12字节
		//   [比例数据]68字节
		// ]


		// 构造路径，检验路径是否存在
		fs::path directory(sourceDirectory);
		if (!fs::exists(directory))
			return false;

		// 文件尾迭代器
		fs::directory_iterator end_iter;
		// 子文件路径
		std::string picPath;
		// 特征数据
		FeaturePointSet pointSet;

		// 新建或打开.dat文件
		std::ofstream fout(destDataFile, std::ios::out | std::ios::binary);
		if (!fout.is_open())
		{
			fout.close();
			return false;
		}

		// 遍历当前文件夹
		for (fs::directory_iterator iter(directory); iter != end_iter; ++iter)
		{
			// 是普通文件就打开并获取信息
			if (fs::is_regular_file(iter->status()))
			{
				picPath = iter->path().string();
				if (!ExtractFeaturePoint(picPath.c_str(), &pointSet))
					continue;

				// 获取文件名中的明星名字
				size_t beg = picPath.find_last_of('\\') + 1, ed = picPath.find_last_of('.');
				std::string starName = picPath.substr(beg, ed - beg);
				// 获取脸部比例数据
				FaceProportion faceProportion(pointSet);
				// 写入文件信息
				fout.write(starName.c_str(), 12);
				fout.write(reinterpret_cast<const char*>(&faceProportion), sizeof(FaceProportion));
			}
		}
		fout.close();

		return true;
	}

	// **********************************************
	// 功能: 从明星脸的脸部比例数据库中搜寻相似的脸型
	// 参数:
	//       [In]sourceDataFilePath 明星脸的脸部比例数据库
	//       [Out]matchResult 匹配结果
	// 返回值: 若成功输出匹配结果则返回true
	bool SearchSimilarStarFace(const char* sourceDataFilePath, const FaceProportion& facePro, MatchInfo* pMatchResult)
	{
		// 检验输出是否为非空指针
		if (pMatchResult == nullptr)
			return false;

		// 检验文件路径是否存在
		std::ifstream fin(sourceDataFilePath, std::ios::in | std::ios::binary);
		if (!fin.is_open())
			return false;

		// 清空数据
		pMatchResult->starName.clear();
		pMatchResult->similarRate = 0.0f;

		char starName[20];
		FaceProportion faceProportion;
		float similarRate, percent;

		// 通过文件大小获取明星数目
		fin.seekg(0, std::ios::end);
		int numStars = (int)fin.tellg() / (sizeof(FaceProportion) + 12);
		fin.seekg(0, std::ios::beg);

		while (numStars--)
		{
			similarRate = 0.0f;
			faceProportion = {};
			fin.read(starName, 12);
			fin.read(reinterpret_cast<char *>(&faceProportion), sizeof(FaceProportion));

			// 进行明星脸数据的匹配度检验
			// 计算两个同类比例数据的比值，若大于1则求倒数
			// 将结果加起来求平均值后即为特征相似度
			for (int i = 0; i < sizeof(FaceProportion) / sizeof(float); ++i)
			{
				percent = fabs((reinterpret_cast<const float*>(&faceProportion))[i] / (reinterpret_cast<const float*>(&facePro))[i]);
				percent = percent > 1.0f ? 1.0f / percent : percent;
				similarRate += percent;
			}
			similarRate /= (sizeof(FaceProportion) / sizeof(float));

			// 更新最大值
			if (similarRate > pMatchResult->similarRate)
			{
				pMatchResult->starName = starName;
				pMatchResult->similarRate = similarRate;
			}
		}

		fin.close();
		return true;
	}

	FaceProportion CountProportionData(const FeaturePointSet & pointSet)
	{
		return FaceProportion(pointSet);
	}


	 // **********************************************
	 // 功能: 提取分析Output/Source的图片信息，
	 // 将变换后的图片和脸部比例数据存放在Output/Transform
	void OutputTransform()
	{

		std::string inputDir = "Output/Source";
		std::string outputDir = "Output/Transform";

		// 确保输入路径存在
		assert(fs::exists(inputDir));

		//获取明星们的名字对应的路径
		std::vector<std::string> starNameDirs;
		assert(File::GetChildDirectories(inputDir, starNameDirs));

		// 由于输出文件夹可能保留旧的文件数据，因此直接把输出文件夹及里面的文件删除，重新创建一个。
		if (fs::exists(outputDir))
		{
			fs::remove_all(outputDir);
		}
		assert(fs::create_directory(outputDir));



		// 给每个明星创建输出的文件夹，然后构建数据库
		for (std::string starNameDir : starNameDirs)
		{

			// 创建明星文件夹
			std::string starOutputDir = starNameDir;
			starOutputDir.replace(starOutputDir.find("Source"), 6, "Transform");
			assert(fs::create_directory(starOutputDir));

			// 构建明星数据库
			BuildStarFaceDatabase(
				starNameDir.c_str(),
				(starOutputDir + "/FaceDatabase.dat").c_str()
			);

			std::vector<std::string> starFiles;
			File::GetChildFiles(starNameDir, starFiles);

			// 进行变换并输出文件
			for (std::string file : starFiles)
			{
				FaceImage image;
				image.OpenRawFaceImage(file);
				image.SaveFaceImage(file.replace(file.find("Source"), 6, "Transform"));
			}
		}
	}





}	// namespace Face

