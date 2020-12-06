//
// Created by ez on 07.11.2020.
//

#include "interface.h"

int Table::addCol(const string &name, TYPE type) {
    for(auto it = type_map.begin(); it != type_map.end(); ++it){
        if(name == it->first)
            return 0;
    }
    type_map.insert(pair<string, TYPE>(name, type));
    for(auto it = rows_vec.begin(); it != rows_vec.end(); ++it){
        switch (type) {
            case TYPE::INT: {
                auto it_map = int_row_map.find(*it);
                it_map->second.insert(pair<string, int>(name, 0));
                break;
            }
            case TYPE::LONG: {
                auto it_map = long_row_map.find(*it);
                it_map->second.insert(pair<string, long>(name, 0));
                break;
            }
            case TYPE::DOUBLE: {
                auto it_map = double_row_map.find(*it);
                it_map->second.insert(pair<string, double>(name, 0));
                break;
            }
            case TYPE::STRING: {
                auto it_map = string_row_map.find(*it);
                it_map->second.insert(pair<string, string>(name, ""));
                break;
            }
        }
    }
    return true;
}

void Table::removeCol(const string &col_name) {
    for (auto it = rows_vec.begin(); it != rows_vec.end(); ++it) {
        if(int_row_map.find(*it)->second.erase(col_name))
            continue;
        else if(long_row_map.find(*it)->second.erase(col_name))
            continue;
        else if(double_row_map.find(*it)->second.erase(col_name))
            continue;
        else if(string_row_map.find(*it)->second.erase(col_name))
            continue;
    }
    type_map.erase(col_name);
}

void Table::removeRow(int row_id) {
    int_row_map.erase(row_id);
    long_row_map.erase(row_id);
    double_row_map.erase(row_id);
    string_row_map.erase(row_id);
    for(auto it = rows_vec.begin(); it != rows_vec.end(); ++it)
        if(*it == row_id){
            rows_vec.erase(it);
            return;
        }
}

int Table::addRow(){
    int id = -1;
    if(rows_vec.size())
        for(auto it = rows_vec.begin(); it != rows_vec.end(); ++it)
            if(*it > id)
                id = *it;
    rows_vec.push_back(++id);

    int_row_map.insert(pair<int, map<string, int>>(id, map<string, int>()));
    long_row_map.insert(pair<int, map<string, long long>>(id, map<string, long long>()));
    double_row_map.insert(pair<int, map<string, double>>(id, map<string, double>()));
    string_row_map.insert(pair<int, map<string, string>>(id, map<string, string>()));

    for(auto it = type_map.begin(); it != type_map.end(); ++it){
        switch(it->second){
            case TYPE::INT:{
                auto it_map = int_row_map.find(id);
                it_map->second.insert(pair<string, int>(it->first, 0));
                break;
            }
            case TYPE::LONG:{
                auto it_map = long_row_map.find(id);
                it_map->second.insert(pair<string, long>(it->first, 0));
                break;
            }
            case TYPE::DOUBLE:{
                auto it_map = double_row_map.find(id);
                it_map->second.insert(pair<string, double>(it->first, 0));
                break;
            }
            case TYPE::STRING:{
                auto it_map = string_row_map.find(id);
                it_map->second.insert(pair<string, string>(it->first, ""));
                break;
            }
        }
    }
    return id;
}

void Table::writeInt(int row_id, const string &col_name, int value) {
    auto it = int_row_map.find(row_id);
    if(it != int_row_map.end()) {
        auto it_inner = it->second.find(col_name);
        if(it_inner != it->second.end())
            it_inner->second = value;
    }
}

int Table::readInt(int row_id, const string &col_name) {
    auto it = int_row_map.find(row_id);
    if(it != int_row_map.end()) {
        auto it_inner = it->second.find(col_name);
        if(it_inner != it->second.end())
            return it_inner->second;
    }
    return 0;
}

void Table::writeLong(int row_id, const string &col_name, long long value) {
    auto it = long_row_map.find(row_id);
    if(it != long_row_map.end()) {
        auto it_inner = it->second.find(col_name);
        if(it_inner != it->second.end())
            it_inner->second = value;
    }
}

long long Table::readLong(int row_id, const string &col_name) {
    auto it = long_row_map.find(row_id);
    if(it != long_row_map.end()) {
        auto it_inner = it->second.find(col_name);
        if(it_inner != it->second.end())
            return it_inner->second;
    }
    return 0;
}

void Table::writeDouble(int row_id, const string &col_name, double value) {
    auto it = double_row_map.find(row_id);
    if(it != double_row_map.end()) {
        auto it_inner = it->second.find(col_name);
        if(it_inner != it->second.end())
            it_inner->second = value;
    }
}

double Table::readDouble(int row_id, const string &col_name) {
    auto it = double_row_map.find(row_id);
    if(it != double_row_map.end()) {
        auto it_inner = it->second.find(col_name);
        if(it_inner != it->second.end())
            return it_inner->second;
    }
    return 0;
}

void Table::writeString(int row_id, const string &col_name, const string& value) {
    auto it = string_row_map.find(row_id);
    if(it != string_row_map.end()) {
        auto it_inner = it->second.find(col_name);
        if(it_inner != it->second.end())
            it_inner->second = value;
    }
}

string Table::readString(int row_id, const string &col_name) {
    auto it = string_row_map.find(row_id);
    if(it != string_row_map.end()) {
        auto it_inner = it->second.find(col_name);
        if(it_inner != it->second.end())
            return it_inner->second;
    }
    return "";
}

void Table::save(){
    file.open("./table/" + table_name, ios_base::trunc | ios_base::out);
    file.close();
    file.open("./table/" + table_name, ios_base::out | ios_base::in | ios_base::app | ios_base::binary);

    int cols = type_map.size(), rows = rows_vec.size();

    file.put(rows >> 0);
    file.put(rows >> 8);
    file.put(rows >> 16);
    file.put(rows >> 24);

    file.put(cols >> 0);
    file.put(cols >> 8);
    file.put(cols >> 16);
    file.put(cols >> 24);

    for(auto it = rows_vec.begin(); it != rows_vec.end(); ++it){
        file.put((*it) >> 0);
        file.put((*it) >> 8);
        file.put((*it) >> 16);
        file.put((*it) >> 24);
    }

    for(auto it = type_map.begin(); it != type_map.end(); ++it){
        file.put(static_cast<char>(it->second));
        file << it->first;
        file.put(0);
        file.put(0);
        for(auto jt = rows_vec.begin(); jt != rows_vec.end(); ++jt){
            switch (it->second){
                case TYPE::INT:{
                    auto val = readInt(*jt, it->first);
                    file.put(val >> 0);
                    file.put(val >> 8);
                    file.put(val >> 16);
                    file.put(val >> 24);
                    break;
                }
                case TYPE::LONG:{
                    auto val = readLong(*jt, it->first);
                    file.put(val >> 0);
                    file.put(val >> 8);
                    file.put(val >> 16);
                    file.put(val >> 24);
                    file.put(val >> 32);
                    file.put(val >> 40);
                    file.put(val >> 48);
                    file.put(val >> 56);
                    break;
                }
                case TYPE::DOUBLE:{
                    auto tem_valval = readDouble(*jt, it->first);
                    long long val = tem_valval * 100000000;
                    file.put(val >> 0);
                    file.put(val >> 8);
                    file.put(val >> 16);
                    file.put(val >> 24);
                    file.put(val >> 32);
                    file.put(val >> 40);
                    file.put(val >> 48);
                    file.put(val >> 56);
                    break;
                }
                case TYPE::STRING:{
                    file << readString(*jt, it->first);
                    file.put(0);
                    file.put(0);
                    break;
                }
                
            }
        }
    }
    file.close();
}

Table::Table(const string &_table_name) : table_name(_table_name){
    file.open("./table/" + table_name, ios_base::out | ios_base::in | ios_base::app | ios_base::binary);
    if(!file.is_open())
        throw runtime_error("file " + _table_name + " is not open");
    if(file.peek() == -1)
        return;

    int rows = 0, cols = 0;
    rows |= file.get() << 0;
    rows |= file.get() << 8;
    rows |= file.get() << 16;
    rows |= file.get() << 24;

    cols |= file.get() << 0;
    cols |= file.get() << 8;
    cols |= file.get() << 16;
    cols |= file.get() << 24;

    for(int i = 0; i < rows; ++i){
        int row_id = 0;
        row_id |= file.get() << 0;
        row_id |= file.get() << 8;
        row_id |= file.get() << 16;
        row_id |= file.get() << 24;
        rows_vec.push_back(row_id);

        int_row_map.insert(pair<int, map<string, int>>(row_id, map<string, int>()));
        long_row_map.insert(pair<int, map<string, long long>>(row_id, map<string, long long>()));
        double_row_map.insert(pair<int, map<string, double>>(row_id, map<string, double>()));
        string_row_map.insert(pair<int, map<string, string>>(row_id, map<string, string>()));
    }

    for(int i = 0; i < cols; ++i) {
        TYPE type = static_cast<TYPE>(file.get());
        string col_name;
        while (file.peek() != -1) {
            if (file.peek() == 0) {
                int save_pos = file.tellg();
                file.get();
                if (file.peek() == 0 || file.peek() == -1) {
                    if (file.peek() == 0)
                        file.get();
                    break;
                }
                file.seekg(save_pos);
            }
            col_name += file.get();
        }
        type_map.insert(pair<string, TYPE>(col_name, type));
        for (int j = 0; j < rows; ++j) {
            switch (type) {
                case TYPE::INT:{
                    int val = 0;
                    val |= file.get() << 0;
                    val |= file.get() << 8;
                    val |= file.get() << 16;
                    val |= file.get() << 24;
                    auto it_map = int_row_map.find(rows_vec[j]);
                    it_map->second.insert(pair<string, int>(col_name, val));
                    break;
                }
                case TYPE::LONG:{
                    long long val = 0;
                    val |= file.get() << 0;
                    val |= file.get() << 8;
                    val |= file.get() << 16;
                    val |= file.get() << 24;
                    val |= (long long)file.get() << 32;
                    val |= (long long)file.get() << 40;
                    val |= (long long)file.get() << 48;
                    val |= (long long)file.get() << 56;

                    auto it_map = long_row_map.find(rows_vec[j]);
                    it_map->second.insert(pair<string, long long>(col_name, val));
                    break;
                }
                case TYPE::DOUBLE:{
                    long long temp_val = 0;
                    temp_val |= file.get() << 0;
                    temp_val |= file.get() << 8;
                    temp_val |= file.get() << 16;
                    temp_val |= file.get() << 24;
                    temp_val |= (long long)file.get() << 32;
                    temp_val |= (long long)file.get() << 40;
                    temp_val |= (long long)file.get() << 48;
                    temp_val |= (long long)file.get() << 56;

                    double val = temp_val / 100000000.0;

                    auto it_map = double_row_map.find(rows_vec[j]);
                    it_map->second.insert(pair<string, double>(col_name, val));
                    break;
                }
                case TYPE::STRING:{
                    string val;
                    while (file.peek() != -1) {
                        if (file.peek() == 0) {
                            int save_pos = file.tellg();
                            file.get();
                            if (file.peek() == 0 || file.peek() == -1) {
                                if (file.peek() == 0)
                                    file.get();
                                break;
                            }
                            file.seekg(save_pos);
                        }
                        val += file.get();
                    }
                    auto it_map = string_row_map.find(rows_vec[j]);
                    it_map->second.insert(pair<string, string>(col_name, val));
                    break;
                }
            }
        }
    }

    file.close();
}