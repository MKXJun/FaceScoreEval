#include "FaceP.h"
#include "File.h"

namespace File
{

namespace fs = std::experimental::filesystem;

// **********************************************
// 功能: 获取文件夹所拥有的子文件夹相对路径
// 参数:
//       [In]InDirectory 指定文件夹
//       [Out]OutChildDirectory 子文件夹集合
//       [In]isRecursive 是否递归查找
// 返回值: 当前文件夹是否存在
bool GetChildDirectories(std::string InDirectory, std::vector<std::string>& OutChildDirectory, bool isRecursive)
{
	// 检查文件夹是否存在
	if (!fs::exists(InDirectory) || !fs::is_directory(InDirectory))
	{
		return false;
	}

	OutChildDirectory.clear();
	if (isRecursive)
	{
		// 递归文件尾迭代器
		fs::recursive_directory_iterator end_iter;

		// 遍历所有文件夹
		for (fs::recursive_directory_iterator iter(InDirectory); iter != end_iter; ++iter)
		{
			// 是文件夹就添加
			if (fs::is_directory(iter->status()))
			{
				OutChildDirectory.push_back(iter->path().generic_string());
			}
		}
	}
	else
	{
		// 文件尾迭代器
		fs::directory_iterator end_iter;

		// 遍历当前文件夹
		for (fs::directory_iterator iter(InDirectory); iter != end_iter; ++iter)
		{
			// 是文件夹就添加
			if (fs::is_directory(iter->status()))
			{
				OutChildDirectory.push_back(iter->path().generic_string());
			}
		}
	}

	return true;
}


// **********************************************
// 功能: 获取文件夹所拥有的子文件相对路径
// 参数:
//       [In]InDirectory 指定文件夹
//       [Out]matchResult 子文件集合
//       [In]isRecursive 是否递归查找
// 返回值: 当前文件夹是否存在
bool GetChildFiles(std::string InDirectory, std::vector<std::string>& OutChildFiles, bool isRecursive)
{
	// 检查文件夹是否存在
	if (!fs::exists(InDirectory) || !fs::is_directory(InDirectory))
	{
		return false;
	}

	if (isRecursive)
	{
		// 递归文件尾迭代器
		fs::recursive_directory_iterator end_iter;

		// 遍历所有子文件夹
		for (fs::recursive_directory_iterator iter(InDirectory); iter != end_iter; ++iter)
		{
			// 是文件就添加
			if (fs::is_regular_file(iter->status()))
			{
				OutChildFiles.push_back(iter->path().generic_string());
			}
		}
	}
	else
	{
		// 文件尾迭代器
		fs::directory_iterator end_iter;

		// 遍历当前文件夹
		for (fs::directory_iterator iter(InDirectory); iter != end_iter; ++iter)
		{
			// 是文件就添加
			if (fs::is_regular_file(iter->status()))
			{
				OutChildFiles.push_back(iter->path().generic_string());
			}
		}
	}

	return true;
}





}	// namespace File

