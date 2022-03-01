#include <string>
#include <cstddef>
#include <vector>
#include <memory>
#include <algorithm>

class Data
{
    public:
        virtual std::string type() const = 0 ;
        virtual std::string to_str() const = 0;
        virtual std::vector<std::byte> to_bytes() const = 0;
        virtual std::unique_ptr<Data> clone() const = 0;
        //virtual void delete_self();
        //virtual ~Data();
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


class Int : public Data{
    int _value;
    public:
        Int(int value) :_value(value){};
        std::string type() const {return "Int";};
        std::string to_str() const {return std::to_string(_value);}
        std::vector<std::byte> to_bytes() const {
            auto v = std::vector<std::byte>(sizeof(_value)); 
            std::copy(reinterpret_cast<const std::byte*>(&_value), reinterpret_cast<const std::byte*>(&_value) + sizeof(_value), v.data());
            return v; 
        }
        /* Int(const Int &other){
            _value = other._value;
        } */
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
        std::vector<std::byte> to_bytes() const {
            auto v = std::vector<std::byte>(_value.size()); 
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
        std::vector<std::byte> to_bytes() const {
            auto v = std::vector<std::byte>(1); 
            v.at(0) = static_cast<const std::byte>(_value);
            return v; 
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
        std::vector<std::byte> to_bytes() const {
            return std::vector<std::byte>();
        }
        //Blank(const Blank &other){}
        std::unique_ptr<Data> clone() const{
            return std::make_unique<Blank>();
        }
        
};