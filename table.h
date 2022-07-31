#include "record.h"
#include <iostream>
#include <functional>
#include "columns.h"
#include "serializer.h"

#ifndef TABLE_H
#define TABLE_H

class Table{

    private:
    Columns columns;
    std::string name;
    std::list<std::unique_ptr<Record>> records; 


    void loop_cols(Record& rec, std::function<void(const std::string&, Data&)> fn)
    {
        for(auto i = 0; i < rec.contents.size(); i++){
            std::string col = columns.cols.at(i).type;
            fn(col, *rec.contents.at(i));
        }
    }

    /* helper functions */
    std::list<unsigned int> get_index_list(); /* get all record indexes*/
    unsigned int get_max_index(); /* get max index */
    void deserialize(std::string filename);

    public:
        /* basic table managing functions */
        Table(std::string name, std::string arg = "table"); /* if arg="json" construct from json */
        void rename_table(std::string new_name){name = new_name;}; 
        void add_col(std::string name, std::string type);
        void add_cols(std::vector<std::string> col_names_and_types);
        void delete_col(std::string colname);
        void delete_cols(std::vector<std::string> col_names);
        void rename_col(std::string oldname, std::string newname){columns.rename_col(oldname,newname);};
        bool operator==(const Table &rhs);

        /* descriptive functions: */
        void print() const;
        void describe() const;

        /* getters: */
        std::vector<std::string> get_colnames() const {return columns.get_colnames();};
        std::vector<std::string> get_coltypes()const {return columns.get_coltypes();};
        unsigned int get_row_num() const {return records.size();}
        unsigned int get_col_num() const {return columns.get_colnum();}

        /* record management: */
        void delete_record(const std::string & colname, const Data & d);
        void clear_records();
        void add_record(std::unique_ptr<Record> rec); 

        /* serialization */
        void serialize(){
            Serializer s;
            s.serialize_table(name,columns,records,"table_" + name + ".json"); /* automatic filename if used didnt choose any*/
        };

        void serialize(std::string filename){
            Serializer s;
            s.serialize_table(name,columns,records,filename);
        };

        /* advanced table functions */
        void truncate(); /* re-arrange indexes so they form integer sequence starting with 1*/
        Table find(const std::string & colname, const Data & d);

        /* t.add_record(String("Jane"), Int(20), Bool(false)) ---- Adds one record to the table. If number of arguments doesnt match number of columns, std::invalid_argument is thrown.
         If nullptr is passed, std::invalid_argument is thrown. To pass null value, use Blank data type. If unknown Data type is added, std::invalid_argument is thrown. */
        template<typename... args>  //https://stackoverflow.com/questions/29972563/how-to-construct-a-vector-with-unique-pointers
        void add_record(args&&...d){
            auto size = sizeof...(d);
            if(size != columns.get_colnum())
                throw std::invalid_argument("Number of arguments( TBD ) does not match number of columns. Record not added.");

            auto rec = std::make_unique<Record>();
            rec->set_index(get_max_index() + 1);
            using discard = std::unique_ptr<Data>[];
            /* https://en.cppreference.com/w/cpp/language/fold
               https://en.cppreference.com/w/cpp/language/operator_other#Built-in_comma_operator */
            (void)discard{ nullptr, ((
                /* args je v této chvíli jeden typ z variadic args..., 
                args(std::move(d)) volá move konstruktor typu */
                rec->add_data(std::move(std::unique_ptr<Data>(new args(std::move(d)))))
            ),void() ,nullptr )...};

            add_record(std::move(rec)); //bad data type is checked here
        }

        /* https://stackoverflow.com/questions/21180346/variadic-template-unpacking-arguments-to-typename
           https://stackoverflow.com/questions/12515616/expression-contains-unexpanded-parameter-packs/12515637#12515637 */


        friend std::ostream& operator<<(std::ostream& os,const Table &t){
            os << "-----" << std::endl;
            os << "TABLE   " << t.name << std::endl;
            os << "index" << "   ";
            auto colnames = t.columns.get_colnames();
            for(const auto& colname : colnames){
                os << colname << "   ";
            }
            os << std::endl;
            int j = 0;
            for(auto i = t.records.begin(); i != t.records.end(); i++ ){
                std::cout << (*i)->get_index() << "   ";
                (*i)->print();
                os << std::endl;
            }
            os << "-----" << std::endl;
            return os;
        };
};


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


/* helper functions */
std::list<unsigned int> Table::get_index_list(){
    std::list<unsigned int> index_list;
    for(const auto& rec : records){
        index_list.push_back(rec->get_index());
    }
    return index_list;
};

unsigned int Table::get_max_index(){
    auto index_list = get_index_list();
    if(index_list.begin() == index_list.end()){
        return 0;
    }
    unsigned int max = *max_element(index_list.begin(), index_list.end());
    return max;
};


/* BASIC TABLE MANAGING FUNCTIONS */

Table::Table(std::string name, std::string arg):name(name){
    if(arg == "json"){
        Serializer s;
        deserialize(name);
    }
    std::cout << "Inicialization of table " << name << " succesful." << std::endl;
};

/* DESERIALIZATION */

void Table::deserialize(std::string filename){
    Serializer s;
    Columns final_columns = s.deserialize_columns(filename);
    this->columns = std::move(final_columns);
    record_list final_records = s.deserialize_records(filename);
    this->records = std::move(final_records);
    this->name = s.deserialize_table_name(filename);
    this->truncate(); /* reset indexes */
};

/* MANAGING COLUMNS*/

/* t.add_col("Name", "String") ---- Adds one column with given name and type. */
void Table::add_col(std::string name, std::string type){ 
    columns.add_column(name, type);
    for(auto& i : records){
        i->add_data();
    }
}

/* t.add_cols({"Age", "Int", "Gender", "Bool"}) ---- Adds multiple columns by using vector of names and types in name,type order. */
void Table::add_cols(std::vector<std::string> col_names_and_types){
    for(auto i = 0; i < col_names_and_types.size(); i = i+2){
        add_col(col_names_and_types.at(i), col_names_and_types.at(i+1));
    }
}

/* t.delete_col("age") ---- Deletes one column with given name. If the table does not contain given name std::invalid_argument is thrown. */
void Table::delete_col(std::string colname){
    int index = columns.get_col_index(colname);
    columns.delete_column(colname);
    for(auto &rec : records){
        rec->delete_data(index);
    }
    std::cout << "Column " << colname << " deleted." << std::endl;
};

/* t.delete_cols{"Age", "Name", "Gender"} ---- Delete multiple columns with specified names. If one of the names doesnt exist in the table, std::invalid_argument is thrown. */
void Table::delete_cols(std::vector<std::string> col_names){
    for(auto &col : col_names){
        delete_col(col);
    }
};


/* RECORDS MANAGEMENT: */

/* t.delete_record('column_name', 'value') ---- Delete all records which contains certain value in given column. If non-existing column name is specified, std::invalid_argument is thrown.*/
void Table::delete_record(const std::string & colname, const Data & d){
    int index = columns.get_col_index(colname);
    if(index == -1){throw std::invalid_argument("Colname to delete doesnt exist");};
    std::erase_if(records, [&](auto& rec){return *rec->contents.at(index) == d;});
}

/* t.clear_records() ---- Delete all records, keep columns. */
void Table::clear_records(){
    records.clear();
}


void Table::add_record(std::unique_ptr<Record> rec){
    if(columns.get_colnum() != rec->contents.size()) {throw "Number of arguments( TBD ) does not match number of columns. Record not added.";};
    for(const auto& r : rec->contents){
        if(r == nullptr){
            throw std::invalid_argument("Cannot add nullptr as data. Use Blank data type instead.");
        }
    }
    int blanks = 0;
    loop_cols(*rec, [&](const std::string& coltype, Data& data){
        if(data.type() != coltype && data.type() != "Blank"){
            std::cout << "column types doesnt match: " <<  coltype << data.type() <<std::endl;
            throw std::invalid_argument("Data doesnt match column types.");
        }
        if(data.type() == "Blank"){ /* check if whole record is Blank by counting blanks */
            blanks ++;
        }
    });
    if(blanks == columns.get_colnum()){
        throw std::invalid_argument("Cannot add column containing only Blank data type.");
    }
    records.push_back(std::move(rec));
}


/* DESCRIPTIVE FUNCTIONS: */

/* pretty printing */
void Table::print() const{ 
    std::cout << *this << std::endl;
}

void Table::describe() const{
    std::cout << std::endl;
    std::cout << "-----" << std::endl;
    std::cout << "DESCRIBING TABLE " << name << std::endl; 
    std::cout << "number of cols:" << columns.get_colnum() << std::endl;
    std::cout << "number of records:" << get_row_num() << std::endl;
    std::cout << "columns: " ;

    auto colnames = columns.get_colnames();
    auto coltypes = columns.get_coltypes();
    for(auto i = 0; i < colnames.size(); i++){
        std::cout << colnames.at(i) << "(" << coltypes.at(i) << ")   ";
    }

    std::cout << std::endl;
    std::cout << "-----" << std::endl;
};



/* ADVANCED TABLE FUNCTIONS: */

void Table::truncate(){ 
    int i = 1;
    for(auto& rec : records){
        rec->set_index(i);
        i++;  
    }
}

/* find("column name", value) ---- returns a table which contains all records with value in a given column. Throws std::invalid_argument if the column does not exist.
Can return an empty table. */
Table Table::find(const std::string & colname, const Data & d){   /* find returns table containing all rows that contain data d in column colname */
    //getting index of column:
    int index = columns.get_col_index(colname);
    //search
    Table result("table");
    //copying col names and types
    for(const auto& col : columns.cols){
        result.add_col(col.name, col.type);
    }
    for(const auto& rec : records){
        if(*rec->contents.at(index) == d){
            // invoke copy constructor of the matched record and push it into the filtered table
            result.records.push_back(std::unique_ptr<Record>(new Record(*rec)));
        }    
    }
    return result;
}

#endif