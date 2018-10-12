#include "FaceP.h"
#include "File.h"

namespace File
{

namespace fs = std::experimental::filesystem;

// **********************************************
// ����: ��ȡ�ļ�����ӵ�е����ļ������·��
// ����:
//       [In]InDirectory ָ���ļ���
//       [Out]OutChildDirectory ���ļ��м���
//       [In]isRecursive �Ƿ�ݹ����
// ����ֵ: ��ǰ�ļ����Ƿ����
bool GetChildDirectories(std::string InDirectory, std::vector<std::string>& OutChildDirectory, bool isRecursive)
{
	// ����ļ����Ƿ����
	if (!fs::exists(InDirectory) || !fs::is_directory(InDirectory))
	{
		return false;
	}

	OutChildDirectory.clear();
	if (isRecursive)
	{
		// �ݹ��ļ�β������
		fs::recursive_directory_iterator end_iter;

		// ���������ļ���
		for (fs::recursive_directory_iterator iter(InDirectory); iter != end_iter; ++iter)
		{
			// ���ļ��о����
			if (fs::is_directory(iter->status()))
			{
				OutChildDirectory.push_back(iter->path().generic_string());
			}
		}
	}
	else
	{
		// �ļ�β������
		fs::directory_iterator end_iter;

		// ������ǰ�ļ���
		for (fs::directory_iterator iter(InDirectory); iter != end_iter; ++iter)
		{
			// ���ļ��о����
			if (fs::is_directory(iter->status()))
			{
				OutChildDirectory.push_back(iter->path().generic_string());
			}
		}
	}

	return true;
}


// **********************************************
// ����: ��ȡ�ļ�����ӵ�е����ļ����·��
// ����:
//       [In]InDirectory ָ���ļ���
//       [Out]matchResult ���ļ�����
//       [In]isRecursive �Ƿ�ݹ����
// ����ֵ: ��ǰ�ļ����Ƿ����
bool GetChildFiles(std::string InDirectory, std::vector<std::string>& OutChildFiles, bool isRecursive)
{
	// ����ļ����Ƿ����
	if (!fs::exists(InDirectory) || !fs::is_directory(InDirectory))
	{
		return false;
	}

	if (isRecursive)
	{
		// �ݹ��ļ�β������
		fs::recursive_directory_iterator end_iter;

		// �����������ļ���
		for (fs::recursive_directory_iterator iter(InDirectory); iter != end_iter; ++iter)
		{
			// ���ļ������
			if (fs::is_regular_file(iter->status()))
			{
				OutChildFiles.push_back(iter->path().generic_string());
			}
		}
	}
	else
	{
		// �ļ�β������
		fs::directory_iterator end_iter;

		// ������ǰ�ļ���
		for (fs::directory_iterator iter(InDirectory); iter != end_iter; ++iter)
		{
			// ���ļ������
			if (fs::is_regular_file(iter->status()))
			{
				OutChildFiles.push_back(iter->path().generic_string());
			}
		}
	}

	return true;
}





}	// namespace File

