#include <iostream>
#include <xmmintrin.h>
#include <smmintrin.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib> 
#include <immintrin.h>

using namespace std;
bool check(char* buf, int idx) {
    int byte = idx / 8;
    int bit = idx % 8;
    char i8 = buf[byte];
    return (i8 & (1 << bit)) != 0;
}

void set(char* buf, int idx) {
    int byte = idx / 8;
    int bit = idx % 8;
    char i8 = buf[byte];
    i8 |= (1 << bit);
    buf[byte] = i8;
}
//
char** matrix_alloc(int row, int col, int align) {
    char** rows = (char**)malloc(row * sizeof(char*));
    for (int i = 0; i < row; i++) {
        rows[i] = (char*)_aligned_malloc(col * sizeof(char), 32);
    }
    return rows;
}


//SSE 的 C / C++ 编程
vector<unsigned int > change(vector<vector<unsigned int>>temp, int maxreal)
{
    char** ch = matrix_alloc(temp.size(), maxreal, 32);
    for (int i = 0; i < temp.size(); i++)
    {

        for (int j = 0; j < maxreal; j++)
        {

            ch[i][j] = 0;
        }

    }
    for (int i = 0; i < temp.size(); i++)
    {
        for (int j = 0; j < temp[i].size(); j++)
        {
            set(ch[i], temp[i][j]);
        }
    }
    char* p = new char[maxreal];
    p = ch[0];
    for (int i = 1; i < temp.size(); i++)
    {
        for (int j = 0; j < maxreal; j += 32)
        {
            __m256 t1, t2, t3;
            t1 = _mm256_loadu_ps((const float*)(p + j));
            t2 = _mm256_loadu_ps((const float*)(ch[i] + j));
            t3 = _mm256_and_ps(t1, t2);
            _mm256_store_ps((float*)(p + j), t3);
        }

    }
    vector<unsigned int >result;
    for (int i = 0; i < maxreal * 8; i++)
    {
        if (check(p, i))
        {
            result.push_back(i);
        }
    }

    return result;
}

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

// 用new申请一个char类型的二维数组
char** new2DCharArray(int row, int col) {
    char** p = new char* [row];
    for (int i = 0; i < row; i++) {
        p[i] = new char[col];
    }
    return p;
}




int main() {


    int maxtemp = 0;
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
            if (num > maxtemp)
            {
                maxtemp = num;
            }
            v.push_back(num);
        }
        container.push_back(v);
    }
    fin.close();
    //求位向量数组最大列数
    int max = 0;
    int a = maxtemp / 256;
    int b = maxtemp % 256;
    if (b == 0) { max = maxtemp; }
    else { max = 256 * (a + 1); }
    int maxreal = 0;
    maxreal = max / 8;
    //查询文件获取
    vector<vector<unsigned int>> query = get_query("D:\\ExpIndex_Query\\ExpQuery1");
    vector<vector<unsigned int>> end;
    for (int i = 0; i < query.size(); i++)
    {
        vector<vector<unsigned int>> temp;
        for (int j = 0; j < query[i].size(); j++)
        {
            temp.push_back(container[query[i][j]]);
        }
        end.push_back(change(temp, maxreal));


    }
    for (int i = 0; i < end.size(); i++)
    {
        if (end[i].empty()) {
            cout << "查询" << i << "为空";
        }
        for (int j = 0; j < end[i].size(); j++)
        {
            cout << end[i][j] << " ";
        }
        cout << endl;
    }

}

