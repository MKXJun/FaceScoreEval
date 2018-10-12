#include <filesystem>

namespace File
{

// **********************************************
// ����: ��ȡ�ļ�����ӵ�е����ļ������·��
// ����:
//       [In]InDirectory ָ���ļ���
//		 [Out]OutChildDirectory ���ļ��м���
//       [In]isRecursive �Ƿ�ݹ����
// ����ֵ: ��ǰ�ļ����Ƿ����
bool GetChildDirectories(std::string InDirectory, std::vector<std::string>& OutChildDirectory, bool isRecursive = false);

// **********************************************
// ����: ��ȡ�ļ�����ӵ�е����ļ����·��
// ����:
//       [In]InDirectory ָ���ļ���
//       [Out]matchResult ���ļ�����
//       [In]isRecursive �Ƿ�ݹ����
// ����ֵ: ��ǰ�ļ����Ƿ����
bool GetChildFiles(std::string InDirectory, std::vector<std::string>& OutChildFiles, bool isRecursive = false);



}	// namespace File