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
    /* helper functions */
    json load_from_json(const std::string& filename); //ok
    void save_into_json(const std::string& filename, const json& json_object); //ok

    public:

    json serialize_data (const std::unique_ptr<Data>& data_class);
    std::unique_ptr<Data> deserialize_data(const std::string& filename);
    std::unique_ptr<Data> deserialize_data(json data_json);

    json serialize_record(const Record& record_class);
    void serialize_records(const std::list<std::unique_ptr<Record>>& record_list, const std::string& filename);
    Record deserialize_record(json record_json);
    std::list<std::unique_ptr<Record>> deserialize_records(std::string filename);

    void serialize_columns(const Columns& columns_class, const std::string& filename);
    Columns deserialize_columns(const std::string& filename);

    void serialize_table();
};


/* helper functions */
void Serializer::save_into_json(const std::string& filename, const json& json_object){
    std::ofstream os(filename);
    os << pretty_print(json_object);
}

json Serializer::load_from_json(const std::string& filename){
    std::ifstream is(filename);
    json result_json;
    std::cout << "here" << std::endl;
    is >> result_json;
    std::cout << "here2" << std::endl;
    return result_json;
}

/* columns serializer */

void Serializer::serialize_columns(const Columns& columns_class, const std::string& filename){ /* parametr instance classy */
    json columns; /* main json file */
    columns.reserve(4);
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

    save_into_json(filename,columns);
    std::cout << "Column serialization finished." << std::endl;
};


Columns Serializer::deserialize_columns(const std::string& filename){ //TBD chybi primary key, test foreign key 
    json columns_json = load_from_json(filename);
    Columns final_column;

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
    json data; /* main data json */
    data["type"] = data_class->type();
    data["data"] = json(byte_string_arg, data_class->to_bytes(), semantic_tag::base64); /* semantic tag must be specified so that .as<>() knowns how to decode */
    return data;
}



std::unique_ptr<Data> Serializer::deserialize_data(json data_json){ 
    Byte_Manager b;
    std::unique_ptr<Data> data = b.copy_from_bytes(data_json["type"].as<std::string>(), data_json["data"].as<std::vector<uint8_t>>(byte_string_arg, semantic_tag::base64));
    return data;
}


json Serializer::serialize_record(const Record& record_class){
    std::cout << "serializing record." << std::endl;
    json record;
    record["index"] = record_class.get_index();
    record["num_of_data"] = record_class.contents.size();
    json all_datas(json_array_arg);
    all_datas.reserve(record_class.contents.size());
    for(auto i = 0; i < record_class.contents.size(); i++){
        json data = serialize_data(record_class.contents.at(i));//, "data" + std::to_string(i));
        all_datas.push_back(std::move(data));
    }
    record["data"] = all_datas;
    std::cout << all_datas << std::endl;
    //save_into_json(filename, record);
    return record;
}

void Serializer::serialize_records(const std::list<std::unique_ptr<Record>>& record_list, const std::string& filename){

    json rec; /* final json file */
    rec["number_of_recs"] = record_list.size();

    json records(json_array_arg); 
    std::cout << "velikost recordu: " << record_list.size() << std::endl;
    records.reserve(record_list.size());
    for(const auto& record : record_list){
        json rec = serialize_record(*record);
        records.push_back(std::move(rec));
    }
    rec["records"] = records;
    save_into_json(filename, rec);
}

Record Serializer::deserialize_record(json record_json){
    //budou se muset radit podle indexu, to bude v deserialize_records
    //tady se jen poskladaji data
    Record rec;
    rec.set_index(record_json["index"].as<int>());
    std::cout << "index set." << std::endl;
    int data_count = record_json["num_of_data"].as<int>();

    json data_json = record_json["data"];
    for(auto i = 0; i < data_count; i++){
        json data = json_query(data_json, "$[0]")[0];
        //Record one_rec = deserialize_record(rec);
        //std::cout << rec << std::endl;
        std::cout << data << std::endl;
        rec.add_data(deserialize_data(data));
        std::cout << "data pushed." << std::endl;
    }
    return rec;
}

std::list<std::unique_ptr<Record>> Serializer::deserialize_records(std::string filename){
    json records = load_from_json(filename);
    std::list<std::unique_ptr<Record>> table_records;
    std::cout << "deserializing records" << records << std::endl;
    //json rec = json_query(records, "$[0]")[0];
    //std::cout << rec << std::endl;
    //std::cout << rec["data"] << std::endl;
    int number_of_recs = records["number_of_recs"].as<int>();
    std::cout << number_of_recs << std::endl;

    json recs = records["records"];
    for(auto i = 0; i < number_of_recs; i++){
        json rec = json_query(recs, "$[0]")[0];
        Record one_rec = deserialize_record(rec);
        table_records.push_back(std::make_unique<Record>(one_rec));
    }
    
    return table_records;
}

/* void Serializer::serialize_table(){

}; mozna nepotrebne? mozna to bude manageovat table*/

#endif