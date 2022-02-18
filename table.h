#include <list>
#include "record.h"
#include <iostream>
#include <stdexcept>
#include <stdexcept>

//dictionary povolenych datovych typu TBD
//ALLOWED_TYPES std::list<std::string> = {"Int", "String", "Bool"}


class Table{
    

    unsigned int row_num;
    unsigned int col_num;
    unsigned int max_id;
    std::list<std::unique_ptr<Record>> records; 
    std::vector<std::string> col_names;
    std::vector<std::string> col_types;
    static const std::list<std::string> ALLOWED_TYPES ;//= {"Int", "String", "Bool"};
    bool is_allowed(std::string type);

    public:
        Table();
        void add(Record rec); 
        void add_col(std::string name, std::string type);
        void add_cols(std::vector<std::string> col_names_and_types);
        unsigned int get_col_num() const {return col_num;}
        unsigned int get_row_num() const {return row_num;}
        std::vector<std::string> get_cols() const {return col_names;};
        std::vector<std::string> get_coltypes()const {return col_types;};

        void print() const;
        void delete_record(const std::string & colname, const Data & d);
        Table find(const std::string & colname, const Data & d);

        void describe() const;
        void truncate(); //TBD

        void add_record(std::unique_ptr<Record> rec);
        void add_record(Record &rec);

        template<typename... args>  //https://stackoverflow.com/questions/29972563/how-to-construct-a-vector-with-unique-pointers
        void add_record(args&&...d){
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

                records.push_back(std::move(rec));
        }
 
};

