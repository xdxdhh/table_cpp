Obrazek z;
ludek_portret = load_obrazek( .... url ....)

1 record = 1 vektor
1 table = list

Table t;
t.add_cols(['vek', 'int', 'jmeno', char, 'fotka', Obrazek]);

t.add_record(10, Ludek, ludek_portret); -->

t.add_record(put(Int,20), nullptr , put(String, 'blank'));
t.add_record(put(Int,20), put(Blank), put(String, "Martin"));
//pouziti blank: zastupne v tabulka, smazat vsechnny co maji blank



t.delete_record('jmeno', 'Ludek');

t.size();
// 30rows x 8 add_cols

t.show_cols();
// "vek (int), jmeno(char), fotka(Obrazek)"

t.delete_table();   
t.delete_record();

t.add_col('bydliste', char);


t.change('jmeno', 'ludek', 'vek' , 12)
t.change('id', '1', 'vek', 60);

xx = t.find('jmeno', 'ludek') --->list




/future
Table s;
s.add(xx);

Bold :
//kontrola ze je opravdu soucasti recordu
//print jako null



general notes:

-při loadování dat dát možnost volby vlastního id sloupce, v tom případě by se nevytvářel žádný vlastní 


Task list:
implementace indexu
--index vlastnost recordu?


# Table Class

### 1. Initialization and managing table

-should tables have their own name ? entity name  THINK\

__Table t;__   --initializes an empty table with an empty ID column \
-__t.add_col(name of column)__ --adds one column                                             
Column of blanks cannot be created  
-__t.add_cols(names of columns)__ --adds multiple columns\
example: t.add_cols({"sex", "Bool", "name", "String"});  
allowed types(see 4.Data types) are checked when adding column  
-__t.delete_col(column_name)__ -- deletes one col from all records  
-__t.delete_cols()__ --   delete multiple columns               TBD 
example: t.delete_cols({"name","age","color"});
-__t.rename_col()__  -- rename one column



-__t.get_cols()__ --returns vector with names of columns  
-__t.get_coltypes()__ --return vector with type names of columns  


-__t.truncate()__ --changes IDs so that they form integer sequence starting with 1 \
-__t.print()__ --prints a 'pretty formatted' table into console \
-__t.describe()__ --show some basic information about table, as number of columns,rows, column name and types  




### 2. Managing records

-__t.add_record(arbitrary number of Datas)__ --adding one record \
-example: t.add_record(put(Int,20), put(Bool,false), put(String, "Martin")); \
-when adding a record, types of Datas should be checked with col_types;  

t.add_record(record);   //private function  

When adding a record, the number of arguments and number of columns(not counting ID column) should be matching. \
For adding a blank data use Blank data type.  

-__t.delete_record(column_name, data)__ --deletes all records which contain data in a given column \
For deleting just one record, ids can be used, e.g. t.delete_record("id", 2);  

-__t.change_record(id, colname, new_data)__ --changes contents of colname column of data with this id TBD  

-__t.clear_records()__ --deletes all records, leaves an empty table with cols  



### 3. Working with multiple tables

-__t.find(column_name, data)__  returns Table with all of the records having data in that column \
Find can be chained, e.g: auto d = t.find("name", String("Martin")).find("age", Int(20));  


### 4. Data types structure

Currently there are 4 pre-implemented types of Data and those are Integer, String, Boolean and Blank. \
User can define his own type, which has to contain: \
1.type() function, which returns name of type, e.g. "Int", "Bool" \
2.to_str() function, some visual representation of data which allows the whole table to be printed into console\
3.to_bytes() function, that transform the data into bytes reprezentation(vector of bytes)\
4.clone() function, which returns a pointer to clone of the data  

When adding data to table, correctness of types is checked, Blank data type can be used in column of any type.  


# Record Class
TBD\

# Database structure

### Primary,unique and foreign key

Several functions can be used to set table keys.\
t.set_primary({colnames})\
t.unset_primary({colnames})\
t.set_foreign(colname, other_table)\
t.unset_foreign()\
t.set_unique(colname)\
t.unset_unique(colname)\
t.reset_keys() --deletes all keys, created id column which serves as primary key  


More cols can be used as combined primary key.\
Columns containing any part of primary key cannot be deleted.\
Combination of primary key must be unique.           --cim kontrolovat uniqueness?