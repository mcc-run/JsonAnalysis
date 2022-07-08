#include "Json.h"



string Json::parseJson(json &ele,string data)
{
	
	data = skip(data);
	if (data.size() == 0) {
		ele.type = DataType::Error;
		ele.StringValue = data;
		return "";
	}
	if (data.find("false") == 0 || data.find("true") == 0)
	{
		ele.type = DataType::Bool;
		if (data.find("true") == 0)
		{
			ele.BoolValue = true;
			return data.substr(4);
		}
		return data.substr(5);
	}
	else if (data.size() >= 4 && tolow(data.substr(0, 4)) == "null") {
		ele.type = DataType::Null;
		return data.substr(4);
	}
	else if (data[0] == '-' || data[0] >= '0' && data[0] <= '9') {
		return parseNumber(ele, data);
	}
	else if (data[0] == '\"') {
		ele.type = DataType::String;
		return parseString(ele, data);
	}
	else if (data[0] == '[') {
		ele.type = DataType::Array;
		return parseArray(ele, data);
	}
	else if (data[0] == '{') {
		ele.type = DataType::Object;
		return parseObject(ele, data);
	}
	ele.type = DataType::Error;
	ele.StringValue = data;
	return "";
}



string Json::skip(string data)
{
	int start = 0;
	while (start < data.size() && data[start] <= 32)start++;
	return data.substr(start);
}




string Json::tolow(string data)
{
	string str = data;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] >= 'A' && str[i] <= 'Z')str[i] += 'a' - 'A';
	}
	return str;
}

int Json::printJson(json& ele, string pre, string preObj, bool newline)
{
	if (ele.type == DataType::Object) {
		printObject(ele, preObj, newline);
		return 100;
	}
	else if (ele.type == DataType::Array) {
		printArray(ele, preObj, newline);
		return 100;
	}
	else {
		cout << pre;
		if (ele.type == DataType::Bool) {
			cout << (ele.BoolValue ? "true" : "false");
			return (ele.BoolValue ? 4 : 5);
		}
		else if (ele.type == DataType::Double) {
			cout << ele.DoubleValue;
			return to_string(ele.DoubleValue).size();
		}
		else if (ele.type == DataType::Error) {
			cout << "error";
			return 5;
		}
		else if (ele.type == DataType::Int) {
			cout << ele.IntValue;
			return to_string(ele.IntValue).size();
		}
		else if (ele.type == DataType::Null) {
			cout << "null";
			return 4;
		}
		else if (ele.type == DataType::String) {
			cout << ele.StringValue;
			return ele.StringValue.size();
		}
	}
	return 0;
}

void Json::printArray(json& ele, string pre, bool newline)
{
	if (ele.type != DataType::Array)return;

	if (newline)cout << endl << pre << "[";
	else cout << "[";

	pre += space;
	

	json* node = ele.child;
	int leng = 0;
	while (node) {
		leng += printJson(*node, "", pre, true);
		node = node->next;
		if (node)cout << ",";
		if (leng > 100 && node) { leng -= 100; cout << endl; }
	}
	pre = pre.substr(0, pre.size() - 4);
	cout << "]";
}

void Json::printObject(json& ele, string pre, bool newline)
{

	if (ele.type != DataType::Object)return;


	if (newline)cout << endl << pre << "{" << endl;
	else cout << "{" << endl;

	pre += space;

	json* node = ele.child;
	while (node) {
		json* left = node->child;
		printJson(*left, pre);
		cout << ":";
		json* right = left->next;
		string preObj = pre;
		int leng = left->StringValue.size();
		while (leng) { leng /= 4; preObj += space; }
		printJson(*right, "", preObj);
		node = node->next;
		if (node)cout << "," << endl;
	}
	pre = pre.substr(0, pre.size() - 4);
	cout << endl <<pre << "}";

}


string Json::parseNumber(json& ele, string data)
{
	//正负号、整数部分、小数点、小数、指数组成
	int symbol = 1;
	double number = 0;
	int esymbol = 1;
	int expoent = 0;

	int index = 0;

	//正负号
	if (data[index] == '-') { symbol = -1; index++; }

	//整数部分
	number = getInteger(data, index);

	//小数部分
	if (data[index] == '.') {
		index++;
		int start = index;
		double tmp = getInteger(data, index);
		int leng = index - start;
		while (leng--) { tmp /= 10; }
		number += tmp;
	}

	//指数部分
	if (data[index] == 'e') {
		index++;
		if (data[index] == '-') { esymbol = -1; index++; }
		expoent = getInteger(data, index);
	}

	number = symbol * number * pow(10, esymbol * expoent);
	if (fabs(number - (int)number) > DBL_EPSILON) {
		ele.type = DataType::Double;
		ele.DoubleValue = number;
	}
	else {
		ele.type = DataType::Int;
		ele.IntValue = number;
	}

	return data.substr(index);
}






int Json::getInteger(string data, int &index)
{
	int sum = 0;
	while (index < data.size() && data[index] <= '9' && data[index] >= '0')sum = sum * 10 + data[index++] - '0';
	return sum;
}

string Json::parseString(json& ele, string data)
{
	if (data[0] != '\"') {
		ele.type = DataType::Error;
		ele.StringValue = data;
		return "";
	}

	int len = 1;
	while (len <= data.size() && data[len] != '\"')len++;

	ele.StringValue = data.substr(0, len+1);

	return data.substr(len+1);
}

string Json::parseArray(json& ele, string data) {
	
	if (data[0] != '[') {
		ele.type = DataType::Error;
		ele.StringValue = data;
		return "";
	}

	data = data.substr(1);

	json *node = nullptr;
	while (data.size() > 0 && data[0] != ']') {
		if (ele.child == nullptr) {
			ele.child = new json;
			node = ele.child;
		}
		else {
			json *nnode = new json;
			node->next = nnode;
			node = nnode;
		}
		data = parseJson(*node, data);
		data = skip(data);
		if (data.size() > 0 && data[0] == ',')data = data.substr(1);
		data = skip(data);
	}
	
	if (data.size() == 0)return data;
	return data.substr(1);
}

string Json::parseObject(json& ele, string data) {

	if (data.size() == 0 || data[0] != '{') {
		ele.type = DataType::Error;
		ele.StringValue = data;
		return "";
	}
	data = data.substr(1);

	json* node = new json;
	while (data.size() > 0 && data[0] != '}') {
		json* left = new json;
		data = parseJson(*left, data);
		data = skip(data);
		if (data.size() > 0 && data[0] == ':')data = data.substr(1);
		json* right = new json;
		data = parseJson(*right, data);
		data = skip(data);
		if (data.size() > 0 && data[0] == ',')data = data.substr(1);
		data = skip(data);
		if (ele.child == nullptr) {
			ele.child = new json;
			node = ele.child;
		}
		else {
			json* nnode = new json;
			node->next = nnode;
			node = nnode;
		}
		node->child = left;
		left->next = right;
	}

	return data.substr(1);
}