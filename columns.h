#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <list>

// -> to solve
/* -> explaining */


#ifndef COLUMNS_H
#define COLUMNS_H
//add allowed types here or left in class?

class Columns{
    private:
        struct column{
            std::string name;
            std::string type;
            bool primary_key_flag;            
        };


        bool is_allowed(std::string type);
        void check_column_existence(std::string name, bool expected, std::string msg) const;
        bool colname_compare(std::string lhs, std::string rhs) const;

    public:
        std::map<std::string, std::string> foreign_keys; //getter or nope?
        static std::list<std::string> ALLOWED_TYPES; 
        std::vector<column> cols;

        void add_column(std::string name, std::string type);
        void delete_column(std::string name); 
        void rename_col(std::string oldname, std::string newname);

        bool has_primary_key() const; /* check if table has primary key */
        void set_primary(std::string name);
        bool is_primary(std::string name) const;

        /* getters */
        int get_colnum() const {return cols.size();};
        std::vector<std::string> get_colnames() const;
        std::vector<std::string> get_coltypes() const;
        int get_col_index(std::string colname) const;
        
        
};

/* helper function for compare */
std::string str_to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c){ return std::tolower(c); }
        );
    return s;
}

std::list<std::string> Columns::ALLOWED_TYPES = {"Int", "String", "Bool"}; /* Blank column cannot be created */

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
    check_column_existence(name, false, " already exists");
    auto& new_col = cols.emplace_back();
    new_col.name = name;
    new_col.primary_key_flag = false;
    new_col.type = type;
    std::cout << "Column " << name << " added." <<std::endl;
}

void Columns::delete_column(std::string name){
    check_column_existence(name, true, " was not found");
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


bool Columns::has_primary_key() const{
    for(const auto& col : cols){
        if(col.primary_key_flag == true)
            return true;
    }
    return false;
}

void Columns::set_primary(std::string name){
    check_column_existence(name, true, " was not found");
    for(auto& col : cols){
        if(col.name == name)
            col.primary_key_flag = true; //TBD chybi kontrola ze je to unique sloupecek
    }
}

bool Columns::is_primary(std::string name) const{
    check_column_existence(name, true, " was not found");
    for(const auto& col : cols){
        if(col.name == name)
            return col.primary_key_flag;
    }
    return false;
}


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
