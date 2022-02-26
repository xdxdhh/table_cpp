#include <vector>
#include "data.h"
#include <memory>
#include <iostream>

class Record{

    public:
    std::vector<std::unique_ptr<Data>> contents; //public or friend?

    //basic record management:
    Record() = default;
    ~Record() = default;
    Record(const Record &other);
    void delete_self();
    
    //data management:
    void add_data(std::unique_ptr<Data> d = nullptr);
    void delete_data(int index);

    //descriptive stuff:
    void print() const;

};


//BASIC RECORD MANAGEMENT:

Record::Record(const Record &other){//copy constructor
    contents.reserve(other.contents.size());
    for(auto i = 0; i < other.contents.size(); i++){
        contents.push_back(other.contents.at(i)->clone());
    }
}

void Record::delete_self(){
    contents.clear();
}

//DATA MANAGEMENT:

void Record::add_data(std::unique_ptr<Data> d){
    //if (d) TBD
    contents.push_back(std::move(d));
}

void Record::delete_data(int index){
    //kontrola jestli jsou data na tom indexu
    if(contents.size() > index){
        contents.erase(contents.begin() + index);
    }
};


//DESCRIPTIVE STUFF:

void Record::print() const{
    //std::cout << this->get_id() << "     ";
    for(auto i = 0; i < contents.size(); i++){
        std::cout << contents.at(i)->to_str() << "     ";
    }
}

