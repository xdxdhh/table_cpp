#include "table.h"
#include "iostream"
#include "jsoncons/include/jsoncons/json.hpp"
#include <jsoncons_ext/jsonpath/jsonpath.hpp>
#include <jsoncons_ext/bson/bson.hpp>
#include <map>


using namespace std;
using namespace jsoncons;
using jsoncons::jsonpath::json_query;
namespace bson = jsoncons::bson;



int main(int argc, char const *argv[])
{
    Table t("ppl");
    t.add_col("vek","Int");
    t.add_cols({"pohlavi", "Bool", "jmeno", "String"});

    t.add_record(Int(20), Bool(false), String("Martin")); 
    t.add_record(Int(20), Bool(false), Blank());

    cout << t << endl;
    
    Table x("druzi lidi");
    x.add_col("vek","Int");
    x.add_cols({"pohlavi", "Bool", "jmeno", "String"});
    x.add_record(Int(20), Bool(false), String("Marsstin"));
    x.add_record(Int(20), Bool(false), String("X"));
    x.add_record(Int(20), Bool(false), String("Y"));
    cout << x << endl;

    x.delete_record("jmeno", String("X"));
    x.print();
    x.truncate();
    x.print();

    Table t2("lidi");
    t2.add_cols({"Jmeno", "String",  "Vek", "Int", "Pohlavi", "Bool"});
    t2.add_record(String("Jana"), Int(20), Bool(false));
    t2.add_record(String("Josef"), Int(44), Bool(true));
    t2.serialize(); 
    Table s("table_lidi.json", "json");
    if(t2 == s)
        cout << "yy.";
    t2.describe(); //porovnavame obsahy ne nazvy
    s.describe();
    cout << t2 << endl;
    cout << s << endl;
    s.rename_table("new lidi");
    cout << s << endl;
    
    
 
}

