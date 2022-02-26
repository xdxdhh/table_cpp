/* #include "table.h"

const std::list<std::string> Table::ALLOWED_TYPES = {"Int", "String", "Bool"};

bool Table::is_allowed(std::string type){
    for(const auto& t : ALLOWED_TYPES){
        if(type == t)
            return true;
    }
    return false;
};


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
    //kontrola not-allowed typu TBD
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
    auto coltypes = get_coltypes();
    std::cout << "Column types:" ;
    for(const auto& coltype : coltypes){
        std::cout << coltype <<", ";
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
            row_num--;
        }
    }
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



void Table::truncate(){ 
    int i = 1;
    for(auto& rec : records){
        rec->contents.at(0) = std::make_unique<Int>(i);
        i++;  
    }
};

void Table::clear_records(){
    records.clear();
    row_num = 0;
    max_id = 0;
    std::cout << "size after deleting table:" << records.size() <<std::endl; //helper print

};

/* void Table::add_record(std::unique_ptr<Record> rec){
    records.push_back(std::move(rec));
}; */
 */