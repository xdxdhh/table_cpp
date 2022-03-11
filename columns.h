#include <string>
#include <vector>
#include <map>
#include <stdexcept>

//add allowed types here?
class Columns{
    private:
        struct column{
            std::string name;
            std::string type;
            bool primary_key_flag;
            //foreign key flag?
            
        };

        std::vector<column> cols;
        std::map<std::string, std::string> foreign_keys;

        void check_column_existence(std::string name) const;

    public:
        void add_column(std::string name, std::string type);
        void delete_column(std::string name);
        bool has_primary_key() const;
        void set_primary(std::string name);
        bool is_primary(std::string name) const;
        
};


void Columns::add_column(std::string name, std::string type){
    //check if type is in allowed types TBD 
    //check if column of this name already exists TBD 
    auto& new_col = cols.emplace_back();
    new_col.name = name;
    new_col.primary_key_flag = false;
    new_col.type = type;
}

void Columns::delete_column(std::string name){
    //check if column of this name exists TBD 
    for(auto it = cols.begin(); it != cols.end(); it++){
        if(it->name == name){
            cols.erase(it);
        }
    }
    //delete from foreign key map tbd
}

bool Columns::has_primary_key() const{
    //check if column of this name exists TBD 
    for(const auto& col : cols){
        if(col.primary_key_flag == true)
            return true;
    }
    return false;
}

void Columns::set_primary(std::string name){
    //check if column of this name exists TBD 
    for(auto& col : cols){
        if(col.name == name)
            col.primary_key_flag = true;
    }
}

bool Columns::is_primary(std::string name) const{
    //check if column of this name exists TBD 
    for(const auto& col : cols){
        if(col.name == name)
            return col.primary_key_flag;
    }
}

void Columns::check_column_existence(std::string name) const{
    //TBD implementovat to_lower a pouzit
    for(const auto& col : cols){ //udelat z toho const_iterate_cols a iterate_cols?
        if(col.name == name)
            throw std::invalid_argument("Column of this name already exists.");
    }
};


std::string str_to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), 
        [](unsigned char c){ return std::tolower(c); }
        );
    return s;
}


bool colname_compare(std::string lhs, std::string rhs){
    if(str_to_lower(lhs) == str_to_lower(rhs))
        return true;
    return false;
}