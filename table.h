#include <list>
#include "record.h"
#include <iostream>
#include <stdexcept>
#include <stdexcept>


class Table{

    private:

    unsigned int max_id;
    std::list<std::unique_ptr<Record>> records; 
    std::vector<std::string> col_names;
    std::vector<std::string> col_types;
    static const std::list<std::string> ALLOWED_TYPES;

    //helper functions:>
    bool is_allowed(std::string type);
    int get_col_index(std::string colname);

    public:

        //basic table managing functions
        Table();
        void add_col(std::string name, std::string type);
        void add_cols(std::vector<std::string> col_names_and_types);
        void delete_col(std::string colname);
        bool operator==(const Table &rhs);

        //descriptive functions:
        void print() const;
        void describe() const;


        //getters:
        std::vector<std::string> get_cols() const {return col_names;};
        std::vector<std::string> get_coltypes()const {return col_types;};
        unsigned int get_col_num() const {return col_names.size();}
        unsigned int get_row_num() const {return records.size();}


        //record management:
        void delete_record(const std::string & colname, const Data & d);
        void clear_records();
        void add_record(std::unique_ptr<Record> rec); 
        //void add_record(std::unique_ptr<Record> rec);


        //advanced table stuff:
        void truncate();
        Table find(const std::string & colname, const Data & d);


        /* template<typename... args>  //https://stackoverflow.com/questions/29972563/how-to-construct-a-vector-with-unique-pointers
        void add_record(args&&...d){
            auto size = sizeof...(d);
            if(size != col_num-1)
                throw "Number of arguments( TBD ) does not match number of columns. Record not added.";

            auto rec = std::make_unique<Record>();
            rec->add_data(std::make_unique<Int>(max_id+1));
            max_id++;
            //rec->print();
            //for(auto i = 0; i < size; i++ ){
                //std::string type = col_types.at(i); //TBD Vyjimka pro spatny typ
                //if(d.at(i)->type() != type) {} TBD


            using discard = std::unique_ptr<Data>[];

            (void)discard{ nullptr, ((
                rec->add_data(std::move(std::forward<args>(d)))
            ),void() ,nullptr )...};

            records.push_back(std::move(rec));
        } */

        template<typename... args>  //https://stackoverflow.com/questions/29972563/how-to-construct-a-vector-with-unique-pointers
        void add_record(args&&...d){
            auto size = sizeof...(d);
            if(size != get_col_num()-1)
                throw std::invalid_argument("Number of arguments( TBD ) does not match number of columns. Record not added.");

            auto rec = std::make_unique<Record>();
            rec->add_data(std::make_unique<Int>(max_id+1));
            max_id++;
            //rec->print();
            //for(auto i = 0; i < size; i++ ){
                //std::string type = col_types.at(i); //TBD Vyjimka pro spatny typ
                //if(d.at(i)->type() != type) {} TBD


            using discard = std::unique_ptr<Data>[];
            // https://en.cppreference.com/w/cpp/language/fold
            // https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator
            (void)discard{ nullptr, ((
                /* args je v této chvíli jeden typ z variadic args..., 
                args(std::move(d)) volá move konstruktor typu */
                rec->add_data(std::move(std::unique_ptr<Data>(new args(std::move(d)))))
            ),void() ,nullptr )...};

            //records.push_back(std::move(rec));
            add_record(std::move(rec));
        }

        friend std::ostream& operator<<(std::ostream& os,const Table &t){
            os << "-" << std::endl;
            os << "TABLE" <<std::endl;
            for(auto i = 0; i < t.get_col_num(); i++){
                os << t.col_names.at(i)<< "   ";
            } 
            os << std::endl;
            for(auto i = t.records.begin(); i != t.records.end(); i++ ){
                if(*i)
                    {(*i)->print();}
                else
                    {os << "je tu null" << std::endl;} //muze se stat?
                os << std::endl;
            }
            os << "-" << std::endl;
            return os;
        };

 
};

const std::list<std::string> Table::ALLOWED_TYPES = {"Int", "String", "Bool"};

//2 functions to pass data arguments into functions -depracated
/* template<typename T,typename Arg>
std::unique_ptr<Data> put(Arg a){
    return std::unique_ptr<Data>(dynamic_cast<Data*>(new T(a)));
    }

template<typename T>
std::unique_ptr<Data> put(){
    return std::unique_ptr<Data>(dynamic_cast<Data*>(new T()));
    } */


/* bool operator==(const Table &lhs, const Table &rhs){
    if(lhs.get_col_num() != rhs.get_col_num() && lhs.get_row_num() != rhs.get_row_num()){return false;}
    for(auto i = 0; i < lhs.get_row_num(); i++){
        if(lhs.records.at(i) != rhs.records.at(i))
            return false;
    }
    return true;
}
 */

bool Table::operator==(const Table &rhs){
    if(this->get_col_num() != rhs.get_col_num() || this->get_row_num() != rhs.get_row_num()){return false;}
    auto it1 = this->records.cbegin();
    auto it2 = rhs.records.cbegin();
    for(; it1 != this->records.cend(); it1++, it2++){
        if(**it1 != **it2)
            return false;
    }
    return true;
};




//HELPER PRIVATE FUNCTIONS
bool Table::is_allowed(std::string type){
    for(const auto& t : ALLOWED_TYPES){
        if(type == t)
            return true;
    }
    return false;
}

// navic se da do teto funkce zahrnout i exception kdyz nenajde TBD 
int Table::get_col_index(std::string colname)
{
    // mozna by slo vyuzit std::find_if
    int index = -1;
    for(auto i = 0; i < get_col_num(); i++){
        if(col_names.at(i) == colname){
            index = i;
        }
    }
    return index;
}


//BASIC TABLE MANAGING FUNCTIONS
Table::Table():max_id(0){
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
}


//t.add_col("Vek", "Int", "Pohlavi", "Bool");
void Table::add_cols(std::vector<std::string> col_names_and_types){
    for(auto i = 0; i < col_names_and_types.size(); i = i+2){
        add_col(col_names_and_types.at(i), col_names_and_types.at(i+1));
    }
}

void Table::delete_col(std::string colname){
    
    //TBD kontrola jestli colname nepatri do primary key 
    int index = get_col_index(colname);
    if(index == -1){throw(std::invalid_argument("column to delete not found in the table"));} //TBD pridat do get_col_index
    for(auto &rec : records){
        rec->delete_data(index);
    }
    col_names.erase(col_names.begin() + index );
    col_types.erase(col_types.begin() + index );
};


//RECORDS MANAGEMENT:

//t.delete_record('jmeno', 'Petr');

void Table::delete_record(const std::string & colname, const Data & d){
    int index = get_col_index(colname);
    if(index == -1){throw std::invalid_argument("Colname to delete doesnt exist");};
    for(auto i = records.begin(); i != records.end(); i++){
        if(*(*i)->contents.at(index) == d){
            //(*i)->delete_self();
            i = records.erase(i);
            i--;
        }
    }
}


void Table::clear_records(){
    records.clear();
    max_id = 0;
    std::cout << "size after deleting table:" << records.size() <<std::endl; //helper print
}

void Table::add_record(std::unique_ptr<Record> rec){
    if(get_col_num() != rec->contents.size()) {throw "Number of arguments( TBD ) does not match number of columns. Record not added.";};
    for(auto i = 0; i < get_col_num(); i++){
        std::string coltype = col_types.at(i);
        if(rec->contents.at(i)->type() != coltype && rec->contents.at(i)->type() != "Blank"){
            std::cout << "column types doesnt match: " <<  col_types.at(i) << rec->contents.at(i)->type() <<std::endl;
            throw std::invalid_argument("Data doesnt match column types.");
        }
    }
    records.push_back(std::move(rec));
}


//DESCRIPTIVE FUNCTIONS:


void Table::print() const{ //pretty printing 
    std::cout << *this << std::endl;
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
        std::cout << "truncationg" << std::endl;
        rec->contents.at(0) = std::make_unique<Int>(i);
        i++;  
    }
}


Table Table::find(const std::string & colname, const Data & d){   //find returns table containing all rows that contain data d in column colname
    //getting index of column:
    int index = get_col_index(colname);
    if(index == -1){throw std::invalid_argument("Colname to find doesnt exist in the table.");};
    //search
    Table result;
    //copying col names
    result.col_names = col_names;
    result.col_types = col_types;

    for(const auto& rec : records){
        if(*rec->contents.at(index) == d){
            // invoke copy constructor of the matched record and push it into the filtered table
            result.records.push_back(std::unique_ptr<Record>(new Record(*rec)));
        }    
    }
    return result;
}