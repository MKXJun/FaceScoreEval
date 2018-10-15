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
	// ����: �����������������������ݿ��ͼƬ��(��������ʶ������)
	// ����:
	//       [In]sourceDir ������ͼƬ���ڵ�Դ�ļ���(δ�任�õ�)
	//       [In]destDir   ���
	//       [In]destFilePath Ŀ�����·������ָ���ļ�������Ϊ.dat
	// ����ֵ: ���ݿ��Ƿ񹹽��ɹ�
	// ��ע: ������ͼƬ�ļ����ڲ�����ͼƬ����ļ�
	//       �������·��������ͼƬ�ļ�����
	bool BuildStarFaceDatabase(const char * sourceDirectory, const char * destDataFile)
	{
		// ���ݲ���
		// [��������
		//   [������]12�ֽ�
		//   [��������]68�ֽ�
		// ]


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


	 // **********************************************
	 // ����: ��ȡ����Output/Source��ͼƬ��Ϣ��
	 // ���任���ͼƬ�������������ݴ����Output/Transform
	void OutputTransform()
	{

		std::string inputDir = "Output/Source";
		std::string outputDir = "Output/Transform";

		// ȷ������·������
		assert(fs::exists(inputDir));

		//��ȡ�����ǵ����ֶ�Ӧ��·��
		std::vector<std::string> starNameDirs;
		assert(File::GetChildDirectories(inputDir, starNameDirs));

		// ��������ļ��п��ܱ����ɵ��ļ����ݣ����ֱ�Ӱ�����ļ��м�������ļ�ɾ�������´���һ����
		if (fs::exists(outputDir))
		{
			fs::remove_all(outputDir);
		}
		assert(fs::create_directory(outputDir));



		// ��ÿ�����Ǵ���������ļ��У�Ȼ�󹹽����ݿ�
		for (std::string starNameDir : starNameDirs)
		{

			// ���������ļ���
			std::string starOutputDir = starNameDir;
			starOutputDir.replace(starOutputDir.find("Source"), 6, "Transform");
			assert(fs::create_directory(starOutputDir));

			// �����������ݿ�
			BuildStarFaceDatabase(
				starNameDir.c_str(),
				(starOutputDir + "/FaceDatabase.dat").c_str()
			);

			std::vector<std::string> starFiles;
			File::GetChildFiles(starNameDir, starFiles);

			// ���б任������ļ�
			for (std::string file : starFiles)
			{
				FaceImage image;
				image.OpenRawFaceImage(file);
				image.SaveFaceImage(file.replace(file.find("Source"), 6, "Transform"));
			}
		}
	}





}	// namespace Face

