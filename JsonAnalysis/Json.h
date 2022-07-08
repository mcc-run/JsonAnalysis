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

//������json��������
typedef struct json{
	json* pre = nullptr, * next = nullptr;
	json* child = nullptr;

	DataType type = DataType::Null;

	int IntValue = 0;
	double DoubleValue = 0;
	string StringValue = "";
	bool BoolValue = false;
};

//json������
class Json
{
public:
	//���ַ���ת��Ϊjson����
	string parseJson(json& ele, string data);

	int printJson(json& ele, string pre,string preObj="", bool newline=false);

	void printArray(json& ele, string pre,bool newline = false);


	void printObject(json& ele, string pre, bool newline = false);

private:

	//������Ч����ǰ�Ŀո����Ч����
	string skip(string data);

	string tolow(string data);

	int getInteger(string data,int &index);

	string parseNumber(json& ele, string data);

	string parseString(json& ele,string data);

	string parseArray(json& ele, string data);

	string parseObject(json& ele, string data);
	
	string space = "    ";

};

