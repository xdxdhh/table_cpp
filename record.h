#include <vector>
#include "data.h"
#include <memory>
#include <iostream>

class Record{
    //svoje sloupecky, ke kazdemu jeho typ(int, bool) a pak ke kazdemu jeho value();

    //kazdy record bude vektor pointeru na jednotliva data

    public:
    std::vector<std::unique_ptr<Data>> contents; //public or friend?
    unsigned int id;

    Record();
    ~Record();
    Record(const Record &other);
    unsigned int get_id ()const {return id;};
    void add_data(std::unique_ptr<Data> d = nullptr);
    void print() const;
    void delete_self();
    void set_id(const unsigned int _id){id = _id;};

};


Record::Record(){}; //allocates empty vector 



Record::Record(const Record &other){//copy constructor
    contents.reserve(other.contents.size());
    for(auto i = 0; i < other.contents.size(); i++){
        contents.push_back(other.contents.at(i)->clone());
    }
}


void Record::add_data(std::unique_ptr<Data> d){
    //if (d) TBD
    contents.push_back(std::move(d));
}

void Record::print() const{
    std::cout << this->get_id() << "     ";
    for(auto i = 0; i < contents.size(); i++){
        if (contents.at(i)){
        std::cout << contents.at(i)->to_str() << "     ";
        }
    }
}

Record::~Record(){
    this->delete_self();
};

void Record::delete_self(){
    /* for(auto& data : contents){
        //data.delete_self(); // neni treba, smazou se samy
        data = nullptr;
    } */
    contents.clear();
    
    //delete contents;
}