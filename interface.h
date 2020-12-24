//
// Created by ez on 07.11.2020.
//

#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <bot.h>

using namespace std;

#ifndef PETBOT_INTERFACE_H
#define PETBOT_INTERFACE_H

class Bot : private TelegramBot{
    vector<Telegram::Update> updates;
public:
    Bot(const string& api_key) : TelegramBot(api_key){}
    int sendKeyboard(long long telegram_chat_id, const string& text,
                       const vector<vector<Telegram::InlineKeyboardButton>>& inline_buttons){
        json result = TelegramBot::sendInlineKeyboard(to_string(telegram_chat_id), text, inline_buttons);
        if(!result["result"]["message_id"].is_null())
            return int(result["result"]["message_id"]);
        return 0;
    }
    int sendMessage(long long telegram_chat_id, const string& text){
        json result = TelegramBot::sendMessage(to_string(telegram_chat_id), text);
        if(!result["result"]["message_id"].is_null())
            return int(result["result"]["message_id"]);
        return 0;
    }
    void deleteMessage(long long telegram_chat_id, int message_id){
        TelegramBot::deleteMessage(telegram_chat_id, message_id);
    }
    bool waitMessages(int wait_time){
        unique_ptr<vector<Telegram::Update>> ptr(getUpdates(wait_time));
        for(auto it = ptr->begin(); it != ptr->end(); ++it)
            updates.push_back(*it);
        return true;
    }
    vector<Telegram::Message> getMessages(){
        vector<Telegram::Message> temp;
        for(auto it = updates.begin(); it != updates.end(); ++it)
            temp.push_back(Telegram::Message(*it));
        updates.clear();
        return temp;
    }
};

class Table{
    map<int, map<string, int>> int_row_map;
    map<int, map<string, long long>> long_row_map;
    map<int, map<string, double>> double_row_map;
    map<int, map<string, string>> string_row_map;
    vector<int> rows_vec;
    fstream file;
    string table_name;
public:
    Table(const string& _table_name);
    enum TYPE{
        INT,
        LONG,
        DOUBLE,
        STRING,
    };
    int addRow();
    int addCol(const string& name, TYPE type);
    void removeRow(int row_id);
    void removeCol(const string& col_name);
    void save();
    void truncate(){

    }
    void clear();

    void writeInt(int row_id, const string& col_name, int value);
    void writeLong(int row_id, const string& col_name, long long value);
    void writeDouble(int row_id, const string& col_name, double value);
    void writeString(int row_id, const string& col_name, const string& value);

    int readInt(int row_id, const string& col_name);
    long long readLong(int row_id, const string& col_name);
    double readDouble(int row_id, const string& col_name);
    string readString(int row_id, const string& col_name);
    vector<int> getIDs(){
        return rows_vec;
    }
    vector<int> getSortedByDecrease(const string& name, TYPE type);
    vector<int> getSortedByIncrease(const string& name, TYPE type);

private:
    struct Column{
        string col_name;
        TYPE type;
    };
    map<string, TYPE> type_map;
public:
    vector<Column> getCols();
};

#endif //PETBOT_INTERFACE_H
