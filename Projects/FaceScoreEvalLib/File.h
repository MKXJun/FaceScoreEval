#include <filesystem>

namespace File
{

// **********************************************
// 功能: 获取文件夹所拥有的子文件夹相对路径
// 参数:
//       [In]InDirectory 指定文件夹
//		 [Out]OutChildDirectory 子文件夹集合
//       [In]isRecursive 是否递归查找
// 返回值: 当前文件夹是否存在
bool GetChildDirectories(std::string InDirectory, std::vector<std::string>& OutChildDirectory, bool isRecursive = false);

// **********************************************
// 功能: 获取文件夹所拥有的子文件相对路径
// 参数:
//       [In]InDirectory 指定文件夹
//       [Out]matchResult 子文件集合
//       [In]isRecursive 是否递归查找
// 返回值: 当前文件夹是否存在
bool GetChildFiles(std::string InDirectory, std::vector<std::string>& OutChildFiles, bool isRecursive = false);



}	// namespace File