#include <stdio.h> 
#include<iostream> 
#include<vector> 
#include <Windows.h> 
#include<cstdlib>
#include<algorithm>
#include <fstream>  
#include <iterator> 
#include <string> 
#include <cstdlib>
#pragma warning(disable:4996)
using namespace std;
#define MAX_PATH 1024 //最长路径长度 

FILE* fp;
char* replace(char* str, const char* from, const char* to) {
    size_t str_len = strlen(str);
    size_t from_len = strlen(from);
    size_t to_len = strlen(to);
    size_t new_len = str_len + to_len - from_len;
    char* new_str = new char[new_len + 1];
    char* p1 = str;
    char* p2 = new_str;
    while (*p1) {
        if (strncmp(p1, from, from_len) == 0) {
            strncpy(p2, to, to_len);
            p2 += to_len;
            p1 += from_len;
        }
        else {
            *p2++ = *p1++;
        }
    }
    *p2 = '\0';
    delete[] str;
    return new_str;
}
void setconsoletextattribute(int a) {
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(outputHandle, a);
}
int getsize(char m) {
    // 获取输出句柄
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(outputHandle, &info);
    if (m=='x')
    {
        return info.srWindow.Right+1;
    }
    else
    {
        return info.srWindow.Bottom+1;
    }
}
void gotoxy(int x, int y) { 
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
int S2I(string s) {
    int len = s.length();  //.length   表示字符串的长度
    int a, b,//用来循环扫描字符串
        bit,//在一个字符串中，对一个外形是数字的字符，把这样的字符转化成数字，存与bit中
        num;//最终存放提取出的数字
    int flag = 0;//判断字符串中是否有数字
    for (a = 0; a < len; a++)
    {
        if (s[a] >= '0' && s[a] <= '9')//过滤掉外形是非数字的字符
        {
            flag++;
            b = a;
            num = 0;//num每次循环的初值置为0
            while (b < len&& s[b] >= '0' && s[b] <= '9')
            {
                bit = s[b] - '0';
                num = num * 10 + bit;
                b++;
            }
            a = b;
        }
    }
    if (flag == 0)
        return -1;
    return num;
}
bool cmp_max(string x, string y) {
    return S2I(x) > S2I(y);
}
bool Sstrstr(string a,string b) {
    if (strstr(a.c_str(), b.c_str()) == NULL) //在a中查找b，如果不存在，
        return 0;
    else //否则存在。
        return 1;
}
/*----------------------------
 * 功能 : 递归遍历文件夹，找到其中包含的所有文件
 *----------------------------
 * 函数 : find
 * 访问 : public
 *
 * 参数 : lpPath [in]   需遍历的文件夹目录
 * 参数 : fileList [in]  以文件名称的形式存储遍历后的文件
 */
void find(const char* lpPath, std::vector<std::string>& fileList)
{
    char szFind[MAX_PATH];
    WIN32_FIND_DATA FindFileData;

    strcpy(szFind, lpPath);
    strcat(szFind, "\\*.*");

    HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
    if (INVALID_HANDLE_VALUE == hFind)  return;

    while (true)
    {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (FindFileData.cFileName[0] != '.')
            {
                char szFile[MAX_PATH];
                strcpy(szFile, lpPath);
                strcat(szFile, "\\");
                strcat(szFile, (char*)(FindFileData.cFileName));
                find(szFile, fileList);
            }
        }
        else
        {
            //std::cout << FindFileData.cFileName << std::endl; 
            fileList.push_back(FindFileData.cFileName);
        }
        if (!FindNextFile(hFind, &FindFileData))  break;
    }
    FindClose(hFind);
}
bool isFileExists_fopen(string& name) {
    if (FILE* file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}
void strread(char str[], const char delims[], std::vector<std::string>& resultList) {
    char* result = NULL;
    result = strtok(str, delims);
    while (result != NULL) {
        printf("result is \"%s\"\n", result);
        resultList.push_back(result);
        result = strtok(NULL, delims);
    }
}
int main(int argc, char** argv)
{
    setconsoletextattribute(0x07);
    for (int i = 0; i < argc; i++)
    {
        cout << "argument[" << i << "] is: " << argv[i] << endl;
    }
    string tmp;
    if (argc>1)
    {
        tmp = argv[1];
    }
    else {
        setconsoletextattribute(FOREGROUND_RED);
        printf("Missing args!\n");
        setconsoletextattribute(0x07);
        return 3;
    }
    if (isFileExists_fopen(tmp))
    {
        while (1) {
            gotoxy(0, getsize('y'));
            setconsoletextattribute(FOREGROUND_RED);
            printf("File \"%s\" already exists, do you want to overwrite it?([y]/[n])\r\n>>", tmp.c_str());
            setconsoletextattribute(0x07);
            char a[114];
            scanf("%s", a);
            if (a[0]=='y')
            {
                break;
            }
            else if (a[0] == 'n') {
                printf("Cancelled by user");
                return 2;
            }
        }
    }
    fp = fopen(tmp.c_str(), "w");
    if (fp == NULL)
    {
        // 打开文件失败
        printf("Error opening file!\n");
        return 1;
    }
    std::vector<std::string> fileList;//定义一个存放结果文件名称的链表 
    //遍历一次结果的所有文件,获取文件名列表 
    find(".",fileList);//之后可对文件列表中的文件进行相应的操作 
    for (int i = 0; i < fileList.size(); i++)
    {
        string tmp;
        tmp = fileList.back();
        fileList.pop_back();
        if (Sstrstr(tmp, ".mcfunction"))
        {
            fileList.push_back(tmp);
        }
    }
    sort(fileList.rbegin(), fileList.rend(),cmp_max);
    //输出文件夹下所有文件的名称 
    int sleepticks = 0;
    //fprintf(fp, "/* Generated by noteblockstudio2java. You may need to perform the \"Alt + enter\" operation manually */\npublic class %s extends Thread{\n\nprivate final EntityPlayer entityplayer; private final World world;private final BlockPos bp;\npublic %s(EntityPlayer entityplayer, World world, BlockPos bp){\n        this.entityplayer=entityplayer;\n        this.world=world;\n        this.bp=bp;\n    }\n    @Override\n    public void run(){\n        try {\n            for (int i=0;i<namelist.length;i++){\n                Thread.sleep(t[i]);\n                world.playSound(entityplayer, bp.getX(), bp.getY(), bp.getZ(), new SoundEvent(new ResourceLocation(namelist[i])), SoundCategory.getByName(SoundCategory1[i]), (float)v[i], (float)p[i]);\n            }\n        } catch (InterruptedException e) {\n            throw new RuntimeException(e);\n        }\n    }\n",argv[2], argv[2]);
    for (int i = 0; i < fileList.size(); i++)
    {
        cout << "Processing: " << fileList[i] << endl;
        fprintf(fp, "//--%s--\n",fileList[i].c_str());
        //fprintf(fp, "       Thread.sleep(%d);\n", 100 * (S2I(fileList[i])-sleepticks));
        fprintf(fp, "timeout %d\n", 100 * (S2I(fileList[i]) - sleepticks));
        FILE* fp1 = fopen(fileList[i].c_str(), "r");
        if (fp1 == NULL) {//若打开⽂件失败则退出
            puts("Error opening file！");
            return 1;
        }
        char c[100000];
        while (1)
        {
            if (feof(fp1)) {
                
                fclose(fp1);
                break;
            }
            fgets(c, 10000, fp1);//从输⼊⽂件读取⼀⾏字符串
            if (!Sstrstr(c, "playsound")) {
                continue;
            }
            std::vector<std::string> rl;
            strread(c, " ", rl);
            fprintf(fp, "%s %s %s %s %s\n", rl[0].c_str(), rl[1].c_str(), rl[2].c_str(), rl[7].c_str(),rl[8].c_str());
        }
        sleepticks = S2I(fileList[i]);
    }
    fclose(fp);
    return 0;
}