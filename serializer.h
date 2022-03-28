#include "jsoncons/include/jsoncons/json.hpp"
#include "columns.h"
#include <fstream>
#include <jsoncons_ext/jsonpath/jsonpath.hpp>

using namespace jsoncons;

#ifndef SERIALIZER_H
#define SERIALIZER_H


using jsoncons::jsonpath::json_query;

class Serializer{
    private:
    json load_from_json(std::string filename);
    public:
    void serialize_data();
    void serialize_record();
    void serialize_columns(const Columns& columns_class);
    Columns deserialize_columns();
    void serialize_table();
};


void Serializer::serialize_columns(const Columns& columns_class){ //parametr instance classy 
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

    std::ofstream os("columns.json");
    os << pretty_print(columns);
};


json Serializer::load_from_json(std::string filename){
    std::ifstream is(filename);
    json result_json;
    is >> result_json;
    return result_json;
}

Columns Serializer::deserialize_columns(){ //chybi primary key, test foreign key

    json columns_json = load_from_json("columns.json");
    Columns final_column;

    std::list<std::string> allowed =  columns_json["allowed_types"].as<std::list<std::string>>();
    for(std::string& type : allowed) final_column.ALLOWED_TYPES.push_back(type); //jak nastavit ty static const atributy? udelat konstruktor ktery nastavuje allowed types ci?to asi nejde 
    

    std::map<std::string, std::string> fg_keys =  columns_json["foreign_keys"].as<std::map<std::string, std::string>>();
    final_column.foreign_keys = fg_keys;


    int number_of_cols = columns_json["number_of_cols"].as<int>();
    std::cout << number_of_cols << std::endl;
    json cols = columns_json["cols"];
    
    for(auto i = 0; i < number_of_cols; i++){
        json col = json_query(cols, "$[" + std::to_string(i) + "]")[0];
        std::cout << col << std::endl;
        final_column.add_column(col["name"].as<std::string>(), col["type"].as<std::string>());
    }

    return final_column;


}

#endif