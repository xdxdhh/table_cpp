#include "jsoncons/include/jsoncons/json.hpp"
#include "columns.h"
#include <fstream>

using namespace jsoncons;

#ifndef SERIALIZER_H
#define SERIALIZER_H

class Serializer{
    public:
    void serialize_data();
    void serialize_record();
    void serialize_columns(const Columns& columns_class);
    void deserialize_columns();
    void serialize_table();
};


void Serializer::serialize_columns(const Columns& columns_class){ //parametr instance classy 
    json columns; //main json file
    columns.reserve(3);
    columns["allowed_types"] = columns_class.get_allowed_types();

    std::cout << columns << std::endl;
    auto col_vec = columns_class.get_cols();
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
    std::cout << columns << std::endl;

    std::ofstream os("columns.json");
    os << pretty_print(columns);
};


void Serializer::deserialize_columns(){
    std::ifstream is("columns.json");
    json columns;
    is >> columns;
}

#endif