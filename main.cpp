#include "table.h"
#include "iostream"
#include <vector>

using namespace std;

void print(Data &d)
{

}

//#define put(type, value) std::unique_ptr<Data>(dynamic_cast<Data*>(new type(value)))
//#define blank std::unique_ptr<Data>(dynamic_cast<Data*>(new Blank()))


int main(int argc, char const *argv[])
{
    /* Table t;
    t.add_col("vek","Int");
    t.add_cols({"pohlavi", "Bool", "jmeno", "String"});

    t.add_record(Int(20), Bool(false), String("Martin")); */
    //t.add_record(Int(20), Bool(false), Blank());
    //t.add_record(put<Int>(20), put<Bool>(false), put<String>("Martin"));
    //t.add_record(put<Int>(60), put<Blank>(), put<String>("Martin"));
    //t.add_record(put<Int>(40), put<Bool>(true) , put<String>("Agata"));

    /* cout << t << endl;
    
    Table x;
    x.add_col("vek","Int");
    x.add_cols({"pohlavi", "Bool", "jmeno", "String"});
    x.add_record(Int(20), Bool(false), String("Marsstin"));
    x.add_record(Int(20), Bool(false), String("X"));
    x.add_record(Int(20), Bool(false), String("Y"));


    x.delete_record("jmeno", String("X"));
    x.print();
    x.truncate();
    x.print(); */
    //auto d = t.find("jmeno", String("Martin")).find("vek", Int(20)) ;
    //d.print();

    Table t("human");
    t.add_cols({"name", "String", "age", "Int", "sex", "Bool"});
    t.add_record(String("Anna"), Int(20), Bool(false));
    t.add_record(String("Anna"), Int(20), Bool(true));

    t.print();
    t.describe();
    t.delete_col("age");
    t.print();
    t.delete_record("sex", Bool(false));
    t.print();
    t.add_record(String("Martin"), Blank());
    t.print();
    /* t.rename_col("name", "jmeno");
    t.print();
    t.describe();

    t.delete_cols({"age", "sex"});
    t.print(); 
 */
}

