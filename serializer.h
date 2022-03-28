#include "jsoncons/include/jsoncons/json.hpp"
#include "columns.h"
#include "record.h"
#include "data.h"
#include <fstream>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>
#include "byte_manager.h"

using namespace jsoncons;

#ifndef SERIALIZER_H
#define SERIALIZER_H


using jsoncons::jsonpath::json_query;

class Serializer{
    private:
    json load_from_json(const std::string& filename);
    void save_into_json(const std::string& filename, const json& json_object);
    public:
    void serialize_data (const std::unique_ptr<Data>& data_class, const std::string& filename);
    std::unique_ptr<Data> deserialize_data(const std::string& filename);

    void serialize_record(const Record& record_class, const std::string& filename);

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

void Serializer::serialize_data(const std::unique_ptr<Data>& data_class, const std::string& filename){
    //std::vector<Data::byte> data_bytes = data_class->to_bytes();
    json data; // main data json
    data["type"] = data_class->type();
    data["data"] = json(byte_string_arg, data_class->to_bytes());
    save_into_json(filename, data);
}

std::unique_ptr<Data> Serializer::deserialize_data(const std::string& filename){
    //std::vector<Data::byte> data_bytes = data_class->to_bytes();
    json data_json = load_from_json(filename);
    Byte_Manager b;
    std::unique_ptr<Data> data = b.copy_from_bytes(data_json["type"].as<std::string>(), data_json["data"].as<std::vector<uint8_t>>());
    return data;
}

void Serializer::serialize_record(const Record& record_class, const std::string& filename){
    for(auto i = 0; i < record_class.contents.size(); i++){
        serialize_data(record_class.contents.at(i), "hah");
    }
}


/* void Serializer::serialize_table(){

}; mozna nepotrebne? mozna to bude manageovat table*/

#endif