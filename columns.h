#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <list>


#ifndef COLUMNS_H
#define COLUMNS_H

class Columns{
    private:
        struct column{
            std::string name;
            std::string type;          
        };

        bool is_allowed(std::string type); /* check if this column type is allowed */
        void check_column_existence(std::string name, bool expected, std::string msg) const; /* check if column already exists and compare it to expected value*/
        bool colname_compare(std::string lhs, std::string rhs) const; /* compare 2 column names*/

    public:
        static std::list<std::string> ALLOWED_TYPES; /* list of allowed column types */
        std::vector<column> cols; /* main vector holding columns */

        /* working with columns */
        void add_column(std::string name, std::string type);
        void delete_column(std::string name); 
        void rename_col(std::string oldname, std::string newname);

        /* helpful getters */
        int get_colnum() const {return cols.size();};
        std::vector<std::string> get_colnames() const;
        std::vector<std::string> get_coltypes() const;
        int get_col_index(std::string colname) const; 
          
};

std::list<std::string> Columns::ALLOWED_TYPES = {"Int", "String", "Bool"}; /* Blank column cannot be created */


/* helper function for compare */
std::string str_to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c){ return std::tolower(c); }
        );
    return s;
}


bool Columns::is_allowed(std::string type){ /* checks if this coltype is allowed */
    for(const auto& t : ALLOWED_TYPES){
        if(type == t)
            return true;
    }
    return false;
}

/* checks if column exists , expected - true/false if we expect it to exist */
void Columns::check_column_existence(std::string name, bool expected, std::string msg) const{ //? bool, vracet true pokud existuje, false pokud neexistuje? mozna lepsi reseni nez expected 
    bool existence_flag = false;
    for(const auto& col : cols){ //? udelat z toho const_iterate_cols a iterate_cols?
        if(str_to_lower(col.name) == str_to_lower(name)){
            existence_flag = true;
        }
    }
    if(existence_flag != expected){
        throw std::invalid_argument("Column of name " + name + msg );
    }
};

/* compare 2 colnames */
bool Columns::colname_compare(std::string lhs, std::string rhs) const{
    if(str_to_lower(lhs) == str_to_lower(rhs))
        return true;
    return false;
}


void Columns::add_column(std::string name, std::string type){
    if(!is_allowed(type)){throw std::invalid_argument("Column of type " + type + " is not allowed in the table.");};
    check_column_existence(name, false, " already exists.");
    auto& new_col = cols.emplace_back();
    new_col.name = name;
    new_col.type = type;
    std::cout << "Column " << name << " added." <<std::endl;
}

void Columns::delete_column(std::string name){
    check_column_existence(name, true, " to delete was not found.");
    std::erase_if(cols, [&](auto const & col){return col.name == name;});
}

void Columns::rename_col(std::string oldname, std::string newname){
    check_column_existence(oldname, true, "doesnt exist. ");
    for(auto& col : cols){
        if(col.name == oldname){
            col.name = newname;
        }
    }
};


std::vector<std::string> Columns::get_colnames() const{
    std::vector<std::string> colnames;
    for(const auto& col : cols){
        colnames.push_back(col.name);
    }
    return colnames;
}

std::vector<std::string> Columns::get_coltypes() const{
    std::vector<std::string> colnames;
    for(const auto& col : cols){
        colnames.push_back(col.type);
    }
    return colnames;
}

int Columns::get_col_index(std::string colname) const{
    check_column_existence(colname, true, " doesnt exist. ");
    for(auto i = 0; i < cols.size(); i++){
        if(cols.at(i).name == colname){
            return i;
        }
    }
    return -1;
}





#endif
