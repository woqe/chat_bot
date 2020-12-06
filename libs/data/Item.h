//
// Created by puff on 13.06.2020.
//

#ifndef DATA_ITEM_H
#define DATA_ITEM_H

#define DOUBLE_PRECISION 32

#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;

class DBFile{
private:
    void to_begin_get(){
        file.seekg(ios_base::beg);
    }
    bool is_open;
    string filename;
    fstream file;
protected:
    bool truncate(){
        if(is_file_open())
            close();
        file.open(filename, ios_base::trunc | ios_base::out);
        close();
    }

    bool is_file_open(){ return is_open; }
    bool open(){
        if(!is_file_open()){
            file.open(filename, ios_base::out | ios_base::in | ios_base::app | ios_base::ate | ios_base::binary);
            is_open = true;
        }
        to_begin_get();
    }
    bool close(){
        file.close();
        is_open = false;
    }

    int write_string(const string& str){
        file << str;
        put_byte(0); //end of string
    }
    int write_int(long long num){ //int size = 8
        if(!is_file_open())
            throw runtime_error("file is not open in write");

        for(int i = 7; i >= 0; --i)
            file.put((num >> i * 8));
    }
    void write_double(double num){
        write_int(num * pow(10, DOUBLE_PRECISION));
    }
    void put_byte(int byte){
        file.put(byte);
    }

    char peek_byte(){
        return file.peek();
    }

    string read_string(){
        string str;
        while(file.peek() != 0)
            str += file.get();
        file.get();
        return str;
    }
    long long read_int(){
        long long num = 0;
        for(int i = 0; i < 8; ++i){
            num = num << 8;
            num |= file.get();
        }
        return num;
    }
    double read_double(){
        return double(read_int()) / pow(10, DOUBLE_PRECISION);
    }
public:
    DBFile(const string& _filename) : filename(_filename), is_open(false){}
    ~DBFile(){
        close();
    }
};

struct Base{
    Base() : to_delete(false){}
    int id;
    bool to_delete;
    void mark_delete(){
        to_delete = true;
    }
};

//struct UserTelegramInterface {
//
//};

struct Pet : Base{
    enum GENDER{
        MALE,
        FEMALE,
        UNDEFINED_GENDER,
    } gender;

    enum TYPE{
        UNDEFINED_TYPE,
        CAT,
        DOG,
    } type;

    enum DONATE{
        DONATE_YES,
        DONATE_NO
    } donate;

    enum PARASITE_VAKCINE{
        PARASITE_VAKCINE_YES,
        PARASITE_VAKCINE_NO,
        PARASITE_VAKCINE_UNDEFINED,

    } parasite_vakcine;

    enum VAKCINE{
        VAKCINE_YES,
        VAKCINE_NO,
        VAKCINE_UNDEFINED
    } vakcine;

    enum STERIL{
        STERIL_YES,
        STERIL_NO,
        STERIL_UNDEFINED,
    } steril;

    enum LOTOK{
        LOTOK_YES,
        LOTOK_NO,
        LOTOK_UNDEFINED,
    } lotok;

    string call_info;
    string bolezn;
    string character;
    string history;
    string photo_1;
    string photo_2;
    string photo_3;
    string name;
    string age;
    long long telegram_creator_id;
    string username;

    enum STATUS{
        EDIT,
        MODERATE,
        CLOSED,
    } status;

    Pet(STATUS _status = STATUS::EDIT, long long _telegram_creator_id = 0, const string& _age = "", const string& _name = "", const string& _photo_1 = "", const string& _photo_2 = "", const string& _photo_3 = "",
            const string& _bolezn = "", const string& _call_info = "", const string& _history = "", const string& _character = "", LOTOK _lotok = LOTOK_NO, STERIL _steril = STERIL_NO, GENDER _gender = MALE,
            TYPE _type = UNDEFINED_TYPE, DONATE _donate = DONATE_NO, PARASITE_VAKCINE _parasite_vakcine = PARASITE_VAKCINE_NO, VAKCINE _vakcine = VAKCINE_NO, const string& _username = ""){
        call_info = _call_info;
        status = _status;
        photo_1 = _photo_1;
        photo_2 = _photo_2;
        photo_3 = _photo_3;
        name = _name;
        history = _history;
        character = _character;
        type = _type;
        donate = _donate;
        parasite_vakcine = _parasite_vakcine;
        vakcine = _vakcine;
        age = _age;
        donate = _donate;
        gender = _gender;
        steril = _steril;
        lotok = _lotok;
        telegram_creator_id = _telegram_creator_id;
        username = _username;
    }
};

struct Transaction : Base{
    enum TYPE_OPERATION{
        DEPOSIT,
        WITHDRAW,
        WAIT,
        BILLING,
    } type_operation;
    Transaction(int _summ, long long _date, const string& _comment, const string& _account, int _account_type, long long _qiwi_id, TYPE_OPERATION _type_operation)
    : summ(_summ), date(_date), comment(_comment), account(_account), account_type(_account_type), qiwi_id(_qiwi_id), type_operation(_type_operation){
    }
    int summ;
    long long date;
    string comment;

    string account;
    int account_type;

    long long qiwi_id;
};

struct Message : Base{
    long long telegram_chat_id;
    int message_id;

    enum MESSAGE_TYPE{
        NULL_OPERATION = 0,
        ENTER_PHOTO,
        ENTER_NAME,
        ENTER_HISTORY,
        ENTER_BOLEZN,
        ENTER_CALL_INFO,
        ENTER_AGE,
        ENTER_CHARACTER,

    } operation;

    Message(long long _telegram_chat_id, int _message_id, MESSAGE_TYPE _operation = NULL_OPERATION)
    : telegram_chat_id(_telegram_chat_id), message_id(_message_id), operation(_operation){
    }
};

template <class T> class Elems : public vector<T>{
private:
    bool access;
public:
    Elems() : access(false){}
    T& operator[](int pos){
        access = true;
        return vector<T>::operator[](pos);
    }
    bool is_access() const{
        return access;
    }
    void unset_access(){
        access = false;
    }
    void set_access(){
        access = true;
    }

};

template <class T> class ContainerInterface{
protected:
    virtual bool save_all() = 0;
    Elems<T> elems;
public:
    T* add(const T& elem){
        T buff = elem;
        buff.id = get_last_id() + 1;
        elems.push_back(buff);
        return &elems[elems.size() - 1];
    }
    vector<T>* get_elems(){
        elems.set_access();
        return &elems;
    }

    int get_last_id(){
        if(elems.size())
            return elems[elems.size() - 1].id;
        return 0;
    }

    T* get_last(){
        if(elems.size())
            return &elems[elems.size() - 1];
        return nullptr;
    }
    void save(){
        if(elems.is_access())
            save_all();
        elems.unset_access();
    }
    virtual ~ContainerInterface(){}
};

class DBPet : private DBFile, public ContainerInterface<Pet>{
public:
    DBPet() : DBFile("dbfiles/pet.bin"){
        open();
        while(peek_byte() != EOF) {

            int id = read_int();

            auto type = (Pet::TYPE)read_int();
            auto lotok = (Pet::LOTOK)read_int();
            auto gender = (Pet::GENDER)read_int();
            auto vakcine = (Pet::VAKCINE)read_int();
            auto parasite_vakcine = (Pet::PARASITE_VAKCINE)read_int();
            auto donate = (Pet::DONATE)read_int();
            auto status = (Pet::STATUS)read_int();
            auto steril = (Pet::STERIL)read_int();

            long long telegram_owner = read_int();
            string age = read_string();

            string photo_1 = read_string();
            string photo_2 = read_string();
            string photo_3 = read_string();

            string history = read_string();
            string character = read_string();
            string bolezn = read_string();
            string call_info = read_string();
            string name = read_string();
            string username = read_string();

            elems.push_back(Pet(status, telegram_owner, age, name, photo_1, photo_2, photo_3, bolezn, call_info, history, character,
                    lotok, steril, gender, type, donate, parasite_vakcine, vakcine, username));
            elems[elems.size() - 1].id = id;
        }
        close();
    }

    bool save_all(){
        Elems<Pet> new_elems;

        truncate();
        open();
        for(auto it = elems.begin(); it != elems.end(); ++it){
            if(it->to_delete)
                continue;
            write_int(it->id);

            write_int(it->type);
            write_int(it->lotok);
            write_int(it->gender);
            write_int(it->vakcine);
            write_int(it->parasite_vakcine);
            write_int(it->donate);
            write_int(it->status);
            write_int(it->steril);

            write_int(it->telegram_creator_id);
            write_string(it->age);

            write_string(it->photo_1);
            write_string(it->photo_2);
            write_string(it->photo_3);

            write_string(it->history);
            write_string(it->character);
            write_string(it->bolezn);
            write_string(it->call_info);
            write_string(it->name);
            write_string(it->username);

            new_elems.push_back(*it);
        }
        close();
        elems = new_elems;
    }

    Pet* get_edit_from_owner(long long telegram_id){
        for(int i = elems.size(); i > 0; --i)
            if(!elems[i - 1].to_delete && elems[i - 1].status == Pet::STATUS::EDIT && elems[i - 1].telegram_creator_id == telegram_id)
                return &elems[i - 1];
        return nullptr;
    }
};

class DBTransaction : private DBFile, public ContainerInterface<Transaction>{
public:
    DBTransaction() : DBFile("dbfiles/transaction.bin"){
        open();
        while(peek_byte() != EOF) {
            int id = read_int();
            int summ = read_int();
            long long date = read_int();
            string comment = read_string();
            string account = read_string();
            int account_type = read_int();
            long long qiwi_id = read_int();
            Transaction::TYPE_OPERATION type = (Transaction::TYPE_OPERATION)read_int();

            elems.push_back(Transaction(summ, date, comment, account, account_type, qiwi_id, type));
            elems[elems.size() - 1].id = id;
        }
        close();
    }

    bool save_all(){
        Elems<Transaction> new_elems;

        truncate();
        open();
        for(auto it = elems.begin(); it != elems.end(); ++it){
            if(it->to_delete)
                continue;
            write_int(it->id);
            write_int(it->summ);
            write_string(it->comment);
            write_string(it->account);
            write_int(it->account_type);
            write_int(it->qiwi_id);
            write_int(it->type_operation);
            new_elems.push_back(*it);
        }
        close();
        elems = new_elems;
    }
};

class DBMessage : private DBFile, public ContainerInterface<Message>{
public:
    DBMessage() : DBFile("dbfiles/message.bin"){
        open();
        while(peek_byte() != EOF) {
            int id = read_int();
            long long telegram_chat_id = read_int();
            int message_id = read_int();
            Message::MESSAGE_TYPE op = (Message::MESSAGE_TYPE)read_int();

            elems.push_back(Message(telegram_chat_id, message_id, op));
            elems[elems.size() - 1].id = id;
        }
        close();
    }

    Message* get_last_service_message(long long chat_id){
        for(int i = elems.size(); i > 0; --i)
            if(!elems[i - 1].to_delete && elems[i - 1].telegram_chat_id == chat_id && elems[i - 1].operation)
                return &elems[i - 1];
        return nullptr;
    }

    bool save_all(){
        Elems<Message> new_elems;
        truncate();
        open();
        for(auto it = elems.begin(); it != elems.end(); ++it){
            if(it->to_delete)
                continue;
            write_int(it->id);
            write_int(it->telegram_chat_id);
            write_int(it->message_id);
            write_int(it->operation);
            new_elems.push_back(*it);
        }
        close();
        elems = new_elems;
    }

};

#endif //DATA_ITEM_H
