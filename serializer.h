#include "jsoncons/include/jsoncons/json.hpp"
#include "columns.h"
#include "record.h"
#include "data.h"
#include <fstream>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>
#include "byte_manager.h"


//167.27

using namespace jsoncons;

#ifndef SERIALIZER_H
#define SERIALIZER_H


using jsoncons::jsonpath::json_query;

class Serializer{
    private:
    json load_from_json(const std::string& filename);
    void save_into_json(const std::string& filename, const json& json_object);
    public:
    json serialize_data (const std::unique_ptr<Data>& data_class);
    std::unique_ptr<Data> deserialize_data(const std::string& filename);

    json serialize_record(const Record& record_class);
    void serialize_records(const std::list<std::unique_ptr<Record>>& record_list, const std::string& filename);

    void serialize_columns(const Columns& columns_class, const std::string& filename);
    Columns deserialize_columns(const std::string& filename);
    void serialize_table();
};


void Serializer::serialize_columns(const Columns& columns_class, const std::string& filename){ //parametr instance classy 
    json columns; //main json file
    columns.reserve(3);
    columns["allowed_types"] = columns_class.ALLOWED_TYPES;   //allowed_types asi nepotrebuju prenaset ?
    auto col_vec = columns_class.cols;
    columns["number_of_cols"] = col_vec.size();
    json all_columns(json_array_arg);
    all_columns.reserve(col_vec.size());

    for(const auto& col : col_vec){
        json column_json;
        column_json["name"] = col.name;
        column_json["type"] = col.type;
        column_json["primary_key_flag"] = col.primary_key_flag;
        all_columns.push_back(std::move(column_json));
    }
    columns["cols"] = all_columns;
    columns["foreign_keys"] = columns_class.foreign_keys;
    //std::cout << columns << std::endl;

    save_into_json("columns.json",columns);
    std::cout << "Column serialization finished." << std::endl;
};

void Serializer::save_into_json(const std::string& filename, const json& json_object){
    std::ofstream os(filename);
    os << pretty_print(json_object);
}

json Serializer::load_from_json(const std::string& filename){
    std::ifstream is(filename);
    json result_json;
    is >> result_json;
    return result_json;
}

Columns Serializer::deserialize_columns(const std::string& filename){ //chybi primary key, test foreign key

    json columns_json = load_from_json(filename);
    Columns final_column;

    std::list<std::string> allowed =  columns_json["allowed_types"].as<std::list<std::string>>();
    for(std::string& type : allowed) final_column.ALLOWED_TYPES.push_back(type); //jak nastavit ty static const atributy? udelat konstruktor ktery nastavuje allowed types ci?to asi nejde 
    std::map<std::string, std::string> fg_keys =  columns_json["foreign_keys"].as<std::map<std::string, std::string>>();
    final_column.foreign_keys = fg_keys;

    int number_of_cols = columns_json["number_of_cols"].as<int>();
    json cols = columns_json["cols"];
    for(auto i = 0; i < number_of_cols; i++){
        json col = json_query(cols, "$[" + std::to_string(i) + "]")[0];
        std::cout << col << std::endl;
        final_column.add_column(col["name"].as<std::string>(), col["type"].as<std::string>());
    }
    return final_column;
}

json Serializer::serialize_data(const std::unique_ptr<Data>& data_class){
    //std::vector<Data::byte> data_bytes = data_class->to_bytes();
    json data; // main data json
    data["type"] = data_class->type();
    data["data"] = json(byte_string_arg, data_class->to_bytes());
    return data;
}

std::unique_ptr<Data> Serializer::deserialize_data(const std::string& filename){
    //std::vector<Data::byte> data_bytes = data_class->to_bytes();
    json data_json = load_from_json(filename);
    Byte_Manager b;
    std::unique_ptr<Data> data = b.copy_from_bytes(data_json["type"].as<std::string>(), data_json["data"].as<std::vector<uint8_t>>());
    return data;
}

json Serializer::serialize_record(const Record& record_class){
    std::cout << "serializing record." << std::endl;
    json record;
    record["index"] = record_class.get_index();
    json all_datas(json_array_arg);
    all_datas.reserve(record_class.contents.size());
    for(auto i = 0; i < record_class.contents.size(); i++){
        json data = serialize_data(record_class.contents.at(i), "data" + std::to_string(i));
        all_datas.push_back(std::move(data));
    }
    record["data"] = all_datas;
    std::cout << all_datas << std::endl;
    //save_into_json(filename, record);
    return record;
}

void Serializer::serialize_records(const std::list<std::unique_ptr<Record>>& record_list, const std::string& filename){
    json records(json_array_arg); //final json file
    records.reserve(record_list.size());
    for(const auto& record : record_list){
        json rec = serialize_record(*record);
        records.push_back(std::move(rec));
    }
    save_into_json(filename, records);
}

/* void Serializer::serialize_table(){

}; mozna nepotrebne? mozna to bude manageovat table*/

#endif