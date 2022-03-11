#include <string>
#include <vector>


//add allowed types here?
class Columns{
    private:
        struct column{
            std::string name;
            std::string type;
            bool primary_key_flag;
            //par: nazev foreign tabulky, foreign key sloupec v teto tabulce
        };

        std::vector<column> cols;

    public:
        void add_column(std::string name, std::string type);
        void delete_column(std::string name);
        bool has_primary_key() const;
        void set_primary(std::string name);
};