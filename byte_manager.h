#include "data.h"
#include <map>


#ifndef BYTE_MANAGER_H
#define BYTE_MANAGER_H

class Byte_Manager{

    std::map<std::string, std::function<std::unique_ptr<Data>(std::vector<Data::byte>)>> byte_function_map;

    public:
    Byte_Manager();
    std::unique_ptr<Data> copy_from_bytes(const std::string& type, const std::vector<Data::byte>& v);

};

Byte_Manager::Byte_Manager(){
    byte_function_map["Int"] = [](const std::vector<Data::byte>& v){return std::unique_ptr<Data>(new Int(Data::copy_from_bytes<int>(v)));};
    byte_function_map["Bool"] = [](const std::vector<Data::byte>& v){return std::unique_ptr<Data>(new Bool(Data::copy_from_bytes<bool>(v)));};
    byte_function_map["String"] = [](const std::vector<Data::byte>& v){return std::unique_ptr<Data>(new String(reinterpret_cast<const char*>(v.data()), v.size()));};
    byte_function_map["Blank"] = [](const std::vector<Data::byte>& v){return std::unique_ptr<Data>(new Blank());};
}

std::unique_ptr<Data> Byte_Manager::copy_from_bytes(const std::string& type, const std::vector<Data::byte>& v){
    return byte_function_map[type](v);
};


#endif