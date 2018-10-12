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
	// ����: �����������������������ݿ�
	// ����:
	//       [In]sourceFolderPath ������ͼƬ���ڵ�Դ�ļ���
	//       [In]destFilePath Ŀ�����·������ָ���ļ�������Ϊ.dat
	// ����ֵ: ���ݿ��Ƿ񹹽��ɹ�
	// ��ע: ������ͼƬ�ļ����ڲ�����ͼƬ����ļ�
	//       �������·��������ͼƬ�ļ�����
	bool BuildStarFaceDatabase(const char * sourceDirectory, const char * destDataFile)
	{
		// ����·��������·���Ƿ����
		fs::path directory(sourceDirectory);
		if (!fs::exists(directory))
			return false;

		// �ļ�β������
		fs::directory_iterator end_iter;
		// ���ļ�·��
		std::string picPath;
		// ��������
		FeaturePointSet pointSet;

		// �½����.dat�ļ�
		std::ofstream fout(destDataFile, std::ios::out | std::ios::binary);
		if (!fout.is_open())
		{
			fout.close();
			return false;
		}

		// ������ǰ�ļ���
		for (fs::directory_iterator iter(directory); iter != end_iter; ++iter)
		{
			// ����ͨ�ļ��ʹ򿪲���ȡ��Ϣ
			if (fs::is_regular_file(iter->status()))
			{
				picPath = iter->path().string();
				if (!ExtractFeaturePoint(picPath.c_str(), &pointSet))
					continue;

				// ��ȡ�ļ����е���������
				size_t beg = picPath.find_last_of('\\') + 1, ed = picPath.find_last_of('.');
				std::string starName = picPath.substr(beg, ed - beg);
				// ��ȡ������������
				FaceProportion faceProportion(pointSet);
				// д���ļ���Ϣ
				fout.write(starName.c_str(), 12);
				fout.write(reinterpret_cast<const char*>(&faceProportion), sizeof(FaceProportion));
			}
		}
		fout.close();

		return true;
	}

	// **********************************************
	// ����: ���������������������ݿ�����Ѱ���Ƶ�����
	// ����:
	//       [In]sourceDataFilePath �������������������ݿ�
	//       [Out]matchResult ƥ����
	// ����ֵ: ���ɹ����ƥ�����򷵻�true
	bool SearchSimilarStarFace(const char* sourceDataFilePath, const FaceProportion& facePro, MatchInfo* pMatchResult)
	{
		// ��������Ƿ�Ϊ�ǿ�ָ��
		if (pMatchResult == nullptr)
			return false;

		// �����ļ�·���Ƿ����
		std::ifstream fin(sourceDataFilePath, std::ios::in | std::ios::binary);
		if (!fin.is_open())
			return false;

		// �������
		pMatchResult->starName.clear();
		pMatchResult->similarRate = 0.0f;

		char starName[20];
		FaceProportion faceProportion;
		float similarRate, percent;

		// ͨ���ļ���С��ȡ������Ŀ
		fin.seekg(0, std::ios::end);
		int numStars = (int)fin.tellg() / (sizeof(FaceProportion) + 12);
		fin.seekg(0, std::ios::beg);

		while (numStars--)
		{
			similarRate = 0.0f;
			faceProportion = {};
			fin.read(starName, 12);
			fin.read(reinterpret_cast<char *>(&faceProportion), sizeof(FaceProportion));

			// �������������ݵ�ƥ��ȼ���
			// ��������ͬ��������ݵı�ֵ��������1������
			// �������������ƽ��ֵ��Ϊ�������ƶ�
			for (int i = 0; i < sizeof(FaceProportion) / sizeof(float); ++i)
			{
				percent = fabs((reinterpret_cast<const float*>(&faceProportion))[i] / (reinterpret_cast<const float*>(&facePro))[i]);
				percent = percent > 1.0f ? 1.0f / percent : percent;
				similarRate += percent;
			}
			similarRate /= (sizeof(FaceProportion) / sizeof(float));

			// �������ֵ
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
	 *	@ ���� - ��ȡ Output\Source �ļ�����ͼƬ��Ϣ��������ȡ�������ݲ������ Output\Transform �С�
	 *	@ ���� - HansonLost
	 */
	void OutputTransform()
	{

		std::string inputDirectory = "Output\\Source";
		std::string outputDirectoy = "Output\\Transform";

		// ȷ������·������
		assert(fs::exists(inputDirectory));

		//��ȡ�����ǵ����ֶ�Ӧ��·��
		std::vector<std::string> starsNamePaths;
		assert(File::GetChildDirectories(inputDirectory, starsNamePaths));

		// ��������ļ��еľ���·��
		std::vector<std::string> starOutputDirectory;

		// ��������ļ��п��ܱ����ɵ��ļ����ݣ����ֱ�Ӱ�����ļ��м�������ļ�ɾ�������´���һ����
		if (fs::exists(outputDirectoy))
		{
			// ɾ���ļ�/�ļ�����Ŀ��Ȼ��Ϊ0
			assert(fs::remove_all(outputDirectoy) != 0);
		}

		assert(fs::create_directory(outputDirectoy));
		for (const std::string& path : starsNamePaths)
		{
			assert(fs::create_directory(path));
		}


		for (size_t i = 0; i < starsNamePaths.size(); ++i)
		{
			// �������ֺͺ�׺
			size_t pos = starsNamePaths[i].find_last_of('.');
			std::string name = starsNamePaths[i].substr(0, pos);
			std::string postfix = starsNamePaths[i].substr(pos + 1);
			// �����ǵ�����ļ���
			const std::string& OutputPath = starOutputDirectory[i];

			// �����ǵ�Դ��Ƭ�ļ�
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

