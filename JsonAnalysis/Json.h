#pragma once
#include <string>
#include <iostream>

using namespace std;


enum class DataType {
	Int,
	Double,
	String,
	Null,
	Bool,
	Array,
	Object,
	Error
};

//定义了json数据类型
typedef struct json{
	json* pre = nullptr, * next = nullptr;
	json* child = nullptr;

	DataType type = DataType::Null;

	int IntValue = 0;
	double DoubleValue = 0;
	string StringValue = "";
	bool BoolValue = false;
};

//json工具类
class Json
{
public:
	//将字符串转换为json对象
	string parseJson(json& ele, string data);

	int printJson(json& ele, string pre,string preObj="", bool newline=false);

	void printArray(json& ele, string pre,bool newline = false);


	void printObject(json& ele, string pre, bool newline = false);

private:

	//跳过有效数据前的空格等无效数据
	string skip(string data);

	string tolow(string data);

	int getInteger(string data,int &index);

	string parseNumber(json& ele, string data);

	string parseString(json& ele,string data);

	string parseArray(json& ele, string data);

	string parseObject(json& ele, string data);
	
	string space = "    ";

};

