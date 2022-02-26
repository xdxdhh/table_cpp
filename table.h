#include <list>
#include "record.h"
#include <iostream>
#include <stdexcept>
#include <stdexcept>


class Table{

    private:

    unsigned int row_num; //delete
    unsigned int col_num; //delete 
    unsigned int max_id;
    std::list<std::unique_ptr<Record>> records; 
    std::vector<std::string> col_names;
    std::vector<std::string> col_types;
    static const std::list<std::string> ALLOWED_TYPES ;

    //helper functions:
    bool is_allowed(std::string type);

    public:

        //basic table managing functions
        Table();
        void add_col(std::string name, std::string type);
        void add_cols(std::vector<std::string> col_names_and_types);


        //descriptive functions:
        void print() const;
        void describe() const;


        //getters:
        std::vector<std::string> get_cols() const {return col_names;};
        std::vector<std::string> get_coltypes()const {return col_types;};
        unsigned int get_col_num() const {return col_num;}
        unsigned int get_row_num() const {return row_num;}


        //record management:
        void delete_record(const std::string & colname, const Data & d);
        void clear_records();
        //void add(Record rec);  possibly?
        //void add_record(std::unique_ptr<Record> rec);


        //advanced table stuff:
        void truncate();
        Table find(const std::string & colname, const Data & d);


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

const std::list<std::string> Table::ALLOWED_TYPES = {"Int", "String", "Bool"};

//2 functions to pass data arguments into functions
template<typename T,typename Arg>
std::unique_ptr<Data> put(Arg a){
    return std::unique_ptr<Data>(dynamic_cast<Data*>(new T(a)));
    }

template<typename T>
std::unique_ptr<Data> put(){
    return std::unique_ptr<Data>(dynamic_cast<Data*>(new T()));
    }


//HELPER PRIVATE FUNCTIONS
bool Table::is_allowed(std::string type){
    for(const auto& t : ALLOWED_TYPES){
        if(type == t)
            return true;
    }
    return false;
}


//BASIC TABLE MANAGING FUNCTIONS
Table::Table():row_num(0), col_num(1), max_id(0){
    std::cout << "Inicialization of table succesful." << std::endl;
    col_names.push_back("id");
    col_types.push_back("Int");
}


void Table::add_col(std::string name, std::string type){ 
    for(const auto& x : col_names){
        if(x == name)
            throw std::invalid_argument("Column of this name already exists.");
    }
    if(is_allowed(type) == false){
        throw std::invalid_argument("Invalid type name.");
    }
    col_names.push_back(name);
    col_types.push_back(type);
    std::cout << "Column " << name << " added." << std::endl;
    for(auto& i : records){
        i->add_data();
    }
    col_num ++;
}


//t.add_col("Vek", "Int", "Pohlavi", "Bool");
void Table::add_cols(std::vector<std::string> col_names_and_types){
    for(auto i = 0; i < col_names_and_types.size(); i = i+2){
        add_col(col_names_and_types.at(i), col_names_and_types.at(i+1));
    }
}


//RECORDS MANAGEMENT:

//t.delete_record('jmeno', 'Petr');

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
            row_num--;
        }
    }
}


void Table::clear_records(){
    records.clear();
    row_num = 0;
    max_id = 0;
    std::cout << "size after deleting table:" << records.size() <<std::endl; //helper print
}


//DESCRIPTIVE FUNCTIONS:


void Table::print() const{ //pretty printing 
    std::cout << "-" << std::endl;
    std::cout << "TABLE" <<std::endl;
    for(auto i = 0; i < col_num; i++){
        std::cout << col_names.at(i)<< "   ";
    } 
    std::cout << std::endl;
    for(auto i = records.begin(); i != records.end(); i++ ){
        if(*i)
            {(*i)->print();}
        else
            {std::cout << "je tu null" << std::endl;} //muze se stat?
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
    auto coltypes = get_coltypes();
    std::cout << "Column types:" ;
    for(const auto& coltype : coltypes){
        std::cout << coltype <<", ";
    }
    std::cout << std::endl;
    std::cout << "-" << std::endl;
};



//ADVANCED TABLE FUNCTIONS:

void Table::truncate(){ 
    int i = 1;
    for(auto& rec : records){
        rec->contents.at(0) = std::make_unique<Int>(i);
        i++;  
    }
}


Table Table::find(const std::string & colname, const Data & d){   //find returns table containing all rows that contain data d in column colname
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