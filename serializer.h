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
using record_list = std::list<std::unique_ptr<Record>>;

class Serializer{
    private:
    /* helper functions */
    json load_from_json(const std::string& filename); 
    void save_into_json(const std::string& filename, const json& json_object); 

    public:

    json serialize_data (const std::unique_ptr<Data>& data_class);
    std::unique_ptr<Data> deserialize_data(const std::string& filename);
    std::unique_ptr<Data> deserialize_data(const json& data_json);

    json serialize_record(const Record& record_class);
    json serialize_records(const record_list& record_list);
    Record deserialize_record(json record_json);
    record_list deserialize_records(std::string filename);

    json serialize_columns(const Columns& columns_class);
    Columns deserialize_columns(std::string filename);

    void serialize_table(const std::string name, const Columns& columns_class, const record_list& record_list, std::string filename);

    std::string deserialize_table_name(std::string filename);
};


/* helper functions */
void Serializer::save_into_json(const std::string& filename, const json& json_object){
    std::ofstream os(filename);
    os << pretty_print(json_object);
}

json Serializer::load_from_json(const std::string& filename){
    std::ifstream is("table_lidi.json");
    if(!is.is_open()){
       std::cout << "Opening json" << filename << "failed." << std::endl; 
    }
    json result_json = json::parse(is);
    return result_json;
}

/* columns serializer */

json Serializer::serialize_columns(const Columns& columns_class){ /* parametr instance classy */
    json columns; /* main json file */
    columns.reserve(3);
    auto col_vec = columns_class.cols;
    columns["number_of_cols"] = col_vec.size();
    json all_columns(json_array_arg);
    all_columns.reserve(col_vec.size());

    for(const auto& col : col_vec){
        json column_json;
        column_json["name"] = col.name;
        column_json["type"] = col.type;
        all_columns.push_back(std::move(column_json));
    }
    columns["cols"] = all_columns;
    return columns;
};


Columns Serializer::deserialize_columns(std::string filename){ 
    json table_json = load_from_json(filename);
    json columns_json = table_json["columns_class"]; /* get "columns_class" part of json */

    Columns final_columns; /* create columns instance*/
    int number_of_cols = columns_json["number_of_cols"].as<int>();
    json cols = columns_json["cols"]; /* get the part of json that contains columns*/
    for(auto i = 0; i < number_of_cols; i++){
        json col = json_query(cols, "$[" + std::to_string(i) + "]")[0]; /* query i-th member*/
        final_columns.add_column(col["name"].as<std::string>(), col["type"].as<std::string>());
    }
    return final_columns;
}

json Serializer::serialize_data(const std::unique_ptr<Data>& data_class){
    json data; /* main data json */
    data["type"] = data_class->type();
    data["data"] = json(byte_string_arg, data_class->to_bytes(), semantic_tag::base64); /* semantic tag must be specified so that .as<>() knowns how to decode */
    return data;
}


std::unique_ptr<Data> Serializer::deserialize_data(const json& data_json){ 
    Byte_Manager b; /* external class that can cope with bytes */
    std::unique_ptr<Data> data = b.copy_from_bytes(data_json["type"].as<std::string>(), data_json["data"].as<std::vector<uint8_t>>(byte_string_arg, semantic_tag::base64)); /* specify decoding semantic tag*/
    return data;
}


json Serializer::serialize_record(const Record& record_class){
    json record;
    record["num_of_data"] = record_class.contents.size();
    json all_datas(json_array_arg);
    all_datas.reserve(record_class.contents.size());
    for(auto i = 0; i < record_class.contents.size(); i++){
        json data = serialize_data(record_class.contents.at(i));//, "data" + std::to_string(i));
        all_datas.push_back(std::move(data));
    }
    record["data"] = all_datas;
    return record;
}

json Serializer::serialize_records(const record_list& record_list){
    json rec; /* final json file */
    rec["number_of_recs"] = record_list.size();

    json records(json_array_arg); 
    records.reserve(record_list.size());
    for(const auto& record : record_list){
        json rec = serialize_record(*record);
        records.push_back(std::move(rec));
    }
    rec["records"] = records;
    return rec;
}

Record Serializer::deserialize_record(json record_json){
    /* data are assembled here */
    Record rec;
    int data_count = record_json["num_of_data"].as<int>();
    json data_json = record_json["data"];

    for(auto i = 0; i < data_count; i++){
        json data = json_query(data_json, "$[" + std::to_string(i) + "]")[0]; /* get i-th element*/
        rec.add_data(deserialize_data(data)); /* add data to record */
    }
    return rec;
}

record_list Serializer::deserialize_records(std::string filename){ 
    json table_json = load_from_json(filename);

    json records_json = table_json["records_class"]; /* get records class*/
    record_list table_records;
    int number_of_recs = records_json["number_of_recs"].as<int>();
    json recs = records_json["records"];/* get records itselves */

    for(auto i = 0; i < number_of_recs; i++){
        json rec = json_query(recs, "$[" + std::to_string(i) + "]")[0]; /* get i-th record*/
        Record one_rec = deserialize_record(rec); /* deserialize it */
        table_records.push_back(std::make_unique<Record>(one_rec)); /* push it into table*/
    }
    
    return table_records;
}


void Serializer::serialize_table(const std::string name, const Columns& columns_class, const record_list& record_list, std::string filename){
    
    json table_json;
    table_json["name"] = name; /* copy name */
    table_json["columns_class"] = serialize_columns(columns_class);
    table_json["records_class"] = serialize_records(record_list);

    save_into_json(filename, table_json);
}




std::string Serializer::deserialize_table_name(std::string filename){ 
    json table_json = load_from_json(filename);
    return table_json["name"].as<std::string>();
}


#endif