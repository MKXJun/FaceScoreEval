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
	// 功能: 构建明星脸的脸部比例数据库
	// 参数:
	//       [In]sourceFolderPath 明星脸图片所在的源文件夹
	//       [In]destFilePath 目标输出路径，需指定文件名类型为.dat
	// 返回值: 数据库是否构建成功
	// 备注: 明星脸图片文件夹内不能有图片外的文件
	//       并且输出路径不能在图片文件夹内
	bool BuildStarFaceDatabase(const char * sourceDirectory, const char * destDataFile)
	{
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


	/**
	 *	@ 描述 - 提取 Output\Source 文件夹中图片信息，分析获取特征数据并存放在 Output\Transform 中。
	 *	@ 作者 - HansonLost
	 */
	void OutputTransform()
	{

		std::string inputDirectory = "Output\\Source";
		std::string outputDirectoy = "Output\\Transform";

		// 确保输入路径存在
		assert(fs::exists(inputDirectory));

		//获取明星们的名字对应的路径
		std::vector<std::string> starsNamePaths;
		assert(File::GetChildDirectories(inputDirectory, starsNamePaths));

		// 明星输出文件夹的绝对路径
		std::vector<std::string> starOutputDirectory;

		// 由于输出文件夹可能保留旧的文件数据，因此直接把输出文件夹及里面的文件删除，重新创建一个。
		if (fs::exists(outputDirectoy))
		{
			// 删除文件/文件夹数目必然不为0
			assert(fs::remove_all(outputDirectoy) != 0);
		}

		assert(fs::create_directory(outputDirectoy));
		for (const std::string& path : starsNamePaths)
		{
			assert(fs::create_directory(path));
		}


		for (size_t i = 0; i < starsNamePaths.size(); ++i)
		{
			// 明星名字和后缀
			size_t pos = starsNamePaths[i].find_last_of('.');
			std::string name = starsNamePaths[i].substr(0, pos);
			std::string postfix = starsNamePaths[i].substr(pos + 1);
			// 该明星的输出文件夹
			const std::string& OutputPath = starOutputDirectory[i];

			// 该明星的源照片文件
			std::vector<std::string> InFilesSet;
			assert(File::GetChildFiles(inputDirectory + '\\' + name, InFilesSet));

			BuildStarFaceDatabase(
				(inputDirectory + '\\' + name).c_str(),
				(OutputPath + '\\' + "FaceDatabase.dat").c_str()
			);

			int ImageNum = 0;
			for (auto file : InFilesSet)
			{
				FaceImage image;
				image.OpenRawFaceImage(file);
				image.SaveFaceImage(OutputPath + '\\' + name + std::to_string(ImageNum) + '.' + postfix);
				++ImageNum;
			}
		}
	}





}	// namespace Face

