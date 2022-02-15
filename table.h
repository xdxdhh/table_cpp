#include <list>
#include "record.h"
#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <stdexcept>

//dictionary povolenych datovych typu TBD

class Table{
    

    unsigned int row_num;
    unsigned int col_num;
    unsigned int max_id;
    std::list<std::unique_ptr<Record>> records; 
    std::vector<std::string> col_names;
    std::vector<std::string> col_types;



    public:
        Table();
        void add(Record rec); 
        void add_col(std::string name, std::string type);
        unsigned int get_col_num() const {return col_num;}
        unsigned int get_row_num() const {return row_num;}
        std::vector<std::string> get_cols() const {return col_names;};

        void print() const;
        void delete_record(const std::string & colname, const Data & d);
        Table find(const std::string & colname, const Data & d);

        void describe() const;
        void truncate(); //TBD

        void add_record(std::unique_ptr<Record> rec);
        void add_record(Record &rec);

        template<typename... args>  //https://stackoverflow.com/questions/29972563/how-to-construct-a-vector-with-unique-pointers
        void add_record(args&&...d){
            try{
                auto size = sizeof...(d);
                if(size != col_num-1)
                    throw "Number of arguments( TBD ) does not match number of columns. Record not added.";

                auto rec = std::make_unique<Record>();
                rec->add_data(std::make_unique<Int>(max_id+1));
                max_id++;
                row_num++;
                //rec->print();
                //for(auto i = 0; i < size; i++ ){
                    //std::string type = col_types.at(i); //TBD Vyjimka pro spatny typ
                    //if(d.at(i)->type() != type) {} TBD


                    using discard = std::unique_ptr<Data>[];

                    (void)discard{ nullptr, ((
                        rec->add_data(std::move(std::forward<args>(d)))
                    ),void() ,nullptr )...};

                records.push_back(std::move(rec));}
            catch (const char* msg) {
                std::cerr << msg << std::endl;
            }

        } 
};

Table::Table():row_num(0), col_num(1), max_id(0){
    std::cout << "Inicialization of table succesful." << std::endl;
    col_names.push_back("id");
}

void Table::add_col(std::string name, std::string type){ 
    for(const auto& x : col_names){
        if(x == name)
            throw std::invalid_argument("Column of this name already exists.");
    }

    col_names.push_back(name);
    col_types.push_back(type);
    std::cout << "Column " << name << " added." << std::endl;
    for(auto& i : records){
        i->add_data();
    }
    col_num ++;
}

void Table::print() const{
    std::cout << "-" << std::endl;
    std::cout << "TABLE" <<std::endl;
    //std::cout << "id   ";
    for(auto i = 0; i < col_num; i++){
        std::cout << col_names.at(i)<< "   ";
    } 
    std::cout << std::endl;
    for(auto i = records.begin(); i != records.end(); i++ ){
        if(*i)
            {
                (*i)->print();
            }
        else
            {std::cout << "je tu null" << std::endl;}
        std::cout << std::endl;
    }
    std::cout << "-" << std::endl;
}

void Table::describe() const{
    std::cout << "-" << std::endl;
    std::cout << "DESCRIBE: Table " << std::endl; //TBD pridat jmeno table
    std::cout << "Number of cols:" << get_col_num() << std::endl;
    std::cout << "Number of records:" << get_row_num() << std::endl;
    auto cols = get_cols();
    std::cout << "Column names:" ;
    for(const auto& col : cols){
        std::cout << col <<", ";
    }
    std::cout << std::endl;
    std::cout << "-" << std::endl;
};



// /t.delete_record('jmeno', 'Petr');

void Table::delete_record(const std::string & colname, const Data & d){
    int index;
    for(auto i = 0; i<col_num; i++){
        if(col_names.at(i) == colname)
            index = i;
    }

    for(auto i = records.begin(); i != records.end(); i++){
        if(*(*i)->contents.at(index) == d){
            std::cout<< "mazu record " << std::endl;
            (*i)->delete_self();
            std::cout<< "mazu record " << std::endl;
            i = records.erase(i);
        }
    }
    row_num--;
}


//find returns table containing all rows that contain data d in column colname

Table Table::find(const std::string & colname, const Data & d){
    Table result;
    //copying col names
    result.col_num = col_num;
    result.col_names = col_names;
    //getting index of column:
    int index;
    for(auto i = 0; i<col_num; i++){
        if(col_names.at(i) == colname)
            index = i;
    }
    //search

    for(const auto& rec : records){
        if(*rec->contents.at(index) == d){
            // invoke copy constructor of the matched record and push it into the filtered table
            result.records.push_back(std::unique_ptr<Record>(new Record(*rec)));
        }    
    }
    return result;
}

/* void Table::add_record(std::unique_ptr<Record> rec){
    records.push_back(std::move(rec));
}; */


void Table::truncate(){
    int i = 1;
    for(auto& rec : records){
        //rec->set_id(i); TBD 
        i++;  
    }
};

 