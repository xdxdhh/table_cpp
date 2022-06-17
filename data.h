#include <string>
#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>


#ifndef DATA_H
#define DATA_H


class Data
{
    public:
        using byte = uint8_t;

        virtual std::string type() const = 0;
        virtual std::string to_str() const = 0;
        virtual std::vector<Data::byte> to_bytes() const = 0;
        virtual std::unique_ptr<Data> clone() const = 0;
        

        template<typename Target>
        static Target copy_from_bytes(const std::vector<Data::byte>& to_copy) {
            Target t; 
            size_t i = 0;
            for(auto it = to_copy.begin(); it != to_copy.end() && i < sizeof(t); ++it, ++i){
                reinterpret_cast<Data::byte*>(&t)[i] = *it;
            }
            return t;
        }

        template<typename T>
        static std::vector<Data::byte> copy_to_bytes(const T * t) {
            auto v = std::vector<Data::byte>(sizeof(*t)); 
            std::copy(reinterpret_cast<const Data::byte*>(t),
                reinterpret_cast<const Data::byte*>(t) + sizeof(*t), v.data());
            return v; 
        }

};


bool operator==(const Data &lhs, const Data &rhs){
    auto lhs_vec = lhs.to_bytes();
    auto rhs_vec = rhs.to_bytes();
    if(rhs_vec.size() != lhs_vec.size())
        return false;
    for(auto i = 0; i < rhs_vec.size(); i++){
        if(rhs_vec.at(i) != lhs_vec.at(i))
            return false;
    }
    return true;
}


class Int : public Data {
    int _value;
    public:
        Int(int value) :_value(value){};
        Int() = default;
        std::string type() const {return "Int";};
        std::string to_str() const {return std::to_string(_value);}
        std::vector<Data::byte> to_bytes() const {return copy_to_bytes(&_value);}
        std::unique_ptr<Data> clone() const{
            return std::make_unique<Int>(*this);
        }
        int get_value() const {return _value;};
};

class String : public Data{
    std::string _value;
    public:
        String(std::string value) :_value(value){};
        String(const char *value) :_value(value){};
        std::string type() const {return "String";};
        std::string to_str() const {return _value;};
        std::vector<Data::byte> to_bytes() const {
            auto v = std::vector<Data::byte>(_value.size()); 
            std::copy(_value.begin(),_value.end(), reinterpret_cast<char*>(v.data()));
            return v; 
        }
        /* String(const String &other){
            _value = other._value;
        } */
        std::unique_ptr<Data> clone() const{
            return std::make_unique<String>(*this);
        }
};

class Bool : public Data{
    bool _value;
    public:
        Bool(bool value) :_value(value){};
        std::string type() const{return "Bool";};
        std::string to_str() const {return _value == false ? "0" : "1";};
        std::vector<Data::byte> to_bytes() const {
            return copy_to_bytes(&_value);
        }
        /* Bool(const Bool &other){
            _value = other._value;
        } */
        std::unique_ptr<Data> clone() const{
            return std::make_unique<Bool>(*this);
        }
};

class Blank : public Data{
    public:
        //Blank(){};
        std::string type() const{return "Blank";};
        std::string to_str() const {return "null";};
        std::vector<Data::byte> to_bytes() const {
            return std::vector<Data::byte>();
        }
        //Blank(const Blank &other){}
        std::unique_ptr<Data> clone() const{
            return std::make_unique<Blank>();
        }
        
};


#endif

/* 
std::map<std::string, std::function<std::unique_ptr<Data>(std::vector<Data::byte>)>> mapa;
mapa["Int"] = [](vector<Data::byte> v){return unique_ptr<Data>(new Int(byte_copy<int>(v)));};
mapa["Bool"] = [](vector<Data::byte> v){return unique_ptr<Data>(new Bool(byte_copy<bool>(v)));};
mapa["String"] = [](vector<Data::byte> v){return unique_ptr<Data>(new String(reinterpret_cast<char*>(v.data())));};
mapa["Blank"] = [](vector<Data::byte> v){return unique_ptr<Data>(new Blank());};  */