
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;
//查看L1数组中是否有num这个值
bool del(unsigned int num, vector<unsigned int>L1)
{
    int count1 = L1.size();
    int flag = 0;
    for (int i = 0; i < count1; i++)
    {
        //if (count1 == L1[i])
        if(num == L1[i])
        {
            flag = 1;
            break;
        }
    }
    return flag;

};
//读取查询文件
vector<vector<unsigned int>> get_query(string file) {
    string line;
    ifstream fin(file);
    vector<vector<unsigned int>> query;
    while (getline(fin, line)) {
        istringstream ss(line);
        vector<unsigned int> vec;
        unsigned int x;
        while (ss >> x) {
            vec.push_back(x);
        }
        query.push_back(vec);
    }
    return query;
}
//对temp二维数组进行合并，最后返回合并后的一维数组L
vector<unsigned int>result(vector<vector<unsigned int>>temp)
{
    sort(temp.begin(),temp.end(), [](vector<unsigned int>& a, vector<unsigned int>& b) {
        return a.size() < b.size();
        });
    vector<unsigned int> L = temp[0];
   
    for (int i = 1; i < temp.size(); i++) // i - set
    {
        for (int j= 0; j < size(L); j++) // L l1
        {
            int flag = del(L[j], temp[i]);
            if (!flag) 
            {
                L.erase(L.begin() + j);
                j--;
            }
        }
    
    }
    return L;
}
int main() {

    
    //读取索引文件
    ifstream fin("D:\\ExpIndex_Query\\ExpIndex", ios::binary);
    vector<vector<unsigned int>> container(1);
    while (!fin.eof()) {
        unsigned int length = 0;
        fin.read((char*)&length, sizeof(length));
        if (length == 0) {
            break;
        }
        vector<unsigned int> v;
        for (int i = 0; i < length; i++) {
            unsigned int num = 0;
            fin.read((char*)&num, sizeof(num));
            v.push_back(num);
        }
        container.push_back(v);
    }
    fin.close();
    
   //读取查询文件
    vector<vector<unsigned int>> query = get_query("D:\\ExpIndex_Query\\ExpQuery1");
    //end为最终输出的交集
    vector<vector<unsigned int>> end;
    for (int i = 0; i < query.size(); i++) 
    {
        //temp是一个二维容器，存储了一行查询记录中每一个元素对应的索引
        vector<vector<unsigned int>> temp;
        for (int j = 0; j < query[i].size(); j++) 
        {
            temp.push_back(container[query[i][j]]);
        }
       end.push_back( result(temp));

    
    }
    for (int i = 0; i <end.size(); i++) 
    {
        if (end[i].empty()) {
            cout << "查询" << i << "为空";
        }
        for (int j = 0; j < end[i].size(); j++) 
        {
            cout << end[i][j] << " ";
        }
        cout << endl;
        cout << endl;
    }
    //system("pause");
    return 0;
   
}




